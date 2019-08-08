#include "cli.hpp"
namespace sane {
    void CLI::printPlaylistVideos(const std::string &t_playlistId,
            const std::map<std::string,std::string> &t_optParams) {
        const std::string part = "snippet,status";
        std::map<std::string, std::string> filter;
        const std::map<std::string,std::string> &optParams = t_optParams;
        nlohmann::json playlistJson;

        filter["playlistId"] = t_playlistId;

        // Get list of videos
        playlistJson = api->sapiGetPlaylistItemsList(part, filter, optParams);

        // Indent stuff.
        int indentLength = 4;
        std::string indent = std::string(indentLength, ' ');
        std::string doubleIndent = std::string(7, ' ');
        std::string tripleIndent = std::string(11, ' ');

        // Determine Spacing between channel title and video title.
        size_t channelTitleLength = playlistJson["items"][0]["snippet"]["channelTitle"].get<std::string>().length();
        size_t channelHeadingIndent;
        size_t channelHeadingLength = std::string("Channel Title").length();
        std::string channelIndent;

        channelHeadingIndent = channelTitleLength;
        if (channelTitleLength < channelHeadingLength) {
            // If Channel title is shorter than heading.
            channelIndent = std::string(channelHeadingLength - channelTitleLength + indentLength, ' ');
            channelHeadingIndent = indentLength;
        } else if (channelTitleLength > channelHeadingLength) {
            // If Channel title is longer than heading.
            channelHeadingIndent -= (channelHeadingLength - indentLength);
            channelIndent = std::string(indentLength, ' ');
        }

        std::cout << "Pos" << "\t" << "Published on" << tripleIndent << "Video ID" << doubleIndent << "Privacy" << "   "
        << "Channel Title" << std::string(channelHeadingIndent, ' ') << "Title" << std::endl;
        for (const auto& playlistItem: playlistJson["items"]) {
            std::string videoId = playlistItem["snippet"]["resourceId"]["videoId"].get<std::string>();
            std::string videoTitle = playlistItem["snippet"]["title"].get<std::string>();
            std::string videoPrivacyStatus = playlistItem["status"]["privacyStatus"].get<std::string>();
            std::string videoPublishDate = playlistItem["snippet"]["publishedAt"].get<std::string>();
            std::string description;
            std::string descriptionAsOneLine;

            std::string channelId = playlistItem["snippet"]["channelId"].get<std::string>();
            std::string channelTitle = playlistItem["snippet"]["channelTitle"].get<std::string>();

            int playlistPosition = playlistItem["snippet"]["position"].get<int>();

            // Humanize date
            videoPublishDate = videoPublishDate.substr(0, videoPublishDate.size() -5);
            std::replace(videoPublishDate.begin(), videoPublishDate.end(), 'T', ' ');
//            description, descriptionAsOneLine = playlistItem["snippet"]["description"].get<std::string>();
//            std::replace(descriptionAsOneLine.begin(), descriptionAsOneLine.end(), '\n', '|');

            std::cout << playlistPosition << "\t" << videoPublishDate << indent << videoId << indent
            << videoPrivacyStatus << indent << channelTitle << channelIndent << videoTitle << std::endl;
        }
    }
} // namespace sane