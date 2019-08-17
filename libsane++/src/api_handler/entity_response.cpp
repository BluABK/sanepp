// Standard libraries.
#include <iostream>
#include <string>
#include <list>

// 3rd party libraries.
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Project specific libraries.
#include <api_handler/api_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>
#include <youtube/toolkit.hpp>

namespace sane {
    void APIHandler::printReport(int t_warningsCount, int t_errorsCount) {
        std::string reportedProblems;

        if (t_warningsCount > 0 and t_errorsCount > 0) {
            reportedProblems = " with " + std::to_string(t_warningsCount) + " warnings and "
                               + std::to_string(t_errorsCount) + " errors";
        }
        else if (t_warningsCount > 0) {
            reportedProblems = " with " + std::to_string(t_warningsCount) + " warnings";
        }
        else if (t_errorsCount > 0) {
            reportedProblems = " with " + std::to_string(t_errorsCount) + " errors";
        }

        // Return the parsed channels (and report possible issues).
        std::cout << "Processing completed" << reportedProblems << "." << std::endl;
    }

    void APIHandler::printReport(std::shared_ptr<YoutubeChannel> &t_channel) {
        int warningsCount = t_channel->getWarnings().size();
        int errorsCount = t_channel->getErrors().size();

        printReport(warningsCount, errorsCount);
    }

    void APIHandler::getSubscriptionsEntities(bool clearProblems) {
        std::list <std::shared_ptr<YoutubeChannel>> channels;
        // snippet is required because 'id' part is the ID of the subscription, not the channel.
        const std::string part = "snippet";
        std::map<std::string, std::string> filter;
        std::map<std::string, std::string> optParams;
        size_t warningsCount = 0;
        size_t errorsCount = 0;
        bool hasNextPage = true;
        int totalResults = 0;
        int counter = 1;

        filter["mine"] = "true";
        optParams["maxResults"] = "50";

        // Iterate the pages in the response.
        while (hasNextPage) {
            nlohmann::json subscriptionsPageJson = youtubeListSubscriptions(part, filter, optParams);

            if (hasItems(subscriptionsPageJson)) {
                // Iterate the subscription resource items on the page
                for (const auto &subscription : subscriptionsPageJson["items"]) {
                    std::map<std::string, std::string> channelFilter;
                    std::string progressPercentString;
                    std::string subscriptionTitle = subscription["snippet"]["title"].get<std::string>();

                    totalResults = subscriptionsPageJson["pageInfo"]["totalResults"].get<int>();

                    // Define the current progress as a whole string line
                    std::string progressLine = std::to_string(counter) + "/" + std::to_string(totalResults);
                    float progressPercent = (float)counter / totalResults * 100;

                    if (counter < totalResults) {
                        progressPercentString = std::to_string(progressPercent).substr(0, 4);
                    } else {
                        // Handle "100." case, where there's no decimals, only the decimal point.
                        progressPercentString = std::to_string(progressPercent).substr(0, 3);
                    }


                    // Return to start of line and overwrite with progressLine (works cos it never shrinks in length)
                    try {
                        std::cout << "\r" << "Retrieving " << "subscriptions... "
                                  << progressPercentString << "% " << "(" << progressLine << "): "
                                  << subscriptionTitle
                                  << std::string(60 - subscriptionTitle.length(), ' ')
                                  << std::flush;
                    } catch (const std::exception &exc) {
                        std::cerr << "APIHandler::getSubscriptionsEntities unhandled exception on '"
                                  << subscriptionTitle <<  "': " << std::string(exc.what()) << std::endl;
                    }
                    channelFilter["id"] = subscription["snippet"]["resourceId"]["channelId"].get<std::string>();

                    // Get a proper Channel resource from the rudimentary Subscription resource.
                    nlohmann::json channelJson = youtubeListChannels("id,snippet,contentDetails", channelFilter);

                    if (hasItems(channelJson)) {
                         std::shared_ptr<YoutubeChannel> channel;
                         channel = std::make_shared<YoutubeChannel>(channelJson["items"][0]);

                        if (channel->wasAborted()) {
                            // Explicitly delete the broken channel object now instead of waiting for smart ptr deallocation.
                            channel.reset();
                            std::cerr << "ERROR: Creation of the following channel was aborted:\n"
                                      << channelJson.dump(4)
                                      << std::endl;
                        } else {
                            // Update total warnings and errors counter.
                            warningsCount += channel->getWarnings().size();
                            errorsCount += channel->getErrors().size();

                            // Add the channel entity to the list.
                            channels.push_back(channel);
                        }

                    } else {
                        std::cerr << "Channels.list() resource had no items!\n" << subscriptionsPageJson.dump(4)
                                  << std::endl;
                    }
                    counter++;
                } // for subscription
            } else {
                std::cerr << "Subscriptions.list() resource page had no items!\n" << subscriptionsPageJson.dump(4)
                          << std::endl;
            }
            // Traverse to next page
            if (subscriptionsPageJson.find("nextPageToken") != subscriptionsPageJson.end()) {
                if (subscriptionsPageJson["nextPageToken"].is_string()
                    and !subscriptionsPageJson["nextPageToken"].empty()) {
                        // Set the pageToken for next page to be requested at start of loop.
                        optParams["pageToken"] = subscriptionsPageJson["nextPageToken"].get<std::string>();
                } else {
                    hasNextPage = false;
                }
            } else {
                hasNextPage = false;
            }
        } // while (hasNextPage)
        // Newline after one-line progressbar.
        std::cout << std::endl;

        printReport(warningsCount, errorsCount);

        if (clearProblems) {
            // Clear the warnings and errors in the channel objects to save resources.
            for (auto &channel : channels) {
                channel->clearErrorsAndWarnings();
            }
        }

        // Store to Database
        std::cout << "Storing to database..." << std::endl;
        addChannelsToDB(channels, NO_ERROR_LOG);
        std::cout << "Storing to database successful!" << std::endl;
    }

} // namespace sane