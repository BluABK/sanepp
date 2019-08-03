#ifndef SANE_JSON_RESPONSE_HPP
#define SANE_JSON_RESPONSE_HPP
namespace sane {
    nlohmann::json sapiGetSubscriptionsJson();

    nlohmann::json sapiGetChannelByUsernameJson(const std::string &t_username);

    nlohmann::json sapiGetChannelByIdJson(const std::string &t_channelId);
} // namespace sane
#endif //SANE_JSON_RESPONSE_HPP
