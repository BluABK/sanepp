// 3rd party libraries.
#include <nlohmann/json.hpp>

#include "youtube_subscriptions.hpp"

using json = nlohmann::json;

YoutubeSubscriptions YoutubeSubscriptions::addFromJson(const json& t_data) {
    // iterate the JSON array of multiple subscriptions.
    int counter = 0;
    for (auto & subscriptionJson : t_data) {
        std::cout << "Sub#" << counter << ":\n\t" << subscriptionJson.dump(4) << '\n';
        counter++;
    }
    return YoutubeSubscriptions();
}
