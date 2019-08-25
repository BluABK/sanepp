#ifndef SANE_LIST_VIDEOS_THREAD_HPP
#define SANE_LIST_VIDEOS_THREAD_HPP

#include <string>
#include <thread>
#include <map>
#include <entities/youtube_video.hpp>


namespace sane {
    class ListVideosThread {
    public:
        ListVideosThread(const std::string &t_part,
                         const std::map<std::string, std::string> &t_filter,
                         const std::map<std::string, std::string> &t_optParams,
                         const std::string &t_playlistItemsPart);

        void listVideos();

        void run();

        std::list<std::shared_ptr<YoutubeVideo>> get();

        std::string getPlaylist();

        void setThreadId(std::thread::id id);

        std::thread::id getThreadId();

        bool finished = false;
        bool started = false;
        std::list<std::shared_ptr<YoutubeVideo>> videos;
        std::thread thread;
    private:
        std::thread::id m_threadId;
        std::string m_part;
        std::map<std::string, std::string> m_filter;
        std::map<std::string, std::string> m_optParams;
        std::string m_playlistItemsPart;

    };
} // namespace sane



#endif //SANE_LIST_VIDEOS_THREAD_HPP
