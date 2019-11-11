// Standard libraries.
#include <iostream>
#include <string>
#include <list>

// 3rd party libraries.
#include <nlohmann/json.hpp>

// Project specific libraries.
#include <api_handler/api_handler.hpp>

namespace sane {
    /**
     * Takes a list of variable maps on the form <varName, value> and compiles them into a HTTP string.
     *
     * The first variable will be '?varName=value' and the following will be '&varName=value...'
     *
     * @param t_variableMaps
     * @return
     */
    std::string APIHandler::compileUrlVariables(const std::list<std::map<std::string, std::string>> &t_variableMaps,
            bool t_isBeginning) {
        std::string compiledString;
        std::string separator;
        if (t_isBeginning) {
            separator = "?";
        } else{
            separator = "&";
        }

        // For each map in list
        for (auto const& varMap : t_variableMaps) {
            // For each var name and value in map
            for (auto const& var : varMap)
            {
                //                ?           varName      =    value
                compiledString += separator + var.first + "=" + var.second;
                // Any run after this one is no longer the first variable so let's set separator = "&".
                separator = "&";
            }
        }

        return compiledString;
    }

    /**
     * Takes a list of variable values and compiles them into a HTTP string.
     *
     * The ?<varName> is implied by the caller, this function only makes a comma-separated list.
     *
     * @param t_variableValues
     * @return
     */
    std::string APIHandler::compileUrlVariables(const std::list<std::string> &t_variableValues) {
        std::string compiledString;
        std::string separator = ",";
        bool firstVariable = true;

        // For each map in list
        for (auto const& value : t_variableValues) {
            if (firstVariable) {
                // This is the first item so we don't prepend a comma here.
                compiledString += value;
                firstVariable = false;
            } else {
                compiledString += separator + value;
            }
        }

        return compiledString;
    }

    nlohmann::json APIHandler::youtubeListActivities(const std::string &t_part,
                                                     const std::map<std::string, std::string> &t_filter,
                                                     const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append required filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_ACTIVITIES + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListCaptions(const std::string &t_part, const std::string &t_videoId,
                                                   const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' and 'videoId' are required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part + "&videoId=" + t_videoId;

        // Append optional parameters.
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_CAPTIONS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListChannels(const std::string &t_part,
                                                   const std::map<std::string, std::string> &t_filter,
                                                   const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_CHANNELS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListChannelSections(const std::string &t_part,
                                                          const std::map<std::string, std::string> &t_filter,
                                                          const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_CHANNEL_SECTIONS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListComments(const std::string &t_part,
                                                   const std::map<std::string, std::string> &t_filter,
                                                   const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_COMMENTS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListCommentThreads(const std::string &t_part,
                                                         const std::map<std::string, std::string> &t_filter,
                                                         const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_COMMENT_THREADS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListGuideCategories(const std::string &t_part,
                                                          const std::map<std::string, std::string> &t_filter,
                                                          const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_GUIDE_CATEGORIES + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListI18nLanguages(const std::string &t_part,
                                                        const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append  optional parameters.
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_I18N_LANGUAGES + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListI18nRegions(const std::string &t_part,
                                                      const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append  optional parameters.
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_I18N_REGIONS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListPlaylistItems(const std::string &t_part,
                                                        const std::map<std::string, std::string> &t_filter,
                                                        const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_PLAYLIST_ITEMS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListPlaylists(const std::string &t_part,
                                                    const std::map<std::string, std::string> &t_filter,
                                                    const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_PLAYLISTS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeSearch(const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part=snippet' is a required STATIC first part of this YouTube API HTTP string.
        compiledVariables += "?part=snippet";

        // Append filter and optional parameters.
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_SEARCH + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeSearchFiltered(const std::map<std::string, std::string> &t_filter,
                                                     const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part=snippet' is a required STATIC first part of this YouTube API HTTP string.
        compiledVariables += "?part=snippet";

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_SEARCH + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListSubscriptions(const std::string &t_part,
                                                        const std::map<std::string, std::string> &t_filter,
                                                        const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_SUBSCRIPTIONS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeInsertSubscription(const std::string &t_part, nlohmann::json &t_requestBody) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Parse the JSON response from the API.
        nlohmann::json jsonData = postOAuth2Response(YOUTUBE_API_SUBSCRIPTIONS + compiledVariables, t_requestBody);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListVideoAbuseReportReasons(const std::string &t_part,
            const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append optional parameters.
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_VIDEO_ABUSE_REPORT_REASONS + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListVideoCategories(const std::string &t_part,
                                                          const std::map<std::string, std::string> &t_filter,
                                                          const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_VIDEO_CATEGORIES + compiledVariables);

        return jsonData;
    }

    nlohmann::json APIHandler::youtubeListVideos(const std::string &t_part,
                                                 const std::map<std::string, std::string> &t_filter,
                                                 const std::map<std::string, std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filter and optional parameters.
        varMaps.push_back(t_filter);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getOAuth2Response(YOUTUBE_API_VIDEOS + compiledVariables);

        return jsonData;
    }
} // namespace sane