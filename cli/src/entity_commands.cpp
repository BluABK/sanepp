#include "cli.hpp"

namespace sane {
    /**
     * Retrieves a list of YouTube subscription objects from YouTube API via SaneAPI
     */
    void CLI::getSubscriptionsFromApi() {
        api->sapiRemoteGetSubscriptions();
    }

    void CLI::printSubscriptionsFull() {
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
                std::cout << "Warnings:\n" << std::endl;
                channel->printWarnings(4);
            }
            if (!errors.empty()) {
                std::cout << "Errors:\n" << std::endl;
                channel->printErrors(4);
            }
            counter++;
        }
    }

    void CLI::printSubscriptionsBasic() {
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
        std::shared_ptr<YoutubeChannel> channel = api->sapiRemoteGetChannelByUsername(t_input);
        channel->print(DEFAULT_INDENT);
    }

    void CLI::printChannelFromApiByName(const std::vector<std::string> &t_input) {
        if (t_input.empty()) {
            std::cout << "Error: no arguments given, required: 1." << std::endl;
        } else {
            printChannelFromApiByName(t_input.front());
        }
    }

    void CLI::printChannelFromApiById(const std::string &t_input) {
        std::shared_ptr<YoutubeChannel> channel = api->sapiRemoteGetChannelById(t_input);
        channel->print(DEFAULT_INDENT);
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