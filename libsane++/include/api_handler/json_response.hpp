#ifndef SANE_JSON_RESPONSE_HPP
#define SANE_JSON_RESPONSE_HPP
namespace sane {
    /** Remote: Requests to the YouTube API with some extra functionality added on. */

    nlohmann::json sapiRemoteGetSubscriptionsJson();

    nlohmann::json sapiRemoteGetChannelByUsernameJson(const std::string &t_username);

    nlohmann::json sapiRemoteGetChannelByIdJson(const std::string &t_channelId);

    /** YouTube: Pass-through kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/ *
     *  "Youtube" prefix is implicit.
     */

} // namespace sane
#endif //SANE_JSON_RESPONSE_HPP
