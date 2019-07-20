#ifndef SANE_YOUTUBE_SUBSCRIPTIONS_HPP
#define SANE_YOUTUBE_SUBSCRIPTIONS_HPP

// Standard
#include <iostream>
#include <list>

// 3rd party libraries.
#include <nlohmann/json.hpp>

// Project
#include "youtube_subscription.hpp"

using json = nlohmann::json;

class YoutubeSubscriptions {
public:
    YoutubeSubscriptions addFromJson(const json& t_data);

private:
    std::list<YoutubeSubscription> subscriptions;
};


#endif //SANE_YOUTUBE_SUBSCRIPTIONS_HPP
