#include <iostream>
#include <string>
#include <list>
#include <vector>
#include <thread>
#include <future>
#include <chrono>

#include <entities/common.hpp>
#include <entities/youtube_channel.hpp>
#include <entities/youtube_video.hpp>
#include <api_handler/api_handler.hpp>

#include <youtube/subfeed.hpp>
#include <db_handler/db_youtube_channels.hpp>

namespace sane {
    std::list<std::shared_ptr<YoutubeVideo>> doThreadingLogicThingy(const std::string &t_part,
                                                                    const std::map<std::string, std::string> &t_filter,
                                                                    const std::map<std::string, std::string> &t_optParams,
                                                                    const std::string &t_playlistItemsPart) {
        std::list<std::shared_ptr<YoutubeVideo>> videos;
        nlohmann::json playlistItemsJson;
        nlohmann::json videoListJson;

        std::map<std::string, std::string> filter = t_filter;
//        std::map<std::string, std::string> optParams = t_optParams;

        // Instantiate API Handler
        std::shared_ptr<sane::APIHandler> api = std::make_shared<sane::APIHandler>();

        // Make the SAPI request and retrieve (a rather limited) JSON.
        //
        // NB: Note using t_playlistItemsPart (likely only contentDetails), not t_part.
        //
        // Due to the limited amount of info in youtube#playlistItems we only request the part that holds videoId,
        // and then we perform a separate videos.list() API request for those IDs further down the line.
        playlistItemsJson = api->youtubeListPlaylistItems(t_playlistItemsPart, filter, t_optParams);

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
            videoListJson = api->youtubeListVideos(t_part, filter, t_optParams);

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

        return videos;
    }

    void updateProgressLine(size_t total, int current) {
        // Define the current progress as a whole string line
        std::string progressPercentString;
        std::string progressLine = std::to_string(current) + "/" + std::to_string(total);
        float progressPercent = (float)current / total * 100;

        if ((size_t)current < total) {
            progressPercentString = std::to_string(progressPercent).substr(0, 4);
        } else {
            // Handle "100." case, where there's no decimals, only the decimal point.
            progressPercentString = std::to_string(progressPercent).substr(0, 3);
        }

        // Return to start of line and overwrite with progressLine (works cos it never shrinks in length)
        std::cout << "\r" << "Retrieving " << "\"Uploaded Videos\" playlists... "
                  << progressPercentString << "% " << "(" << progressLine << ")" << std::flush;
    }

