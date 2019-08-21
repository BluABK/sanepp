/*
 *  SaneAPI handling via libcURL.
 */

// Standard libraries.
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <regex>
#include <thread>
#include <ctime>

// 3rd party libraries.
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <yhirose/httplib.h>


// Project specific libraries.
#include <api_handler/api_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>
#include <config_handler/config_handler.hpp>

namespace sane {
    static httplib::Server oauth2server;

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

    void urlEncode(std::string &t_stringToEncode) {
        t_stringToEncode = std::regex_replace(t_stringToEncode, std::regex("\\/"), "%2F");
        t_stringToEncode = std::regex_replace(t_stringToEncode, std::regex(":"), "%3A");
    }

    void urlDecode(std::string &t_stringToDecode) {
        t_stringToDecode = std::regex_replace(t_stringToDecode, std::regex("%2F"), "/");
        t_stringToDecode = std::regex_replace(t_stringToDecode, std::regex("%3A"), ":");
    }

    /**
     * Basic logger function to log httplib.
     *
     * @param req
     * @param res
     */
    void httpLog(const httplib::Request &req, const httplib::Response &res) {
        std::cout << "\nRequest headers:\n" << std::endl;
        for (const auto& header : req.headers) {
            std::cout << "first: " << header.first << std::endl;
            std::cout << "second: " << header.second << std::endl;
        }
        std::cout << "\nRequest params:\n" << std::endl;
        for (const auto& header : req.params) {
            std::cout << "first: " << header.first << std::endl;
            std::cout << "second: " << header.second << std::endl;
        }
        std::cout << "Request body:\n" << req.body << std::endl;
        std::cout << "------------------------------------------" << std::endl;
        std::cout << "\nResponse body:\n" << res.body << std::endl;
        std::cout << "\nResponse headers:\n" << std::endl;
        for (const auto& header : res.headers) {
            std::cout << "first: " << header.first << std::endl;
            std::cout << "second: " << header.second << std::endl;
        }
    }

    /**
     * Catches a Google OAuth 2.0 code response (used in step 2).
     *
     * This is basically a re-purposed cpp-httplib logger, to keep things simple.
     *
     * @param req   httplib::Request struct.
     * @param res   httplib::Response struct.
     */
    void APIHandler::oauth2CodeResponseCatcher(const httplib::Request &req, const httplib::Response &res) {
        std::string code;
        for (const auto& param : req.params) {
            if (param.first == "code") {
                code = param.second;
            } else if (param.first == "error") {
                std::cerr << "OAuth2 ERROR: " << param.second << std::endl;

                // Stop the static OAuth2 httplib server.
                stopOAuth2Server();
            }
        }

        if (!code.empty()) {
            // Store code to config:
            std::shared_ptr<ConfigHandler> cfg = std::make_shared<ConfigHandler>();

            // 1. Get current config as JSON object.
            nlohmann::json config = cfg->getConfig();

            // 2. Update it with the new value.
            config["youtube_auth"]["oauth2"]["code"] = code;

            // 3. Overwrite the old config.
            cfg->setConfig(config);

            // Stop the static OAuth2 httplib server.
            stopOAuth2Server();
        }
    }

    /**
     * Step 2/3: Run a local loopback httplib server to catch Google's OAuth 2.0 response.
     *
     * @param t_redirectUri
     */
    void APIHandler::runOAuth2Server(const std::string &t_redirectUri) {
        // Break down redirect URI into host and port.
        std::string strippedUri = std::regex_replace(t_redirectUri, std::regex("http:\\/\\/"), "");
        std::vector<std::string> tokens = tokenize(strippedUri, ':');
        std::string host = tokens[0];
        int port = std::stoi(tokens[1]);

        oauth2server.set_logger([](const auto &req, const auto &res) {
            oauth2CodeResponseCatcher(req, res);
        });

        try {
//            std::cout << "Starting OAuth2 listener server on: " << host << ":" << port <<  "." << std::endl;
            oauth2server.listen(host.c_str(), port);
//            std::cout << "Stopped OAuth2 listener server on: " << host << ":" << port <<  "." << std::endl;
        } catch (std::exception &exc) {
            std::cerr << "APIHandler::runOAuth2Server ERROR: Unexpected exception: " << exc.what() << std::endl;
        }
    }

