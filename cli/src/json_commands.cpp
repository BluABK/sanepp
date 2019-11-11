#include <algorithm>
#include <string>

#include "cli.hpp"
#include <youtube/toolkit.hpp>

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
        // snippet is required because 'id' part is the ID of the subscription, not the channel.
        const std::string part = "snippet";
        std::map<std::string, std::string> filter;
        std::map<std::string, std::string> optParams;
        nlohmann::json subsJson;

        filter["mine"] = "true";
        optParams["maxResults"] = "50";

        do {
            subsJson = api->youtubeListSubscriptions(part, filter, optParams);
            std::cout << subsJson.dump(jsonIndent) << std::endl;
            optParams["pageToken"] = getNextPageToken(subsJson);
        } while (hasNextPage(subsJson));
    }

    void CLI::printChannelJsonFromApiByName(const std::string &t_input, int jsonIndent) {
        const std::string part = "snippet";
        std::map<std::string, std::string> filter;
        std::map<std::string, std::string> optParams;

        filter["forUsername"] = t_input;
        optParams["maxResults"] = "1";
        nlohmann::json channelListJson = api->youtubeListChannels(part, filter, optParams);

        if (hasItems(channelListJson)) {
            std::cout << channelListJson["items"][0].dump(jsonIndent) << std::endl;
        } else {
            std::cerr << "CLI::printChannelJsonFromApiByName Error: Result had no items:\n" << channelListJson.dump(4)
                      << std::endl;
        }
    }

    void CLI::printChannelJsonFromApiByName(const std::vector<std::string> &t_input, int jsonIndent) {
        if (t_input.empty()) {
            std::cout << "Error: no arguments given, required: 1." << std::endl;
        } else {
            printChannelJsonFromApiByName(t_input.front(), jsonIndent);
        }
    }

    void CLI::printChannelJsonFromApiById(const std::string &t_input, int jsonIndent) {
        const std::string part = "snippet";
        std::map<std::string, std::string> filter;
        std::map<std::string, std::string> optParams;

        filter["id"] = t_input;
        optParams["maxResults"] = "1";
        nlohmann::json channelListJson = api->youtubeListChannels(part, filter, optParams);

        if (hasItems(channelListJson)) {
            std::cout << channelListJson["items"][0].dump(jsonIndent) << std::endl;
        } else {
            std::cerr << "CLI::printChannelJsonFromApiById Error: Result had no items:\n" << channelListJson.dump(4)
                      << std::endl;
        }
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
            jsonData = api->youtubeListActivities(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListActivities(part, filter, optParams);
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
            jsonData = api->youtubeListCaptions(part, videoId);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListCaptions(part, videoId, optParams);
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
            jsonData = api->youtubeListChannels(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListChannels(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,contentDetails", "id=chan1:chan2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/channelSections/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listChannelSectionsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
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
            jsonData = api->youtubeListChannelSections(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListChannelSections(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,id", "id=comment1:comment2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/comments/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listCommentsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
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
            jsonData = api->youtubeListComments(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListComments(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,id", "id=commentThread1:commentThread2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/commentThreads/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listCommentThreadsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
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
            jsonData = api->youtubeListCommentThreads(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListCommentThreads(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,id", "id=commentThread1:commentThread2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/guideCategories/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listGuideCategoriesJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
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
            jsonData = api->youtubeListGuideCategories(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListGuideCategories(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part <optional params>.
     *
     * Example: args = ["snippet", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/i18nLanguages/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listi18nLanguagesJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
        std::string part;
        std::map<std::string,std::string> optParams;
        nlohmann::json jsonData;

        if (t_input.empty() or t_input.size() > 2) {
            std::cout << "Error: wrong amount of arguments given, required: <= 2." << std::endl;
            return;
        }

        part = t_input.at(0);

        if (t_input.size() == 1) {
            jsonData = api->youtubeListI18nLanguages(part);
        } else if (t_input.size() == 2) {
            optParams = stringToMap(t_input.at(1));
            jsonData = api->youtubeListI18nLanguages(part, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part <optional params>.
     *
     * Example: args = ["snippet", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/i18nRegions/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listi18nRegionsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
        std::string part;
        std::map<std::string,std::string> optParams;
        nlohmann::json jsonData;

        if (t_input.empty() or t_input.size() > 2) {
            std::cout << "Error: wrong amount of arguments given, required: <= 2." << std::endl;
            return;
        }

        part = t_input.at(0);

        if (t_input.size() == 1) {
            jsonData = api->youtubeListI18nRegions(part);
        } else if (t_input.size() == 2) {
            optParams = stringToMap(t_input.at(1));
            jsonData = api->youtubeListI18nRegions(part, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,id", "id=commentThread1:commentThread2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/playlistItems/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listPlaylistItemsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
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
            jsonData = api->youtubeListPlaylistItems(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListPlaylistItems(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,id", "id=commentThread1:commentThread2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/playlists/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listPlaylistsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
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
            jsonData = api->youtubeListPlaylists(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListPlaylists(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: <optional params>.
     *
     * Example: args = ["maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/search/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listSearchJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
        std::map<std::string,std::string> optParams;
        nlohmann::json jsonData;

        if (t_input.size() > 1) {
            std::cout << "Error: wrong amount of arguments given, required: >= 1." << std::endl;
            return;
        }

        if (t_input.empty()) {
            jsonData = api->youtubeSearch();
        } else if (t_input.size() == 1) {
            optParams = stringToMap(t_input.at(0));
            jsonData = api->youtubeSearch(optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: filter <optional params>.
     *
     * Example: args = ["id=commentThread1:commentThread2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/search/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listFilteredSearchJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
        std::map<std::string,std::string> filter;
        std::map<std::string,std::string> optParams;
        nlohmann::json jsonData;

        if (t_input.empty() or t_input.size() > 2) {
            std::cout << "Error: wrong amount of arguments given, required: >= 1." << std::endl;
            return;
        }

        filter = stringToMap(t_input.at(0));

        if (t_input.size() == 1) {
            jsonData = api->youtubeSearchFiltered(filter);
        } else if (t_input.size() == 2) {
            optParams = stringToMap(t_input.at(1));
            jsonData = api->youtubeSearchFiltered(filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,id", "id=commentThread1:commentThread2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/subscriptions/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listSubscriptionsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
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
            jsonData = api->youtubeListSubscriptions(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListSubscriptions(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes args: part, t_requestBody.
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/subscriptions/insert
     *
     * @param t_part
     * @param t_requestBody
     * @param jsonIndent
     */
    void CLI::insertSubscriptionsJsonToApi(const std::string &t_part, nlohmann::json &t_requestBody, int jsonIndent) {
        nlohmann::json body;
        nlohmann::json jsonData;

        log->debug("CLI::insertSubscriptionsJsonToApi(const std::string &t_part="
                 + t_part + ", nlohmann::json &t_requestBody="
                 + t_requestBody.dump() + ", int jsonIndent=" + std::to_string(jsonIndent) + ")");

        jsonData = api->youtubeInsertSubscription(t_part, t_requestBody);

        std::string channelTitle = "MISSING_CHANNEL_TITLE";
        std::string channelId = "MISSING_CHANNEL_ID";

        if (jsonData.find("snippet") != jsonData.end()) {
            nlohmann::json snippet_ = jsonData["snippet"];

            if (snippet_.find("title") != snippet_.end()) {
                channelTitle = snippet_["title"].get<std::string>();
            } else {
                log->error(R"(insertSubscriptionsJsonToApi: jsonData["snippet"]["title"] object not found!)");
                log->error(jsonData.dump(jsonIndent));
            }

            if (snippet_.find("channelId") != snippet_.end()) {
                channelId = snippet_["channelId"].get<std::string>();
            } else {
                log->error(R"(insertSubscriptionsJsonToApi: jsonData["snippet"]["channelId"] object not found!)");
                log->error(jsonData.dump(jsonIndent));
            }
        } else {
            log->error(R"(insertSubscriptionsJsonToApi: jsonData["snippet"] object not found!)");
            log->error(jsonData.dump(jsonIndent));
        }

        // Print the result
        if (jsonData.find("kind") != jsonData.end()) {
            if (jsonData["kind"].get<std::string>() == "youtube#subscription") {
                std::cout << "Successfully subscribed to: " << channelTitle << " (" << channelId << ")." << std::endl;
            } else {
                log->error(R"(insertSubscriptionsJsonToApi: jsonData["kind"] is not a subscription resource!)");
                log->error(jsonData.dump(jsonIndent));
                std::cerr << "Subscription to channel failed! (see log)" << std::endl;
            }
        } else {
            log->error(R"(insertSubscriptionsJsonToApi: jsonData["kind"] object not found!)");
            log->error(jsonData.dump(jsonIndent));
            std::cerr << "Subscription to channel failed! (see log)" << std::endl;
        }
    }

    /**
     * Takes tokenized arg: channelId.
     *
     * Example: args = ["snippet,id", "id=commentThread1:commentThread2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/subscriptions/insert
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::insertSubscriptionByChannelId(const std::vector<std::string> &t_input, int jsonIndent) {
        const std::string part = "id,snippet";
        const std::string channelId = t_input.at(0);
        nlohmann::json requestBody;
        nlohmann::json jsonData;

        if (t_input.empty() or t_input.size() < 1) {
            std::cout << "Error: wrong amount of arguments given, required: >= 1." << std::endl;
            return;
        }

        // Generate bare minimum request body for subscription insert (id and type):
        requestBody["snippet"] = { { "resourceId", { {"channelId", channelId}, {"kind", "youtube#channel"} } } };

        if (t_input.size() == 1) {
            insertSubscriptionsJsonToApi(part, requestBody, jsonIndent);
        }
    }

    /**
     * Takes tokenized args: part <optional params>.
     *
     * Example: args = ["snippet", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/videoAbuseReportReasons/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listVideoAbuseReportReasonsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
        std::string part;
        std::map<std::string,std::string> optParams;
        nlohmann::json jsonData;

        if (t_input.empty() or t_input.size() > 2) {
            std::cout << "Error: wrong amount of arguments given, required: <= 2." << std::endl;
            return;
        }

        part = t_input.at(0);

        if (t_input.size() == 1) {
            jsonData = api->youtubeListVideoAbuseReportReasons(part);
        } else if (t_input.size() == 2) {
            optParams = stringToMap(t_input.at(1));
            jsonData = api->youtubeListVideoAbuseReportReasons(part, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,id", "id=commentThread1:commentThread2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/videoCategories/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::listVideoCategoriesJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
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
            jsonData = api->youtubeListVideoCategories(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListVideoCategories(part, filter, optParams);
        }

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    /**
     * Takes tokenized args: part filter <optional params>.
     *
     * Example: args = ["snippet,id", "id=commentThread1:commentThread2", "maxResults=15"]
     *
     * For more info see: https://developers.google.com/youtube/v3/docs/videos/list
     *
     * @param t_input
     * @param jsonIndent
     */
    void CLI::printVideosJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent) {
        nlohmann::json jsonData = listVideosJsonFromApi(t_input);

        // Print the result
        std::cout << jsonData.dump(jsonIndent) << std::endl;
    }

    nlohmann::json CLI::listVideosJsonFromApi(const std::vector<std::string> &t_input) {
        std::string part;
        std::map<std::string,std::string> filter;
        std::map<std::string,std::string> optParams;
        nlohmann::json jsonData;

        if (t_input.empty() or t_input.size() < 2) {
            std::cout << "Error: wrong amount of arguments given, required: >= 2." << std::endl;
            return nlohmann::json::object();
        }

        part = t_input.at(0);
        filter = stringToMap(t_input.at(1));

        if (t_input.size() == 2) {
            jsonData = api->youtubeListVideos(part, filter);
        } else if (t_input.size() == 3) {
            optParams = stringToMap(t_input.at(2));
            jsonData = api->youtubeListVideos(part, filter, optParams);
        }

        return jsonData;
    }
} // namespace sane