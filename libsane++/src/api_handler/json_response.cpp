// Standard libraries.
#include <iostream>
#include <string>
#include <list>

// 3rd party libraries.
#include <nlohmann/json.hpp>

// Project specific libraries.
#include <api_handler/api_handler.hpp>
#include <api_handler/json_response.hpp>

namespace sane {
    nlohmann::json sapiGetSubscriptionsJson() {
        std::cout << "Retrieving subscriptions from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_SUBSCRIPTIONS);
        std::cout << "Got response from SaneAPI: " << jsonData.size() << " channels." << std::endl;

        return jsonData;
    }

    nlohmann::json sapiGetChannelByUsernameJson(const std::string &t_username) {
        std::cout << "Retrieving channel '" << t_username << "' from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_CHANNEL "?username=" + t_username);
        std::cout << "Got response from SaneAPI" << std::endl;

        return jsonData;
    }

    nlohmann::json sapiGetChannelByIdJson(const std::string &t_channelId) {
        std::cout << "Retrieving channel '" << t_channelId << "' from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_CHANNEL "?id=" + t_channelId);
        std::cout << "Got response from SaneAPI, processing channel..." << std::endl;

        return jsonData;
    }
} // namespace sane