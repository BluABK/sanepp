#include <algorithm>
#include <string>

#include "cli.hpp"

namespace sane {
    /**
     * Takes a comma-separated string on the form of:
     *      single assignments: "a=b,c=d,e=f"
     *      and
     *      list assignments: "a=b:t:y,b=t:a:u,c=stuff"
     *
     * and returns a map on the form of map[a] = b etc.
     *
     * @param t_string
     * @return
     */
    std::map<std::string, std::string> CLI::stringToMap(const std::string &t_string) { // FIXME: Handle '=' in value str
        std::map<std::string, std::string> variableAssignmentMap;

        // Split out assignments by tokenizing on comma.
        std::vector<std::string> variableAssignments = tokenize(t_string, ',');

        // For each assignment in variableAssignments
        std::vector<std::string> assignmentTokens;
        std::string variable;
        std::string value;
        for (auto const& assignment: variableAssignments) {
            // Split out var name and value by tokenizing on equals symbol.
            assignmentTokens = tokenize(assignment, '=');
            variable = assignmentTokens.at(0);
            value = assignmentTokens.at(1);

            // Replace custom list separator ':' with a proper ','.
            std::replace(value.begin(), value.end(), ':', ',');

            // Add assignment to map.
            variableAssignmentMap[variable] = value;
        }

        return variableAssignmentMap;
    }

    void CLI::printSubscriptionsJsonFromApi(int jsonIndent) {
        nlohmann::json subsJson = api->sapiRemoteGetSubscriptionsJson();
        std::cout << subsJson.dump(jsonIndent) << std::endl;
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

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,contentDetails", "channelId=abc123", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/activities/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listActivitiesJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
        std::string part;
        std::map<std::string,std::string> filter;
        std::map<std::string,std::string> optParams;
        nlohmann::json jsonData;

        if (t_input.empty() or t_input.size() < 2) {
            std::cout << "Error: wrong amount of arguments given, required: >= 2." << std::endl;
            return;
        }

        part = t_input.at(0);
        filter = stringToMap(t_input.at(1));

        if (t_input.size() == 2) {
            jsonData = api->sapiGetActivitiesList(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->sapiGetActivitiesList(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part videoId <optional params>.
     *
     * Example: args = ["snippet,id", "abc123", "id=eeaagg"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/captions/list
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listCaptionsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
        std::string part;
        std::string videoId;
        std::map<std::string,std::string> optParams;
        nlohmann::json jsonData;

        if (t_input.empty() or t_input.size() < 2) {
            std::cout << "Error: wrong amount of arguments given, required: >= 2." << std::endl;
            return;
        }

        part = t_input.at(0);
        videoId = t_input.at(1);

        if (t_input.size() == 2) {
            jsonData = api->sapiGetCaptionsList(part, videoId);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->sapiGetCaptionsList(part, videoId, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,contentDetails", "id=chan1:chan2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/channels/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listChannelsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
        std::string part;
        std::map<std::string,std::string> filter;
        std::map<std::string,std::string> optParams;
        nlohmann::json jsonData;

        if (t_input.empty() or t_input.size() < 2) {
            std::cout << "Error: wrong amount of arguments given, required: >= 2." << std::endl;
            return;
        }

        part = t_input.at(0);
        filter = stringToMap(t_input.at(1));

        if (t_input.size() == 2) {
            jsonData = api->sapiGetChannelsList(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->sapiGetChannelsList(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }
} // namespace sane