    void APIHandler::stopOAuth2Server() {
        oauth2server.stop();
    }

    /**
     * Step 1/3: Send a request to Google's OAuth 2.0 server.
     *
     * @param t_clientId
     * @param t_scope
     * @param t_redirectUri
     * @param t_state
     * @param t_loginHint
     * @param t_runServer
     * @param t_oauth2Uri
     * @param t_responseType
     * @return
     */
    nlohmann::json APIHandler::generateOAuth2URI(const std::string &t_clientId, const std::string &t_scope,
                                                 const std::string &t_redirectUri, const std::string &t_state,
                                                 const std::string &t_loginHint, bool t_runServer,
                                                 const std::string &t_oauth2Uri, const std::string &t_responseType) {
        // OAuth2
        std::string uri;
        std::string clientId = t_clientId;
        std::string redirectUri = t_redirectUri;
        std::string responseType = t_responseType;
        std::string scope = t_scope;
        std::string state = t_state; // FIXME: Not yet implemented
        std::string loginHint = t_loginHint; // FIXME: Not yet implemented
        std::string oauth2_uri = t_oauth2Uri;
        nlohmann::json refreshToken;

        // Use config to set parameters that weren't passed a value.
        std::shared_ptr<ConfigHandler> cfg = std::make_shared<ConfigHandler>();

        if (clientId.empty() and cfg) {
            clientId = cfg->getString("youtube_auth/oauth2/client_id");

            if (clientId.empty()) {
                std::cerr << "generateOAuth2URI ERROR: required value client_id is missing, unable to generate URI!"
                          << std::endl;
                return {};
            }
        }

        if (redirectUri.empty() and cfg) {
            redirectUri = cfg->getString("youtube_auth/oauth2/redirect_uri");
        }

        if (responseType.empty() and cfg) {
            responseType = cfg->getString("youtube_auth/oauth2/response_type");
        }

        if (scope.empty() and cfg) {
            for (auto& s : cfg->getStringList("youtube_auth/oauth2/scope")) {
                if (!s.empty()) {
                    if (!scope.empty()) {
                        // Add "whitespace" prefix to URLs as long as it's not the first one.
                        scope += "%20";
                    }
                    scope += s;
                }
            }

            // If scope is *still* empty, throw a tantrum.
            if (scope.empty()) {
                std::cerr << "generateOAuth2URI ERROR: required value scope is empty, unable to generate URI!"
                          << std::endl;
                return {};
            }
        }

        // URL-encode URI parameters
        urlEncode(scope);
        urlEncode(redirectUri);

        // Construct the OAuth2 Authentication URI: // FIXME: Add options
        uri = oauth2_uri + "?scope="            + scope
                         + "&response_type="    + responseType
//                         + "&state="            + state
                         + "&redirect_uri="     + redirectUri
                         + "&client_id="         + clientId;

        // Run server
        if (t_runServer) {
            // FIXME: Server code goes here.
            urlDecode(redirectUri);
        }

        // Return constructed OAuth2 Authentication URI.
        return uri;
    }