    std::list<std::shared_ptr<YoutubeVideo>> listUploadedVideos(const std::list<std::string> &t_playlists,
                                                                const std::string &t_part,
                                                                const std::map<std::string, std::string> &t_filter,
                                                                const std::map<std::string, std::string> &t_optParams,
                                                                const std::string &t_playlistItemsPart) {
        using std::chrono_literals::operator""s;
        using std::chrono_literals::operator""ms;

        int threadLimit = 10;
        std::map<std::thread::id, std::thread> threads;
        std::map<std::thread::id, std::future<std::list<std::shared_ptr<YoutubeVideo>>>> threadFutures;
        std::list<std::shared_ptr<YoutubeVideo>> videos;
        nlohmann::json playlistItemsJson;
        nlohmann::json videoListJson;

        // For playlist in t_playlists
        int playlistCounter = 1;
        // This print can be anything as long as it's shorter than the progress line print below.
        std::cout << "Retrieving " << t_playlists.size() << " \"uploaded videos\" playlists: X" << std::flush;
        int activeThreads = 0;
        std::map<std::thread::id, int> exceptionThreads = std::map<std::thread::id, int>();
        for (const auto& playlist : t_playlists) {
            // Add passed filters and optional parameters.
            std::map<std::string,std::string> filter = t_filter;
//            const std::map<std::string,std::string>& optParams = t_optParams;

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

            bool startedThreadForThisPlaylist = false;

            while (!startedThreadForThisPlaylist) {
                // Wait for active threads to go below limit.
                while(activeThreads >= threadLimit) {
                    // Iterate over api call job threads until enough are joined.
                    for (auto &threadFuture : threadFutures) { // first = id, second = std::future instance
                        std::list<std::thread::id> threadsToClean = std::list<std::thread::id>();

                        std::thread::id id = threadFuture.first;

                        // Get thread status using wait_for.
                        auto status = threadFuture.second.wait_for(0ms);

                        // If thread is finished:
                        if (status == std::future_status::ready) {
                            // Join thread
                            try {
                            threads[id].join();
                            std::cout << "Joined thread: " << id << std::endl;

                            // Append videos //FIXME: videos aren't appended
    //                            std::list<std::shared_ptr<YoutubeVideo>> threadVideos = threadFuture.second.get();
    //                            videos.emplace_back(threadVideos);

                            // Update progress info.
//                            updateProgressLine(t_playlists.size(), playlistCounter++);

                            // Add id to list of threads to be cleaned.
                            threadsToClean.push_back(id);

                            // Decrement active threads counter.
                            --activeThreads;
                            } catch (std::exception &exc) {
                                std::cerr << "Exception occurred while joining thread ("
                                          << (threads.find(id) != threads.end() ? "exists" : "!exists") << ") "
                                          << id << ": " << std::string(exc.what())  << "\n" << std::endl;
                                if (exceptionThreads.find(id) != exceptionThreads.end()) {
                                    exceptionThreads[id]++;
                                } else {
                                    exceptionThreads[id] = 0;
                                }
                                std::cerr << "Bad threads: " << std::endl;
                                std::cerr << "\tScore\tID " << std::endl;
                                for (const auto &baddie : exceptionThreads) {
                                    std::cerr << "\t" << baddie.second << "\t" << baddie.first << std::endl;
                                }
                            }
                        }

//                        for (const auto &finishedThread : threadsToClean) {
//                            // Pop thread and future instances from their respective containers.
//                            std::cout << "Erasing thread: " << id << std::endl;
//                            threadFutures.erase(finishedThread);
//                            threads.erase(finishedThread);
//                            std::cout << "Erased thread: " << id << std::endl;
//                        }
                    } // for threadFuture in threadFutures
                } // while activeThreads > threadLimit

                // Spawn a new thread unless limit is reached
                if (activeThreads < threadLimit) {
                    // Spawn thread:
                    // - Create a packaged_task using some task and get its future.
                    std::packaged_task<std::list<std::shared_ptr<YoutubeVideo>>(
                            const std::string,
                            const std::map<std::string, std::string>,
                            const std::map<std::string, std::string>,
                            const std::string)> task { &doThreadingLogicThingy };

                    // Get the std::future instance.
                    auto future = task.get_future();

                    // Run task on new thread.
                    std::thread thread { std::move(task), t_part, filter, t_optParams, t_playlistItemsPart };

                    std::thread::id threadId = thread.get_id();
                    std::cout << "Started thread: " << threadId << " [" << playlist << "]" << std::endl;
                    // thread/future is not copyable, and needs to be moved.
                    threadFutures[threadId] = std::move(future);
                    threads[threadId] = std::move(thread);

                    // Increment active threads counter.
                    ++activeThreads;

                    // Signal that a thread has been added for this particular playlist.
                    startedThreadForThisPlaylist = true;
                } // if (activeThreads < threadLimit)
            } // while (!startedThreadForThisPlaylist)
        } // for playlist in t_playlists

        std::cout << "Out of loop" << std::endl;

        // Wait for all remaining threads to finish.
        while(!threads.empty()) {
            // Iterate over api call job threads until enough are joined.
            for (auto &threadFuture : threadFutures) { // first = id, second = std::future instance
                std::list<std::thread::id> threadsToClean = std::list<std::thread::id>();

                std::thread::id id = threadFuture.first;

                // Get thread status using wait_for.
                auto status = threadFuture.second.wait_for(0ms);

                // If thread is finished:
                if (status == std::future_status::ready) {
                    // Join thread
                    threads[id].join();

                    // Append videos
//                    videos.emplace_back(threadFuture.second.get());  //FIXME: videos aren't appended

                    // Update progress info.
                    playlistCounter++;
                    updateProgressLine(t_playlists.size(), playlistCounter);

                    // Add id to list of threads to be cleaned.
                    threadsToClean.push_back(id);

                    // Decrement active threads counter.
                    --activeThreads;
                }

                for (const auto &finishedThread : threadsToClean) {
                    // Pop thread and future instances from their respective containers.
                    threadFutures.erase(finishedThread);
                    threads.erase(finishedThread);
                }
            } // for threadFuture in threadFutures
        } // while activeThreads > threadLimit


        std::cout << std::endl;  // Newline after playlist counter is done.

        return videos;
    }

    /**
     * Create subs-feed from a list of channel uploaded videos playlists.
     *
     * @param t_playlists
     * @param t_part
     * @param t_filter
     * @param t_optParams
     * @return
     */
    std::list<std::shared_ptr<YoutubeVideo>> createSubscriptionsFeed(const std::string &t_part,
            const std::map<std::string, std::string> &t_filter,
            const std::map<std::string, std::string> &t_optParams) {
        // Get subscriptions from DB.
        std::list<std::string> errors;
//        std::cout << "Retrieving subscriptions from DB..." << std::endl;
        std::list<std::shared_ptr<YoutubeChannel>> channels = getChannelsFromDB(&errors);

        // FIXME: Debug speedup, limit subs count:
        if (false) {
            auto end = std::next(channels.begin(), std::min((size_t)10, channels.size()));
            std::list <std::shared_ptr<YoutubeChannel>> limitedChannels( channels.begin(), end);
            channels = limitedChannels;
        }

        // Retrieve uploaded videos playlist IDs.
//        std::cout << "Retrieving \"uploaded videos\" playlists..." << std::endl;
        std::list<std::string> playlists;
        for (const auto &channel : channels) {
            // Get the uploads playlist.
            playlists.push_back(channel->getUploadsPlaylist());
        }

        // Video uploads
        std::list<std::shared_ptr<YoutubeVideo>> videos;

        // Get list of uploaded videos for every given channel/playlist.
        videos = listUploadedVideos(playlists, t_part, t_filter, t_optParams);

        // Sort by publishedAt date.
//        std::cout << "Sorting subs-feed videos by publishedAt datetime..." << std::endl;
        videos.sort(sortYoutubeVideoDateDescending());

        return videos;
    }
}