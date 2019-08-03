// Standard libraries.
#include <iostream>
#include <string>
#include <list>

// 3rd party libraries.
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Project specific libraries.
#include <api_handler/entity_response.hpp>
#include <api_handler/api_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>

namespace sane {
    void printReport(int t_warningsCount, int t_errorsCount) {
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

    void printReport(std::shared_ptr<YoutubeChannel> &t_channel) {
        int warningsCount = t_channel->getWarnings().size();
        int errorsCount = t_channel->getErrors().size();

        printReport(warningsCount, errorsCount);
    }

    void sapiRemoteGetSubscriptions(bool clearProblems) {
        std::list <std::shared_ptr<YoutubeChannel>> channels;

        std::cout << "Retrieving subscriptions from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_SUBSCRIPTIONS);
        std::cout << "Got response from SaneAPI, processing " << jsonData.size() << " channels..." << std::endl;

        // iterate the JSON array of multiple channels and append a YoutubeChannel.
        size_t warningsCount = 0;
        size_t errorsCount = 0;
        for (auto & subscriptionJson : jsonData) {
            // Create a new YoutubeChannel object for each channel.
            std::shared_ptr<YoutubeChannel> channel = std::make_shared<YoutubeChannel>(subscriptionJson);
            if (channel->wasAborted()) {
                // Explicitly delete the broken channel object now instead of waiting for smart ptr deallocation.
                channel.reset();
                std::cerr << "\tERROR: Creation of the following channel was aborted:" << std::endl;
                std::cerr << jsonData.dump(4);
            } else {
                warningsCount += channel->getWarnings().size();
                errorsCount += channel->getErrors().size();

                // Append channels list with the new YoutubeChannel object.
                channels.push_back(channel);
            }
        }

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


    std::shared_ptr<YoutubeChannel> sapiRemoteGetChannelByUsername(const std::string &t_username, bool clearProblems) {
        std::cout << "Retrieving channel '" << t_username << "' from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_CHANNEL "?username=" + t_username);
        std::cout << "Got response from SaneAPI, processing channel..." << std::endl;

        // Create a new YoutubeChannel object for the requested channel.
        std::shared_ptr<YoutubeChannel> channel = std::make_shared<YoutubeChannel>(jsonData);

        if (channel->wasAborted()) {
            // Explicitly delete the broken channel object now instead of waiting for smart ptr deallocation.
            channel.reset();
            std::cerr << "\tERROR: Creation of the following channel was aborted:" << std::endl;
            std::cerr << jsonData.dump(4);

            return nullptr;
        } else {
            // Print a report of any errors or warnings that might have occurred.
            printReport(channel);

            if (clearProblems) {
                // Clear the warnings and errors in the channel object to save resources.
                channel->clearErrorsAndWarnings();
            }

            // Return the new YoutubeChannel object.
            return channel;
        }
    }

    std::shared_ptr<YoutubeChannel> sapiRemoteGetChannelById(const std::string &t_channelId, bool clearProblems) {
        std::cout << "Retrieving channel '" << t_channelId << "' from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_CHANNEL "?id=" + t_channelId);
        std::cout << "Got response from SaneAPI, processing channel..." << std::endl;

        // Create a new YoutubeChannel object for the requested channel.
        std::shared_ptr<YoutubeChannel> channel = std::make_shared<YoutubeChannel>(jsonData);

        if (channel->wasAborted()) {
            // Explicitly delete the broken channel object now instead of waiting for smart ptr deallocation.
            channel.reset();
            std::cerr << "\tERROR: Creation of the following channel was aborted:" << std::endl;
            std::cerr << jsonData.dump(4);

            return nullptr;
        } else {
            // Print a report of any errors or warnings that might have occurred.
            printReport(channel);

            if (clearProblems) {
                // Clear the warnings and errors in the channel object to save resources.
                channel->clearErrorsAndWarnings();
            }

            // Return the new YoutubeChannel object.
            return channel;
        }
    }
} // namespace sane