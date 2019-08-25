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
#include <youtube/list_videos_thread.hpp>

namespace sane {
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

        int threadLimit = 30;
        std::list<std::map<std::string, std::string>> filters;
        std::list<std::thread> threads;
        std::list<std::shared_ptr<ListVideosThread>> videoThreadObjects;
        std::map<std::thread::id, std::shared_ptr<ListVideosThread>> idMap;
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

//            // Initialize a ListVideosThread object
//            videoThreadObjects.emplace_back(
//                    std::make_shared<ListVideosThread>(t_part, filter, t_optParams, t_playlistItemsPart));
            std::shared_ptr<ListVideosThread> p(
                    new ListVideosThread(t_part, filter, t_optParams, t_playlistItemsPart));
            videoThreadObjects.emplace_back(p);
        } // for playlist in t_playlists

        // Do threading
        bool threadingDone = false;
//        int threadCountTarget = videoThreadObjects.size();
        while (!threadingDone) {
            // Iterate ListVideoThread objects until an un-started one is found.
            for (auto& videoThreadObject : videoThreadObjects) {
                // Spawn a new thread unless limit is reached
                if (activeThreads < threadLimit) {
                    if (!videoThreadObject->started) {
                        // Spawn thread and run task on the new thread:
                        // - Use a heap-allocated object and a reference-counted pointer
                        //   to ensure that the object stays around as long as the thread does.
                        std::thread thread ( &ListVideosThread::run, videoThreadObject );
                        std::thread::id threadId = thread.get_id();

                        // Associate the thread ID with videoThreadObject.
                        idMap[threadId] = videoThreadObject;

//                        std::cout << "Started thread: " << threadId
//                                  << " [" << videoThreadObject->getPlaylist() << "]" << std::endl;

                        // Append thread to list of threads.
                        threads.push_back(std::move(thread));

                        // Increment active threads counter.
                        ++activeThreads;
                    }
                }
            }

//            std::string threadString;
//            for (auto& thread : threads) {
//                auto id = thread.get_id();
//                std::stringstream ss;
//                ss << id;
//                threadString += " " + ss.str();;
//            }
//
//            std::cout << "Current threads: " << threadString << std::endl;

            // Join threads (if any).
//            for (auto& thread : threads) {
            // Use iterator and while loop in order to delete joined items as we traverse.
            auto threadIter = threads.begin();
            while (threadIter != threads.end()) {
                // Check the ID map If thread is associated with a finished object.
                if (idMap[threadIter->get_id()]->finished) {
                    // Attempt to synchronize the thread.
                    try {
                        std::thread::id id = threadIter->get_id();
                        threadIter->join();
//                        std::cout << "Joined thread: " << id << std::endl;

                        // Append videos //FIXME: videos aren't appended
//                         std::list<std::shared_ptr<YoutubeVideo>> threadVideos = idMap[id]->get();
//                         videos.emplace_back(threadVideos);

                        // Update progress info.
                        updateProgressLine(t_playlists.size(), playlistCounter++);

                        // Decrement active threads counter.
                        --activeThreads;

                        // Cleanup!

                        // Remove associated ListVideoThread object from its list.
                        auto vtoIter = std::find(videoThreadObjects.begin(), videoThreadObjects.end(), idMap[id]);
                        videoThreadObjects.erase(vtoIter);

                        // Clear the idMap entry
                        idMap.erase(id);

                        // Remove thread from threads list (to avoid running into NULL)
                        // erase() makes the passed iterator invalid, but returns the iterator to
                        // the next of deleted element
                        threadIter = threads.erase(threadIter);
                    } catch (std::exception &exc) {
                        std::cerr << "Exception occurred while joining thread "
                                  << threadIter->get_id() << ": " << std::string(exc.what())  << "\n" << std::endl;

                        // Bad thread tracker/scoreboard.
                        if (exceptionThreads.find(threadIter->get_id()) != exceptionThreads.end()) {
                            exceptionThreads[threadIter->get_id()]++;
                        } else {
                            exceptionThreads[threadIter->get_id()] = 0;
                        }

                        std::cerr << "Bad threads: " << std::endl;
                        std::cerr << "\tScore\tID " << std::endl;

                        for (const auto &baddie : exceptionThreads) {
                            std::cerr << "\t" << baddie.second << "\t" << baddie.first << std::endl;
                        }
                    } // try/catch
                }
            }

            // If there's no more ListVideoThread objects, our job here is done.
            if (videoThreadObjects.empty()) {
                threadingDone = true;
            }
//            else {
//                std::cout << "videoThreadObjects: " << videoThreadObjects.size() << std::endl;
//            }
        } // while !threadingDone

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