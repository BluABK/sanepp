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

    nlohmann::json APIHandler::sapiRemoteGetSubscriptionsJson() {
        std::cout << "Retrieving subscriptions from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_SUBSCRIPTIONS);
        std::cout << "Got response from SaneAPI: " << jsonData.size() << " channels." << std::endl;

        return jsonData;
    }

    nlohmann::json APIHandler::sapiRemoteGetChannelJsonByUsername(const std::string &t_username) {
        std::cout << "Retrieving channel '" << t_username << "' from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_CHANNEL "?username=" + t_username);
        std::cout << "Got response from SaneAPI" << std::endl;

        return jsonData;
    }

    nlohmann::json APIHandler::sapiRemoteGetChannelJsonById(const std::string &t_channelId) {
        std::cout << "Retrieving channel '" << t_channelId << "' from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_REMOTE_GET_CHANNEL "?id=" + t_channelId);
        std::cout << "Got response from SaneAPI, processing channel..." << std::endl;

        return jsonData;
    }

    nlohmann::json APIHandler::sapiGetActivitiesList(const std::string &t_part,
            const std::map<std::string,std::string> &t_filters, const std::map<std::string,std::string> &t_optParams) {
        // Setup
        std::list<std::map<std::string, std::string>> varMaps;
        std::string compiledVariables;

        // 'part' is a required first part of a YouTube API HTTP string.
        compiledVariables += "?part=" + t_part;

        // Append filters and optional parameters.
        varMaps.push_back(t_filters);
        varMaps.push_back(t_optParams);
        compiledVariables += compileUrlVariables(varMaps, IS_NOT_BEGINNING);

        // Parse the JSON response from the API.
        nlohmann::json jsonData = getSapiResponse(SAPI_YT_ACTIVITIES_LIST + compiledVariables);

        return jsonData;
    }
} // namespace sane