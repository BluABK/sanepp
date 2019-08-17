/*
 *  SaneAPI handling via libcURL.
 */

// Standard libraries.
#include <iostream>
#include <sstream>
#include <fstream>
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
    static size_t writeCallback(void *t_contents, size_t t_size, size_t t_nmemb, void *t_userp) {
        ((std::string*)t_userp)->append((char*)t_contents, t_size * t_nmemb);
        return t_size * t_nmemb;
    }

    nlohmann::json APIHandler::getOAuth2Token(const std::string &t_tokenUri, const std::string &t_refreshToken,
                                              const std::string &t_clientId, const std::string &t_clientSecret) {
        CURL *curl;
        std::string readBuffer;
        long responseCode;
        nlohmann::json accessToken;
        std::string refreshToken = t_refreshToken;
        std::string clientId = t_clientId;
        std::string clientSecret = t_clientSecret;
        std::string tokenUri = t_tokenUri;

        // Use config to set parameters that weren't passed a value.
        // FIXME: Interim config solution: read a JSON config file
        std::ifstream ifs{ "config.json" };
        nlohmann::json config = nlohmann::json::parse(ifs);

        if (refreshToken.empty()) {
            refreshToken = config["youtube_auth"]["oauth2"]["refresh_token"].get<std::string>();
        }
        if (clientId.empty()) {
            clientId = config["youtube_auth"]["oauth2"]["client_id"].get<std::string>();
        }
        if (clientSecret.empty()) {
            clientSecret = config["youtube_auth"]["oauth2"]["client_secret"].get<std::string>();
        }
        if (tokenUri.empty()) {
            tokenUri = "https://accounts.google.com/o/oauth2/token";
        }

        // Start a libcURL easy session and assign the returned handle.
        // NB: Implicitly calls curl_global_init, which is *NOT* thread-safe!
        curl = curl_easy_init();
        if(curl) {
            CURLcode result;

            // Custom headers
            struct curl_slist *chunk = nullptr;
            chunk = curl_slist_append(chunk, "Content-type: application/x-www-form-urlencoded");

            // POST data
            std::string postFields = "refresh_token="   + refreshToken
                                   + "&client_id="      + clientId
                                   + "&client_secret="  + clientSecret
                                   + "&grant_type=refresh_token";

            curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
            curl_easy_setopt(curl, CURLOPT_URL, tokenUri.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postFields.c_str());
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
                return accessToken;
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
                    accessToken = nlohmann::json::parse(readBuffer);
                } catch (nlohmann::detail::parse_error &exc) {
                    std::cerr << "Skipping APIHandler::getOAuth2Token due to Exception: " << std::string(exc.what())
                              << accessToken.dump() << std::endl;
                } catch (const std::exception &exc) {
                    std::cerr << "Skipping APIHandler::getOAuth2Token due to Unexpected Exception: "
                              << std::string(exc.what()) << accessToken.dump() << "\n" << std::endl;
                }
            }
        }
        return accessToken;
    }

    /**
     * Gets a SaneAPI response via cURL.
     *
     * @param url   A const string of the full API route URL.
     * @return      Response parsed as JSON or - if cURL failed - an explicitly expressed empty object.
     */
    nlohmann::json APIHandler::getOAuth2Response(const std::string &url) {
        nlohmann::json jsonData = nlohmann::json::object();


        // Get OAuth2 access token.
        std::string accessToken;
        nlohmann::json accessTokenJson = getOAuth2Token();

        if (accessTokenJson.find("access_token") != accessTokenJson.end()) {
            if (accessTokenJson["access_token"].is_string()) {
                accessToken = accessTokenJson["access_token"].get<std::string>();
            } else {
                std::cerr << "Invalid access token: not string!\n" << accessTokenJson.dump(4) << std::endl;
                return jsonData;
            }
        } else {
            std::cerr << "Invalid access token: not in JSON!\n" << accessTokenJson.dump(4) << std::endl;
            return jsonData;
        }

        // Proceed with the original cURL request.
        CURL *curl;
        std::string readBuffer;
        long responseCode;

        // Start a libcURL easy session and assign the returned handle.
        // NB: Implicitly calls curl_global_init, which is *NOT* thread-safe!
        curl = curl_easy_init();
        if(curl) {
//            std::cout << url << std::endl;
            CURLcode result;

            // Custom headers
            struct curl_slist *chunk = nullptr;

            std::string auth_header = "Authorization: Bearer " + accessToken;

            chunk = curl_slist_append(chunk, auth_header.c_str());
            chunk = curl_slist_append(chunk, "Content-type: application/json");

            curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
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
                    std::cerr << "Skipping APIHandler::getOAuth2Response due to Exception: " << std::string(exc.what())
                              << jsonData.dump() << std::endl;
                } catch (const std::exception &exc) {
                    std::cerr << "Skipping APIHandler::getOAuth2Response due to Unexpected Exception: "
                              << std::string(exc.what()) << jsonData.dump() << "\n" << std::endl;
                }
            }
        }
        return jsonData;
    }
} // namespace sane.
