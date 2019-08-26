#include <youtube/list_videos_thread.hpp>
#include <entities/youtube_video.hpp>
#include <api_handler/api_handler.hpp>
#include <youtube/toolkit.hpp>

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

        // Instantiate API Handler
        std::shared_ptr<sane::APIHandler> api = std::make_shared<sane::APIHandler>();

        // Make the SAPI request and retrieve (a rather limited) JSON.
        //
        // NB: Note using t_playlistItemsPart (likely only contentDetails), not t_part.
        //
        // Due to the limited amount of info in youtube#playlistItems we only request the part that holds videoId,
        // and then we perform a separate videos.list() API request for those IDs further down the line.
        try {
            playlistItemsJson = api->youtubeListPlaylistItems(m_playlistItemsPart, m_filter, m_optParams);

            // Make sure the playlistItemsJson response was valid and contains items.
            if (hasItems(playlistItemsJson)) {
                // Do some separate videos.list() API request for current playlist items to actually obtain useful info:

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
    //                std::cout << "\tRetrieving additional video info... " << std::endl;
                try {
                    videoListJson = api->youtubeListVideos(m_part, m_filter, m_optParams);

                    // Make sure the videoListJson response was valid.
                    if (!videoListJson.empty()) {
                        // FIXME: No pagination support, will cutoff at 50 max.
                        videosJson = videoListJson["items"];
                    } // if videoListJson not empty
                } catch (std::exception &exc) {
                std::cerr << "Exception occurred while videoListJson thread "
                          << getThreadId() << ": " << std::string(exc.what())  << "\n" << std::endl;
                } // try/catch: videoListJson

            } // if playlistItemsJson not empty
        } catch (std::exception &exc) {
            std::cerr << "Exception occurred while playlistItemsJson thread "
                      << getThreadId() << ": " << std::string(exc.what())  << "\n" << std::endl;
        } // try/catch: playlistItemsJson
        // Thread info
        finished = true;
    }

    void ListVideosThread::run() {
        if (started) {
            std::cerr << "ERROR: ListVideosThread is ALREADY RUNNING for playlist: "
                      << m_filter["playlistId"] << std::endl;
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
} // namespace sane