#include <youtube/subfeed.hpp>
#include <algorithm>

#include "cli.hpp"

namespace sane {
    void CLI::printPlaylistVideos(const std::string &t_playlistId,
            const std::map<std::string,std::string> &t_optParams) {
        const std::string part = "snippet,status";
        std::map<std::string, std::string> filter;
        const std::map<std::string,std::string> &optParams = t_optParams;
        nlohmann::json playlistJson;

        filter["playlistId"] = t_playlistId;

        // Get list of videos.
        playlistJson = api->youtubeListPlaylistItems(part, filter, optParams);

        // Store page info.
        int resultsPerPage = playlistJson["pageInfo"]["resultsPerPage"].get<int>();
        int totalResults = playlistJson["pageInfo"]["totalResults"].get<int>();
        std::string nextPage = playlistJson["nextPageToken"].get<std::string>();

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

            std::string channelId = playlistItem["snippet"]["channelId"].get<std::string>();
            std::string channelTitle = playlistItem["snippet"]["channelTitle"].get<std::string>();

            int playlistPosition = playlistItem["snippet"]["position"].get<int>();

            // Humanize date
            videoPublishDate = videoPublishDate.substr(0, videoPublishDate.size() -5);
            std::replace(videoPublishDate.begin(), videoPublishDate.end(), 'T', ' ');

            // Print table item.
            std::cout << playlistPosition << "\t" << videoPublishDate << indent << videoId << indent
            << videoPrivacyStatus << indent << channelTitle << channelIndent << videoTitle << std::endl;

        }
        // Print page info/summary.
        std::cout << std::endl << "Showing " << resultsPerPage << "/" << totalResults << " results."
        << " Next page: " << nextPage << std::endl;
    }

    /**
     * Gets the string length of the longest channel title in DB.
     * @return
     */
    size_t CLI::getLongestChannelTitleLength() {
        // Get subscriptions.
        std::list<std::string> errors;
//        std::cout << "Retrieving subscriptions from DB..." << std::endl;
        std::list <std::shared_ptr<YoutubeChannel>> channels = getChannelsFromDB(&errors);

        // Retrieve uploaded videos playlist IDs.
//        std::cout << "Retrieving \"uploaded videos\" playlists..." << std::endl;
        std::list<std::string> playlists;
        size_t longestChannelTitleLength = 0;
        for (const auto& channel : channels) {
            // Get the longest channel title (used in indent calculation).
            if (channel->getTitle().length() > longestChannelTitleLength) {
                longestChannelTitleLength = channel->getTitle().length();
            }
        }

        return longestChannelTitleLength;
    }

    /**
     * Prints the subscriptions feed videos as a nicely indented table.
     *
     * @param t_videoLimit
     * @param t_part
     * @param t_filter
     * @param t_optParams
     */
    void CLI::printSubscriptionsFeed(int t_videoLimit,
                                     const std::string &t_part,
                                     const std::map<std::string, std::string> &t_filter,
                                     const std::map<std::string, std::string> &t_optParams) {
        size_t longestChannelTitleLength = getLongestChannelTitleLength();

        // Get list of subscriptions feed videos.
//        std::cout << "Retrieving videos from \"uploaded videos\" playlists..." << std::endl;
        std::list<std::shared_ptr<YoutubeVideo>> videos = createSubscriptionsFeed(t_part, t_filter, t_optParams);

        // Handle any limits (0 == disable limit)
        if (t_videoLimit > 0) {
            // Iterate t_videoLimit amount of videos from the original list.
            auto lastVideoIterator = std::next(videos.begin(), std::min((size_t)t_videoLimit, videos.size()));

            // Make a new list containing the first n elements of the first list
            std::list <std::shared_ptr<YoutubeVideo>> limited(videos.begin(), lastVideoIterator);

            // Replace the original list with the new limited version.
            videos = limited;
        }

        // Printing section

        // Table headings.
        const std::string publishDateHeading    = "Published on";
        const std::string definitionHeading     = "Definition";
        const std::string videoUrlHeading       = "URL";
        const std::string hasCaptionsHeading    = "Captions?";
        const std::string channelTitleHeading   = "Channel Title";
        const std::string videoTitleHeading     = "Title";

        // Table contents helpers.
        const std::string youtubeVideoURLBase   = "https://www.youtube.com/watch?v=";

        // Indent stuff.
        int indentLength = 4;
        std::string indent = std::string(indentLength, ' ');
        std::string doubleIndent = std::string(7, ' ');
        std::string tripleIndent = std::string(11, ' ');

        // Determine Spacing between channel title and video title.
        std::string longestChannelTitleOffset = std::string(longestChannelTitleLength, ' ');
        const std::string urlIndent = std::string(youtubeVideoURLBase.length() + 11, ' ');
        const std::string urlHeadingIndent = std::string(urlIndent.length() - 3, ' ');

        // Set a default indent value.
        std::string channelHeadingIndent = std::string(longestChannelTitleLength, ' ');
        if (longestChannelTitleLength < channelTitleHeading.length()) {
            // If Longest channel title is shorter than heading.
            longestChannelTitleOffset = std::string(channelTitleHeading.length() - longestChannelTitleLength, ' ');
            channelHeadingIndent = std::string(indentLength, ' ');
        } else if (longestChannelTitleLength > channelTitleHeading.length()) {
            // If Longest channel title is longer than heading.
            channelHeadingIndent = std::string(longestChannelTitleLength - channelTitleHeading.length(), ' ');
        }

        std::cout << std::endl;
        std::cout << "#" << "\t" << publishDateHeading << tripleIndent << videoUrlHeading << urlHeadingIndent
                  << indent << definitionHeading << indent
                  << hasCaptionsHeading << indent << channelTitleHeading << channelHeadingIndent << indent
                  << videoTitleHeading << std::endl;
        int position = 0;
        for (const auto& video: videos) {
            const std::string videoId = video->getId();
            const std::string videoTitle = video->getTitle();
            const std::string videoPrivacyStatus = video->getPrivacyStatus();
            const std::string videoPublishDate = video->getPublishedAt().isoDateAndTime;

            const std::string channelId = video->getChannelId();
            const std::string channelTitle = video->getChannelTitle();
            const std::string definition = video->isHD() ? "HD" : "SD";
            const std::string hasCaptions = video->hasCaptions() ? "Yes." : "No.";

            // Per-item offsets.
            const std::string definitionOffset = std::string(definitionHeading.length() - definition.length(), ' ');
            const std::string hasCaptionsOffset = std::string(hasCaptionsHeading.length() - hasCaptions.length(), ' ');
            const std::string channelTitleOffset = std::string(longestChannelTitleLength - channelTitle.length(), ' ');

            // Print table item.
            std::cout << position << "\t" << videoPublishDate << indent
                      << youtubeVideoURLBase << videoId << indent
                      << definition << definitionOffset << indent << hasCaptions << hasCaptionsOffset << indent
                      << channelTitle << channelTitleOffset << indent << videoTitle << std::endl;

            position++;
        }
    }
} // namespace sane