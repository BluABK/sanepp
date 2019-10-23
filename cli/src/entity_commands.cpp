#include <youtube/toolkit.hpp>
#include "cli.hpp"

namespace sane {
    /**
     * Retrieves a list of YouTube subscription objects from YouTube API via SaneAPI
     */
    void CLI::getSubscriptionsFromApi() {
        log->info("Getting subscriptions from API handler.");
        api->getSubscriptionsEntities();
    }

    void CLI::printSubscriptionsFull() {
        log->info("Printing subscriptions (full).");
        // Fetch list of channels
        std::list <std::shared_ptr<YoutubeChannel>> channels;
        channels = sane::getChannelsFromDB(NO_ERROR_LOG);
        int counter = 1;  // Humanized counting.
        for (auto & channel : channels) {
            std::list<std::map<std::string, nlohmann::json>> warnings = channel->getWarnings();
            std::list<std::map<std::string, nlohmann::json>> errors = channel->getErrors();

            std::cout << "Sub#" << counter << ":" << std::endl;
            channel->print(DEFAULT_INDENT);
            // Print any warnings and errors
            if (!warnings.empty()) {
                log->info(std::to_string(warnings.size()) + " warnings for '" +
                          channel->getChannelId() + "' (" + channel->getTitle() + "):");
                channel->logWarnings();
                std::cout << "Warnings:\n" << std::endl;
                channel->printWarnings(4);
            }
            if (!errors.empty()) {
                std::cerr << "Errors:\n" << std::endl;
                channel->printErrors(4);
            }
            counter++;
        }
    }

    void CLI::printSubscriptionsBasic() {
        log->info("Printing subscriptions (basic).");
        // Fetch list of channels
        std::list <std::shared_ptr<YoutubeChannel>> channels;
        channels = sane::getChannelsFromDB(NO_ERROR_LOG);
        // Spacing between each column item.
        const std::string columnSpacing(4, ' ');
        // Pad subscription counter based on the amount of digits in the sum total.
        const std::size_t maxCounterDigitAmount = std::to_string(channels.size()).length();
        // Max length of IDs, used to calculate padding.
        const std::size_t idItemMaxLength = 24;
        // Humanized counting.
        int counter = 1;

        // Print a table heading/legend.
        std::cout << padStringValue("#", maxCounterDigitAmount) << columnSpacing <<
                  padStringValue("Channel ID", idItemMaxLength) << columnSpacing <<
                  padStringValue("Uploads playlist ID", idItemMaxLength) <<
                  columnSpacing << "Channel title" <<  std::endl;

        for (auto & channel : channels) {
            // Pad item columns with spaces to ensure a uniform indentation.
            const std::string paddedChannelId = padStringValue(channel->getId(), idItemMaxLength);
            const std::string paddedUploadsPlaylistId = padStringValue(
                    channel->getUploadsPlaylist(), idItemMaxLength);
            const std::string paddedNumbering = padStringValue(std::to_string(counter), maxCounterDigitAmount);

            std::cout << paddedNumbering << columnSpacing <<
                      paddedChannelId << columnSpacing << paddedUploadsPlaylistId << columnSpacing << channel->getTitle()
                      << std::endl;

            counter++;
        }
    }

    void CLI::printChannelFromApiByName(const std::string &t_input) {
        const std::string part = "snippet";
        std::map<std::string, std::string> filter;
        std::map<std::string, std::string> optParams;

        filter["forUsername"] = t_input;
        optParams["maxResults"] = "1";
        nlohmann::json channelListJson = api->youtubeListChannels(part, filter, optParams);

        if (hasItems(channelListJson)) {
            std::shared_ptr<YoutubeChannel> channel = std::make_shared<YoutubeChannel>(channelListJson["items"][0]);

            channel->print(DEFAULT_INDENT);
        } else {
            std::cerr << "CLI::printChannelFromApiByName Error: Result had no items:\n" << channelListJson.dump(4)
                      << std::endl;
        }
    }

    void CLI::printChannelFromApiByName(const std::vector<std::string> &t_input) {
        if (t_input.empty()) {
            std::cout << "Error: no arguments given, required: 1." << std::endl;
        } else {
            printChannelFromApiByName(t_input.front());
        }
    }

    void CLI::printChannelFromApiById(const std::string &t_input) {
        const std::string part = "snippet";
        std::map<std::string, std::string> filter;
        std::map<std::string, std::string> optParams;

        filter["id"] = t_input;
        optParams["maxResults"] = "1";
        nlohmann::json channelListJson = api->youtubeListChannels(part, filter, optParams);

        if (hasItems(channelListJson)) {
            std::shared_ptr<YoutubeChannel> channel = std::make_shared<YoutubeChannel>(channelListJson["items"][0]);

            channel->print(DEFAULT_INDENT);
        } else {
            std::cerr << "CLI::printChannelFromApiById Error: Result had no items:\n" << channelListJson.dump(4)
                      << std::endl;
        }
    }

    void CLI::printChannelFromApiById(const std::vector<std::string> &t_input) {
        if (t_input.empty()) {
            std::cout << "Error: no arguments given, required: 1." << std::endl;
        } else {
            printChannelFromApiById(t_input.front());
        }
    }

    void CLI::printVideosFromApi(const std::vector<std::string> &t_input, bool t_printFullInfo) {
        nlohmann::json videosJson = listVideosJsonFromApi(t_input);
        if (!videosJson.empty() and videosJson.find("items") != videosJson.end()) {
            for (nlohmann::json videoItemJson : videosJson["items"]) {
                std::shared_ptr<YoutubeVideo> video = std::make_shared<YoutubeVideo>(videoItemJson);

                std::list<std::map<std::string, nlohmann::json>> warnings = video->getWarnings();
                std::list<std::map<std::string, nlohmann::json>> errors = video->getErrors();

                // Print video info.
                video->print(DEFAULT_INDENT, t_printFullInfo);

                // Print any warnings and errors.
                if (!warnings.empty()) {
                    std::cout << "\nWarnings:" << std::endl;
                    video->printWarnings(DEFAULT_INDENT, true);
                }
                if (!errors.empty()) {
                    std::cout << "\nErrors:" << std::endl;
                    video->printErrors(DEFAULT_INDENT, true);
                }

            }
        }
    }
} // namespace sane