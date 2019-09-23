#ifndef SANE_LIST_VIDEOS_THREAD_HPP
#define SANE_LIST_VIDEOS_THREAD_HPP

#include <string>
#include <thread>
#include <map>
#include <entities/youtube_video.hpp>
#include <spdlog/logger.h>


namespace sane {
    class ListVideosThread {
    public:
        ListVideosThread(const std::string &t_part,
                         const std::map<std::string, std::string> &t_filter,
                         const std::map<std::string, std::string> &t_optParams,
                         const std::string &t_playlistItemsPart);

        void listVideos();

        void run();

        nlohmann::json get();

        std::string getPlaylist();

        void setThreadId(std::thread::id id);

        std::thread::id getThreadId();

        std::string getThreadIdString();

        bool finished = false;
        bool started = false;
    private:
        nlohmann::json videosJson;
        std::thread::id m_threadId;
        std::string m_part;
        std::map<std::string, std::string> m_filter;
        std::map<std::string, std::string> m_optParams;
        std::string m_playlistItemsPart;
        std::shared_ptr<spdlog::logger> log;

    };
} // namespace sane



#endif //SANE_LIST_VIDEOS_THREAD_HPP
