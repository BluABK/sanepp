#ifndef SANE_ENTITY_RESPONSE_HPP
#define SANE_ENTITY_RESPONSE_HPP

#include <list>

#include <entities/youtube_channel.hpp>

#define CLEAR_PROBLEMS true
#define DONT_CLEAR_PROBLEMS false

namespace sane {
    void printReport(int t_warningsCount, int t_errorsCount);

    void printReport(std::shared_ptr<YoutubeChannel> &t_channel);

    void sapiGetSubscriptions(bool clearProblems = CLEAR_PROBLEMS);

    std::shared_ptr<YoutubeChannel> sapiGetChannelByUsername(const std::string &t_username,
                                                             bool clearProblems = CLEAR_PROBLEMS);

    std::shared_ptr<YoutubeChannel> sapiGetChannelById(const std::string &t_channelId,
                                                       bool clearProblems = CLEAR_PROBLEMS);
} // namespace sane.
#endif //SANE_ENTITY_RESPONSE_HPP