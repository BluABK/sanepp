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
    bool hasItems(const nlohmann::json &t_jsonList) {
        // JSON is valid (not empty object).
        if (!t_jsonList.empty()) {
            // JSON contains pageInfo object.
            if (t_jsonList.find("pageInfo") != t_jsonList.end()) {
                // JSON pageInfo object contains totalResults item.
                if (t_jsonList["pageInfo"].find("totalResults") != t_jsonList["pageInfo"].end()) {
                    // JSON pageInfo object totalResults is a valid numeric value.
                    if (t_jsonList["pageInfo"]["totalResults"].is_number()) {
                        // JSON pageInfo object totalResults numeric value is larger than zero.
                        if (t_jsonList["pageInfo"]["totalResults"].get<int>() > 0) {
                            return true;
                        }
                    } else {
                        std::cerr <<  "hasItems Error: JSON[pageInfo][totalResults] is not numeric:\n"
                                  << t_jsonList.dump(4) << std::endl;
                    }
                } else {
                    std::cerr <<  "hasItems Error: JSON[pageInfo] does not contain totalResults:\n"
                              << t_jsonList.dump(4) << std::endl;
                }
            } else {
                std::cerr <<  "hasItems Error: JSON does not contain pageInfo:\n" << t_jsonList.dump(4) << std::endl;
            }
        }
        return false;
    }

    std::list<std::shared_ptr<YoutubeVideo>> listUploadedVideos(const std::list<std::string> &t_playlists,
                                                                const std::string &t_part,
                                                                const std::map<std::string, std::string> &t_filter,
                                                                const std::map<std::string, std::string> &t_optParams,
                                                                const std::string &t_playlistItemsPart) {
        std::list<std::shared_ptr<YoutubeVideo>> videos;
        nlohmann::json playlistItemsJson;
        nlohmann::json videoListJson;

        // Instantiate API Handler
        std::shared_ptr<sane::APIHandler> api = std::make_shared<sane::APIHandler>();

        // For playlist in t_playlists
        // TODO: This probably needs threading
        int playlistCounter = 1;
        // This print can be anything as long as it's shorter than the progress line print below.
        std::cout << "Retrieving " << t_playlists.size() << " \"uploaded videos\" playlists: X" << std::flush;
        for (const auto& playlist : t_playlists) {
            // Define the current progress as a whole string line
            std::string progressPercentString;
            std::string progressLine = std::to_string(playlistCounter) + "/" + std::to_string(t_playlists.size());
            float progressPercent = (float)playlistCounter / t_playlists.size() * 100;

            if ((size_t)playlistCounter < t_playlists.size()) {
                progressPercentString = std::to_string(progressPercent).substr(0, 4);
            } else {
                // Handle "100." case, where there's no decimals, only the decimal point.
                progressPercentString = std::to_string(progressPercent).substr(0, 3);
            }

            // Return to start of line and overwrite with progressLine (works cos it never shrinks in length)
            std::cout << "\r" << "Retrieving " << "\"Uploaded Videos\" playlists... "
                      << progressPercentString << "% " << "(" << progressLine << ")" << std::flush;

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
            //
            // NB: Note using t_playlistItemsPart (likely only contentDetails), not t_part.
            //
            // Due to the limited amount of info in youtube#playlistItems we only request the part that holds videoId,
            // and then we perform a separate videos.list() API request for those IDs further down the line.
            playlistItemsJson = api->youtubeListPlaylistItems(t_playlistItemsPart, filter, optParams);

            // Make sure the playlistItemsJson response was valid and contains items.
            if (hasItems(playlistItemsJson)) {
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
//                std::cout << "\tRetrieving additional video info... " << std::endl;
                videoListJson = api->youtubeListVideos(t_part, filter, optParams);

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
        std::cout << std::endl;  // Newline after playlist counter is done.

        return videos;
    }

    // Create subs-feed from a list of channel uploaded videos playlists.
    std::list<std::shared_ptr<YoutubeVideo>> createSubscriptionsFeed(const std::list<std::string> &t_playlists,
                                                                     const std::string &t_part,
                                                                     const std::map<std::string, std::string> &t_filter,
                                                                     const std::map<std::string, std::string> &t_optParams) {
        // Video uploads
        std::list<std::shared_ptr<YoutubeVideo>> videos;

        // Get list of uploaded videos for every given channel/playlist.
        videos = listUploadedVideos(t_playlists, t_part, t_filter, t_optParams);

        // Sort by publishedAt date.
//        std::cout << "Sorting subs-feed videos by publishedAt datetime..." << std::endl;
        videos.sort(sortYoutubeVideoDateDescending());

        return videos;
    }

}