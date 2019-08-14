#include <iostream>
#include <string>
#include <list>
#include <vector>

#include <entities/common.hpp>
#include <entities/youtube_channel.hpp>
#include <entities/youtube_video.hpp>
#include <api_handler/api_handler.hpp>

#include <youtube/subfeed.hpp>

namespace sane {
    // Get list of uploaded videos for a channel
    std::shared_ptr<YoutubeVideo> getPlaylistItemsList(const std::string &t_part,
                                                       const std::map<std::string,std::string> &t_filter,
                                                       const std::map<std::string,std::string> &t_optParams) {
        nlohmann::json jsonData;

        // Instantiate API Handler
        std::shared_ptr<sane::APIHandler> api = std::make_shared<sane::APIHandler>();

        jsonData = api->sapiGetPlaylistItemsList(t_part, t_filter, t_optParams);
    }

    std::list<std::shared_ptr<YoutubeVideo>> listUploadedVideos(const std::list<std::string> &t_playlists,
                                                                const std::string &t_part,
                                                                const std::map<std::string, std::string> &t_filter,
                                                                const std::map<std::string, std::string> &t_optParams) {
        std::list<std::shared_ptr<YoutubeVideo>> videos;
        nlohmann::json jsonData;

        // Instantiate API Handler
        std::shared_ptr<sane::APIHandler> api = std::make_shared<sane::APIHandler>();

        // For playlist in t_playlists
        // TODO: This probably needs threading
        int counter = 1;
        for (const auto& playlist : t_playlists) {
            std::cout << "Retrieving playlist " << counter << " / " << t_playlists.size() << "\t" << playlist
                      << std::endl;

            // YouTube API essentials
            const std::string part = "snippet,contentDetails";

            // Add passed filters and optional parameters.
            std::map<std::string,std::string> filter = t_filter;
            const std::map<std::string,std::string>& optParams = t_optParams;

            if (filter.find("id") != filter.end()) {
                std::cerr << "listUploadedVideos ERROR: FILTER id=" << filter["id"]
                          << " was specified but playlistId is wanted! Unsetting id key." << std::endl;

                // Unset the id filter (there can only be one).
                filter.erase("id");
            }

            if (filter.find("id") != filter.end()) {
                std::cerr << "listUploadedVideos ERROR: FILTER playlistId=" << filter["playlistId"]
                          << " was specified but will be overridden with " << playlist << "!" << std::endl;
            }

            // Set the playlist id.
            filter["playlistId"] = playlist;

            // Make the SAPI request and retrieve JSON.
            jsonData = api->sapiGetPlaylistItemsList(t_part, t_filter, t_optParams);

            // Make sure the response was valid.
            if (!jsonData.empty()) {
                // For video item in response // FIXME: No pagination support, will cutoff at 50 max.
                for (auto playlistItemJson : jsonData) {
                    // Create the YoutubeVideo entity.
                    std::shared_ptr<YoutubeVideo> video = std::make_shared<YoutubeVideo>(playlistItemJson);

                    // Finally append the video to the list.
                    videos.push_back(video);
                }
            }
        }

        return videos;
    }

    // Create subfeed from a list of channel uploaded videos playlists.
    void createSubscriptionsFeed(const std::list<std::string> &t_playlists) {
        // YouTube API essentials
        const std::string part = "snippet,contentDetails";
        // Any filter besides "id="
        const std::map<std::string,std::string> filter;
        const std::map<std::string,std::string> optParams;

        // Video uploads
        std::list<std::shared_ptr<YoutubeVideo>> videos;

        // Get list of uploaded videos for every given channel/playlist.
        videos = listUploadedVideos(t_playlists, part, filter, optParams);

        // Sort by publishedAt date.
        std::cout << "Sorting subfeed videos by publishedAt datetime..." << std::endl;
//        videos.sort( [](const YoutubeVideo &video1, const YoutubeVideo &video2) );
        videos.sort(YoutubeVideoPublishedAtComparator());
    }

}