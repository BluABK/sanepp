#ifndef SANE_SUBFEED_HPP
#define SANE_SUBFEED_HPP

#include <api_handler/api_handler.hpp>
#include <entities/youtube_video.hpp>
#include <entities/youtube_channel.hpp>
#include <types.hpp>
#include <youtube/toolkit.hpp>

namespace sane {
    struct sortYoutubeVideoDateDescending {
        bool operator ()(const std::shared_ptr<YoutubeVideo> &video1, const std::shared_ptr<YoutubeVideo> &video2) {
            return video1->getPublishedAt().timestampWithMsec > video2->getPublishedAt().timestampWithMsec;
        }
    };

    struct sortYoutubeVideoDateAscending {
        bool operator ()(const std::shared_ptr<YoutubeVideo> &video1, const std::shared_ptr<YoutubeVideo> &video2) {
            return video1->getPublishedAt().timestampWithMsec < video2->getPublishedAt().timestampWithMsec;
        }
    };

    std::list<std::shared_ptr<YoutubeVideo>> listUploadedVideos(std::list<std::string> &t_playlists,
            const std::string &t_part,
            const std::map<std::string, std::string> &t_filter,
            const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>(),
            const std::string &t_playlistItemsPart = "contentDetails");

    // FIXME: list() version, might also need search() if list turns out to be unreliable.
    std::list<std::shared_ptr<YoutubeVideo>> createSubscriptionsFeed(const std::string &t_part,
            const std::map<std::string, std::string> &t_filter,
            const std::map<std::string, std::string> &t_optParams= std::map<std::string, std::string>());
}
#endif //SANE_SUBFEED_HPP

