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
        nlohmann::json playlistItemsJson;
        nlohmann::json videoListJson;

        // Instantiate API Handler
        std::shared_ptr<sane::APIHandler> api = std::make_shared<sane::APIHandler>();

        // For playlist in t_playlists
        // TODO: This probably needs threading
        int playlistCounter = 1;
        for (const auto& playlist : t_playlists) {
            std::cout << "Retrieving playlist " << playlistCounter << " / " << t_playlists.size() << "\t" << playlist
                      << std::endl;

            // Create an actual useful part to be used with videos.list().
            const std::string informativePart = t_part + "," + "snippet";

            // Add passed filters and optional parameters.
            std::map<std::string,std::string> filter = t_filter;
            const std::map<std::string,std::string>& optParams = t_optParams;

            if (filter.find("id") != filter.end()) {
                std::cerr << "listUploadedVideos ERROR: FILTER id=" << filter["id"]
                          << " was specified but playlistId is wanted! Unsetting id key." << std::endl;

                // Unset the id filter (there can only be one).
                filter.erase("id");
            }

            if (filter.find("playlistId") != filter.end()) {
                std::cerr << "listUploadedVideos ERROR: FILTER playlistId=" << filter["playlistId"]
                          << " was specified but will be overridden with " << playlist << "!" << std::endl;
            }

            // Set the current playlist id.
            filter["playlistId"] = playlist;

            // Make the SAPI request and retrieve (a rather limited) JSON.
            // NB: Note using t_part (likely only contentDetails), not part.
            playlistItemsJson = api->sapiGetPlaylistItemsList(t_part, filter, optParams);

            // Make sure the playlistItemsJson response was valid.
            if (!playlistItemsJson.empty()) {
                // Do some separate videos.list() API request for current playlist items to actually obtain useful info:

                // 1. Clear playlistItems-specific filters:
                filter.erase("playlistId");

                // 2. Populate filter 'id=' with list of video IDs from the playlistItems response.
                bool firstItem = true;
                for (const auto& playlistItemJson : playlistItemsJson["items"]) {
                    // The "id" field in playlistItem is the item's ID, not the video's.

                    // The actual video ID can be found inside of the parts.
                    if (playlistItemJson.find("contentDetails") != playlistItemJson.end()) {
                        if (firstItem) {
                            // Don't prepend comma to first item.
                            filter["id"] = playlistItemJson["contentDetails"]["videoId"].get<std::string>();

                            firstItem = false;
                        } else {
                            // Append id to string, prepended with comma.
                            filter["id"] += "," + playlistItemJson["contentDetails"]["videoId"].get<std::string>();
                        }
                    } else {
                        // Kind is playlistItem, but no snippet or contentDetails were provided.
                        std::cerr << "listUploadedVideos Error: Unable to set video ID: Kind is youtube#playlistItem, "
                                  << "but contentDetails parts was not available!" << std::endl;
                    } // if contentDetails in playlistItemJson
                } // for playlistItemJson in current playlistItemsJson

                // 3. Request proper information for the current video IDs using the API's videos.list().
                std::cout << "\tRetrieving additional video info... " << std::endl;
                videoListJson = api->sapiGetVideosList(informativePart, filter, optParams);

                // Make sure the videoListJson response was valid.
                if (!videoListJson.empty()) {
                    // For video item in response // FIXME: No pagination support, will cutoff at 50 max.
                    for (auto videoJson : videoListJson["items"]) {
                        // Create the YoutubeVideo entity.
                        std::shared_ptr<YoutubeVideo> video = std::make_shared<YoutubeVideo>(videoJson);

                        // Finally append the video to the list.
                        videos.push_back(video);
                    } // for video in videoListJson
                } // if videoListJson not empty
            } // if playlistItemsJson not empty
            playlistCounter++;
        } // for playlist in t_playlists

        return videos;
    }

    // Create subfeed from a list of channel uploaded videos playlists.
    std::list<std::shared_ptr<YoutubeVideo>> createSubscriptionsFeed(const std::list<std::string> &t_playlists) {
        // YouTube API essentials:
        // Due to the limited amount of info in youtube#playlistItems we'll just request the part that holds videoId,
        // and then we'll do a separate videos.list() API request for those IDs further down the line.
        const std::string part = "contentDetails";
        // Any filter besides "id="
        std::map<std::string,std::string> filter;
        std::map<std::string,std::string> optParams;

        // Video uploads
        std::list<std::shared_ptr<YoutubeVideo>> videos;

        // Set maxResults // FIXME: bake into parameter
        optParams["maxResults"] = "50";

        // Get list of uploaded videos for every given channel/playlist.
        videos = listUploadedVideos(t_playlists, part, filter, optParams);

        // Sort by publishedAt date.
        std::cout << "Sorting subfeed videos by publishedAt datetime..." << std::endl;
        videos.sort(sortYoutubeVideoDateDescending());

        return videos;
    }

}