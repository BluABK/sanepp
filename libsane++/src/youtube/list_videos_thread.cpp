#include <youtube/list_videos_thread.hpp>
#include <entities/youtube_video.hpp>
#include <api_handler/api_handler.hpp>
#include <youtube/toolkit.hpp>
#include <config_handler/config_handler.hpp>
#include <log_handler/log_handler.hpp>

namespace sane {
    ListVideosThread::ListVideosThread(const std::string &t_part, const std::map<std::string, std::string> &t_filter,
                                       const std::map<std::string, std::string> &t_optParams,
                                       const std::string &t_playlistItemsPart) {
        m_part = t_part;
        m_filter = t_filter;
        m_optParams = t_optParams;
        m_playlistItemsPart = t_playlistItemsPart;
    }

    void ListVideosThread::listVideos() {
        started = true;

        nlohmann::json playlistItemsJson;
        nlohmann::json videoListJson;

        // Set up logger
        std::shared_ptr<sane::LogHandler> logHandler = std::make_shared<sane::LogHandler>();
        log = logHandler->createLogger("ListVideosThread");

        // Instantiate API Handler
        std::shared_ptr<sane::APIHandler> api = std::make_shared<sane::APIHandler>();
        std::shared_ptr<ConfigHandler> cfg = std::make_shared<ConfigHandler>();

        // Make the SAPI request and retrieve (a rather limited) JSON.
        //
        // NB: Note using t_playlistItemsPart (likely only contentDetails), not t_part.
        //
        // Due to the limited amount of info in youtube#playlistItems we only request the part that holds videoId,
        // and then we perform a separate videos.list() API request for those IDs further down the line.
        try {
            // Check config for instructions to retry failed requests, if any.
            unsigned int retryAttempts = 0;
            if (cfg->hasSection("youtube_requests/retries")) {
                if (cfg->isUnsignedNumber("youtube_requests/retries")) {
                    retryAttempts = cfg->getUnsignedInt("youtube_requests/retries");
                }
            }

            // Amount of attempts (1 initial run + retries).
            unsigned int attempts = 1 + retryAttempts;

            // Perform the request once and retry the given amount of times upon failure.
            for (unsigned int i = 0; i <= attempts; i++) {
                if (i > 0) {
                    // If i > 0 then the loop is past its first iteration which means it's a retry.
                    log->warn("listVideos: Retrying failed request (attempt: "
                            + std::to_string(i) + "/" + std::to_string(attempts) + ") for playlistId: "
                            + m_filter["playlistId"] + ".");
                }

                // Perform the YouTube request.
                playlistItemsJson = api->youtubeListPlaylistItems(m_playlistItemsPart, m_filter, m_optParams);

                // Check if it was successful.
                if (hasItems(playlistItemsJson)) {
                    break;
                } else {
                    log->warn("listVideos: No videos exist in playlistId: " + m_filter["playlistId"] + "!");
                    continue;
                }
            }

            // Make sure the playlistItemsJson response was valid and contains items.
            if (hasItems(playlistItemsJson)) {
                // Do some separate videos.list() API request for current playlist items to actually obtain useful info:

                // Store playlistId for error purposes, before removing it from the filters
                std::string playlistId = "UNSET!";
                if (m_filter.find("playlistId") != m_filter.end()) {
                    playlistId = m_filter["playlistId"];
                }

                // 1. Clear playlistItems-specific filters:
                m_filter.erase("playlistId");

                // 2. Populate filter 'id=' with list of video IDs from the playlistItems response.
                bool firstItem = true;
                for (const auto& playlistItemJson : playlistItemsJson["items"]) {
                    // The "id" field in playlistItem is the item's ID, not the video's.

                    // The actual video ID can be found inside of the parts.
                    if (playlistItemJson.find("contentDetails") != playlistItemJson.end()) {
                        if (firstItem) {
                            // Don't prepend comma to first item.
                            m_filter["id"] = playlistItemJson["contentDetails"]["videoId"].get<std::string>();

                            firstItem = false;
                        } else {
                            // Append id to string, prepended with comma.
                            m_filter["id"] += "," + playlistItemJson["contentDetails"]["videoId"].get<std::string>();
                        }
                    } else {
                        // Kind is playlistItem, but no snippet or contentDetails were provided.
                        std::cerr << "listUploadedVideos Error: Unable to set video ID: Kind is youtube#playlistItem, "
                                  << "but contentDetails parts was not available!" << std::endl;
                    } // if contentDetails in playlistItemJson
                } // for playlistItemJson in current playlistItemsJson

                // 3. Request proper information for the current video IDs using the API's videos.list().
                log->debug("Retrieving additional video info for videos: " + m_filter["id"]);
                try {
                    if (m_filter.find("id") != m_filter.end()) {
                        videoListJson = api->youtubeListVideos(m_part, m_filter, m_optParams);
                    } else {
                        std::string logString;
                        // Error: id not found in filter, report it.
                        logString += "youtubeListVideos: Missing id field in playlist '"
                                  + playlistId + "': " + "m_part = " + m_part + ", " + "m_filter { ";
                        for (auto const& m : m_filter) {
                            logString += m.first + ": " + m.second + ", ";
                        }
                        logString += "}, m_optParams { ";
                        for (auto const& m : m_optParams) {
                            logString += m.first + ": " + m.second + ", ";
                        }
                        logString += "}.";

                        log->error(logString);
                    }

                    // Make sure the videoListJson response was valid.
                    if (!videoListJson.empty()) {
                        // FIXME: No pagination support, will cutoff at 50 max.
                        videosJson = videoListJson["items"];
                    } // if videoListJson not empty
                } catch (std::exception &exc) {
                log->critical("Exception occurred while videoListJson thread " + getThreadIdString() + ": "
                            + std::string(exc.what()));
                } // try/catch: videoListJson

            } else {
                log->warn("No videos exist in playlistId: " + m_filter["playlistId"] + "!");
            } // if/else (playlistItemsJson not empty)
        } catch (std::exception &exc) {
            log->critical("Exception occurred while playlistItemsJson thread " + getThreadIdString() + ": "
                        + std::string(exc.what()));
        } // try/catch: playlistItemsJson
        // Thread info
        finished = true;
    }

    void ListVideosThread::run() {
        if (started) {
            log->error("ListVideosThread is ALREADY RUNNING for playlist: " + m_filter["playlistId"]);
        } else {
            listVideos();
        }
    }

    nlohmann::json ListVideosThread::get() {
        return videosJson;
    }

    std::string ListVideosThread::getPlaylist() {
        return m_filter["playlistId"];
    }

    void ListVideosThread::setThreadId(std::thread::id id) {
        m_threadId = id;
    }

    std::thread::id ListVideosThread::getThreadId() {
        return m_threadId;
    }

    std::string ListVideosThread::getThreadIdString() {
        std::stringstream ss;
        ss << getThreadId();
        std::string idString = ss.str();

        return idString;
    }
} // namespace sane