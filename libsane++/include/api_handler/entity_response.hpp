#ifndef SANE_ENTITY_RESPONSE_HPP
#define SANE_ENTITY_RESPONSE_HPP

#include <list>

#include <entities/youtube_channel.hpp>

#define CLEAR_PROBLEMS true
#define DONT_CLEAR_PROBLEMS false

namespace sane {
    void printReport(int t_warningsCount, int t_errorsCount);

    void printReport(std::shared_ptr<YoutubeChannel> &t_channel);

    /** Remote: Requests to the YouTube API with some extra functionality added on. */

    void sapiRemoteGetSubscriptions(bool clearProblems = CLEAR_PROBLEMS);

    std::shared_ptr<YoutubeChannel> sapiRemoteGetChannelByUsername(const std::string &t_username,
                                                                   bool clearProblems = CLEAR_PROBLEMS);

    std::shared_ptr<YoutubeChannel> sapiRemoteGetChannelById(const std::string &t_channelId,
                                                             bool clearProblems = CLEAR_PROBLEMS);

    /** YouTube: Pass-through kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/
     * "Youtube" prefix is implicit.
     * */

} // namespace sane.
#endif //SANE_ENTITY_RESPONSE_HPP