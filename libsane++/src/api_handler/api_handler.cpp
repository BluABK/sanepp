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
#include <api_handler/api_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>

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
    nlohmann::json APIHandler::getSapiResponse(const std::string& url) {
        CURL *curl;
        std::string readBuffer;
        long responseCode;
        nlohmann::json jsonData = nlohmann::json::object();

        // Start a libcURL easy session and assign the returned handle.
        // NB: Implicitly calls curl_global_init, which is *NOT* thread-safe!
        curl = curl_easy_init();
        if(curl) {
            CURLcode result;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

            // Perform a blocking file transfer
            result = curl_easy_perform(curl);

            if (result == CURLE_OK) {
                // All fine. Proceed as usual.
                curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
                if (responseCode != 200) {
                    std::cerr << "SAPI request failed with error " << responseCode << ": " << readBuffer << "\n"
                              << std::endl;
                }
            } else {
                std::cerr << "cURL easy perform failed with non-zero code: " << result << "!" << std::endl;
                return jsonData;
            }

            // Cleanup call REQUIRED by curl_easy_init, which closes the handle.
            //
            // This might close all connections this handle has used and possibly has kept open until
            // now - unless it was attached to a multi handle while doing the transfers.
            // Don't call this function if you intend to transfer more files,
            // re-using handles is a key to good performance with libcurl
            curl_easy_cleanup(curl);

            // Convert readBuffer to JSON
            if (responseCode == 200) {
                try {
                    jsonData = nlohmann::json::parse(readBuffer);
                } catch (nlohmann::detail::parse_error &exc) {
                    std::cerr << "Skipping APIHandler::getSapiResponse due to Exception: " << std::string(exc.what())
                              << jsonData.dump() << std::endl;
                } catch (const std::exception &exc) {
                    std::cerr << "Skipping APIHandler::getSapiResponse due to Unexpected Exception: "
                              << std::string(exc.what()) << jsonData.dump() << "\n" << std::endl;
                }
            }
        }
        return jsonData;
    }
} // namespace sane.
