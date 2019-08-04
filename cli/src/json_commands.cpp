#include "cli.hpp"

namespace sane {
    void CLI::printSubscriptionsJsonFromApi(int jsonIndent) {
        nlohmann::json subsJson = api->sapiRemoteGetSubscriptionsJson();
        subsJson.dump(jsonIndent);
    }

    void CLI::printChannelJsonFromApiByName(const std::string &t_input, int jsonIndent) {
        nlohmann::json channelJson;
        channelJson = api->sapiRemoteGetChannelJsonByUsername(t_input);
        std::cout << channelJson.dump(jsonIndent) << std::endl;
    }

    void CLI::printChannelJsonFromApiByName(const std::vector<std::string> &t_input, int jsonIndent) {
        if (t_input.empty()) {
            std::cout << "Error: no arguments given, required: 1." << std::endl;
        } else {
            printChannelJsonFromApiByName(t_input.front(), jsonIndent);
        }
    }

    void CLI::printChannelJsonFromApiById(const std::string &t_input, int jsonIndent) {
        nlohmann::json channelJson = api->sapiRemoteGetChannelJsonById(t_input);
        std::cout << channelJson.dump(jsonIndent) << std::endl;
    }

    void CLI::printChannelJsonFromApiById(const std::vector<std::string> &t_input, int jsonIndent) {
        if (t_input.empty()) {
            std::cout << "Error: no arguments given, required: 1." << std::endl;
        } else {
            printChannelJsonFromApiById(t_input.front(), jsonIndent);
        }
    }
} // namespace sane