    /**
     * Step 3/3: Exchange authorization code for refresh and access tokens
     *
     * @param t_code
     * @param t_clientId
     * @param t_clientSecret
     * @param t_redirectUri
     * @return
     */
    nlohmann::json APIHandler::authorizeOAuth2(const std::string &t_code, const std::string &t_clientId,
                                              const std::string &t_clientSecret, const std::string &t_redirectUri,
                                              const std::string &t_tokenUri) {
        CURL *curl;
        std::string readBuffer;
        long responseCode;
        nlohmann::json responseTokens;
        std::string code = t_code;
        std::string clientId = t_clientId;
        std::string clientSecret = t_clientSecret;
        std::string redirectUri = t_redirectUri;
        std::string tokenUri = t_tokenUri;

        // Use config to set parameters that weren't passed a value.
        std::shared_ptr<ConfigHandler> cfg = std::make_shared<ConfigHandler>();

        if (code.empty()) {
            code = cfg->getString("youtube_auth/oauth2/code");

            // If code is *still* empty, throw a tantrum.
            if (code.empty()) {
                std::cerr << "authorizeOAuth2 ERROR: Required value 'code' is empty, unable to authorize!"
                          << std::endl;
                return responseTokens;
            }
        }
        if (clientId.empty()) {
            clientId = cfg->getString("youtube_auth/oauth2/client_id");
        }
        if (clientSecret.empty()) {
            clientSecret = cfg->getString("youtube_auth/oauth2/client_secret");
        }
        if (redirectUri.empty()) {
            clientSecret = cfg->getString("youtube_auth/oauth2/redirect_uri");
        }
        if (tokenUri.empty()) {
            tokenUri = cfg->getString("youtube_auth/oauth2/token_uri");
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
            std::string postFields =   "code="            + code
                                     + "&client_id="      + clientId
                                     + "&client_secret="  + clientSecret
                                     + "&redirect_uri="   + redirectUri
                                     + "&grant_type=authorization_code";

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
                    std::cerr << "authorizeOAuth2 API request failed with error "
                              << responseCode << ": " << readBuffer << "\n" << std::endl;
                    return responseTokens;
                }
            } else {
                std::cerr << "authorizeOAuth2 cURL easy perform failed with non-zero code: "
                          << result << "!" << std::endl;
                return responseTokens;
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
                    responseTokens = nlohmann::json::parse(readBuffer);
                } catch (nlohmann::detail::parse_error &exc) {
                    std::cerr << "Skipping APIHandler::authorizeOAuth2 due to Exception: " << std::string(exc.what())
                              << responseTokens.dump() << std::endl;
                } catch (const std::exception &exc) {
                    std::cerr << "Skipping APIHandler::authorizeOAuth2 due to Unexpected Exception: "
                              << std::string(exc.what()) << responseTokens.dump() << "\n" << std::endl;
                }
            }
        }
        // Store access token and related to config:
        // 1. Get current config as JSON object.
        nlohmann::json config = cfg->getConfig();

        // 2. Update it with the new values.
        if (responseTokens.find("access_token") != responseTokens.end()) {
            config["youtube_auth"]["oauth2"]["access_token"] = responseTokens["access_token"].get<std::string>();
        }
        if (responseTokens.find("expires_in") != responseTokens.end()) {
            // Get token TTL in seconds.
            int expiresInSeconds = responseTokens["expires_in"].get<int>();
            // Get current timestamp in seconds since epoch.
            auto now = std::chrono::system_clock::now();
            time_t timeSinceEpoch = std::chrono::system_clock::to_time_t(now);
            // Get timestamp of when TTL is set to expire.
            int expiresAtTimestamp = (int)timeSinceEpoch + expiresInSeconds;

            config["youtube_auth"]["oauth2"]["expires_at"] = expiresAtTimestamp;
        }
        if (responseTokens.find("refresh_token") != responseTokens.end()) {
            config["youtube_auth"]["oauth2"]["refresh_token"] = responseTokens["refresh_token"].get<std::string>();
        }
        // 3. Overwrite the old config.
        cfg->setConfig(config);

        return responseTokens;
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
                    std::cerr << "API request failed with error " << responseCode << ": " << readBuffer << "\n"
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
     * Gets an OAuth2 YouTube API response via cURL.
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
                    std::cerr << "API request failed with error " << responseCode << ": " << readBuffer << "\n"
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
