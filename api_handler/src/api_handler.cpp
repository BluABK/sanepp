/*
 *  SaneAPI handling via libcURL.
 */

// Standard libraries.
#include <iostream>
#include <string>
#include <list>

// 3rd party libraries.
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Project specific libraries.
#include "api_handler.hpp"
#include <entities/youtube_channel.hpp>
#include <db_youtube_channels.hpp>

const std::string SAPI_URL_GET_SUBSCRIPTIONS = "http://127.0.0.1:5002/api/v1/remote/subscriptions";

namespace sane {
    /**
     * Callback function to be called when receiving the http response from the server.
     *
     * You need to pass a function to handle the response stored in contents.
     * The CURLOPT_WRITEDATA is set the fourth param in the cb function.
     * You can write the response to this buffer and access it at your user context.
     *
     * @param contents
     * @param size
     * @param nmemb
     * @param userp
     * @return
     */
    static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
        ((std::string*)userp)->append((char*)contents, size * nmemb);
        return size * nmemb;
    }
    /**
     * Gets a SaneAPI response via cURL.
     *
     * @param url   A const string of the full API route URL.
     * @return      Response parsed as JSON or - if cURL failed - an explicitly expressed empty object.
     */
    nlohmann::json getSapiResponse(const std::string& url) {
        CURL *curl;
        std::string readBuffer;
        nlohmann::json jsonData = nlohmann::json::object();

        // Start a libcURL easy session and assign the returned handle.
        // NB: Implicitly calls curl_global_init, which is *NOT* thread-safe!
        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            // Perform a blocking file transfer
            curl_easy_perform(curl);

            // Cleanup call REQUIRED by curl_easy_init, which closes the handle.
            //
            // This might close all connections this handle has used and possibly has kept open until
            // now - unless it was attached to a multi handle while doing the transfers.
            // Don't call this function if you intend to transfer more files,
            // re-using handles is a key to good performance with libcurl
            curl_easy_cleanup(curl);

            // Convert readBuffer to json
            jsonData = nlohmann::json::parse(readBuffer);
        }
        return jsonData;
    }

    std::list <std::shared_ptr<YoutubeChannel>> sapiGetSubscriptions() {
        std::list <std::shared_ptr<YoutubeChannel>> channels;

        std::cout << "Retrieving subscriptions from YouTube API..." << std::endl;

        // Parse the JSON response from the API.
        std::cout << "Waiting for SaneAPI response..." << std::endl;
        nlohmann::json jsonData = getSapiResponse(SAPI_URL_GET_SUBSCRIPTIONS);
        std::cout << "Got response from SaneAPI, processing " << jsonData.size() << " channels..." << std::endl;

        // iterate the JSON array of multiple channels and append a YoutubeChannel.
        int warnings = 0;
        int errors = 0;
        for (auto & subscriptionJson : jsonData) {
            // Create a new YoutubeChannel object for each channel.
            std::shared_ptr<YoutubeChannel> channel = std::make_shared<YoutubeChannel>();
    //        // Report warnings (errors are on by default).
    //        channel->enableWarnings(true);
            channel->addFromJson(subscriptionJson);

            if (channel->wasAborted()) {
                // Explicitly delete the broken channel object now instead of waiting for smart ptr deallocation.
                channel.reset();
                std::cerr << "\tERROR: Creation of the following channel was aborted:" << std::endl;
                std::cerr << jsonData.dump(4);
            } else {
                warnings += channel->getWarningCount();
                errors += channel->getErrorCount();

                // Append channels list with the new YoutubeChannel object.
                channels.push_back(channel);
            }
        }

        std::string reportedProblems;
        if (warnings > 0 and errors > 0) {
            reportedProblems = " with " + std::to_string(warnings) + " warnings and "
                    + std::to_string(errors) + " errors";
        }
        else if (warnings > 0) {
            reportedProblems = " with " + std::to_string(warnings) + " warnings";
        }
        else if (errors > 0) {
            reportedProblems = " with " + std::to_string(errors) + " errors";
        }

        // Return the parsed channels.
        std::cout << "Processing completed" << reportedProblems << "." << std::endl;

        // Store to Database
        std::cout << "Storing to database..." << std::endl;
        addChannelsToDB(channels);
        std::cout << "Storing to database successful!" << std::endl;

        return channels;
    }
} // namespace sane.
