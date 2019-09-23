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
#include <log_handler/log_handler.hpp>

namespace sane {
    static httplib::Server oauth2server;

    APIHandler::APIHandler() {
        std::shared_ptr<sane::LogHandler> logHandler = std::make_shared<sane::LogHandler>();
        log = logHandler->createLogger("api");
    }

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

    std::string redact(const std::string &t_string) {
        return std::string("[REDACTED] (len: " + std::to_string(t_string.length()) + ").");
    }

    void APIHandler::urlEncode(std::string &t_stringToEncode) {
        const std::string original = t_stringToEncode;

        t_stringToEncode = std::regex_replace(t_stringToEncode, std::regex("\\/"), "%2F");
        t_stringToEncode = std::regex_replace(t_stringToEncode, std::regex(":"), "%3A");

        log->debug("Encoded string to URL: '" + original + "' --> '" + t_stringToEncode + "'.");
    }

    void APIHandler::urlDecode(std::string &t_stringToDecode) {
        const std::string original = t_stringToDecode;

        t_stringToDecode = std::regex_replace(t_stringToDecode, std::regex("%2F"), "/");
        t_stringToDecode = std::regex_replace(t_stringToDecode, std::regex("%3A"), ":");

        log->debug("Decoded URL to string: '" + original + "' --> '" + t_stringToDecode + "'.");
    }

    void APIHandler::updateOAuth2TokenConfig(nlohmann::json &t_response) {
        std::shared_ptr<ConfigHandler> cfg = std::make_shared<ConfigHandler>();

        // 1. Get current config as JSON object.
        nlohmann::json config = cfg->getConfig();

        // 2. Update it with the new values.
        if (t_response.find("access_token") != t_response.end()) {
            log->debug("(config) access_token = " + redact(t_response["access_token"].get<std::string>()));
            config["youtube_auth"]["oauth2"]["access_token"] = t_response["access_token"].get<std::string>();
        }
        if (t_response.find("expires_in") != t_response.end()) {
            // Get token TTL in seconds.
            long int expiresInSeconds = t_response["expires_in"].get<int>();
            // Get current timestamp in seconds since epoch.
            auto now = std::chrono::system_clock::now();
            time_t timeSinceEpoch = std::chrono::system_clock::to_time_t(now);
            // Get timestamp of when TTL is set to expire.
            long int expiresAtTimestamp = (long int)timeSinceEpoch + expiresInSeconds;

            log->debug("(config) expires_at = " + std::to_string(expiresAtTimestamp));
            config["youtube_auth"]["oauth2"]["expires_at"] = expiresAtTimestamp;
        }
        if (t_response.find("refresh_token") != t_response.end()) {
            log->debug("(config) refresh_token = " + redact(t_response["refresh_token"].get<std::string>()));
            config["youtube_auth"]["oauth2"]["refresh_token"] = t_response["refresh_token"].get<std::string>();
        }

        // 3. Overwrite the old config.
        cfg->setConfig(config);
        log->info("Updated OAuth2 token configuration.");
    }

    /**
     * Basic logger function to log httplib.
     *
     * @param req
     * @param res
     */
    void APIHandler::httpLog(const httplib::Request &req, const httplib::Response &res) {
        const std::string logPrefix = "(httpLog) ";

        log->info(logPrefix + " \nRequest headers:\n");
        for (const auto& header : req.headers) {
            log->info(logPrefix + "first: " + header.first);
            log->info(logPrefix + "second: " + header.second);
        }
        log->info(logPrefix + " \nRequest params:\n");
        for (const auto& header : req.params) {
            log->info(logPrefix + "first: " + header.first);
            log->info(logPrefix + "second: " + header.second);
        }
        log->info(logPrefix + "Request body:\n" + req.body);
        log->info(logPrefix + "------------------------------------------");
        log->info(logPrefix + "\nResponse body:\n" + res.body);
        log->info(logPrefix + "\nResponse headers:\n");
        for (const auto& header : res.headers) {
            log->info(logPrefix + "first: " + header.first);
            log->info(logPrefix + "second: " + header.second);
        }
    }

    /**
     * Catches a Google OAuth 2.0 code response (used in step 2).
     *
     * This is basically a re-purposed cpp-httplib logger, to keep things simple.
     *
     * @param req   httplib::Request struct.
     * @param       httplib::Response struct.
     */
    void APIHandler::oauth2CodeResponseCatcher(const httplib::Request &req, const httplib::Response &) {
        std::string code;

        std::shared_ptr<sane::LogHandler> logHandler = std::make_shared<sane::LogHandler>();
        std::shared_ptr<spdlog::logger> staticLog = logHandler->createLogger("api (static)");

        for (const auto& param : req.params) {
            if (param.first == "code") {
                code = param.second;
            } else if (param.first == "error") {
                staticLog->error("OAuth2 ERROR: " + param.second);
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
            staticLog->debug("(config) code = " + redact(code));
            config["youtube_auth"]["oauth2"]["code"] = code;

            // 3. Overwrite the old config.
            cfg->setConfig(config);
            staticLog->info("(config) Updated OAuth2 code entry.");

            // Stop the static OAuth2 httplib server.
            staticLog->info("Stopping OAuth2 httplib server.");
            stopOAuth2Server();
        }
    }

    void APIHandler::stopOAuth2Server() {
        std::shared_ptr<sane::LogHandler> logHandler = std::make_shared<sane::LogHandler>();
        std::shared_ptr<spdlog::logger> staticLog = logHandler->createLogger("api (static)");

        staticLog->info("Stopping OAuth2 httplib server.");
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
    std::string APIHandler::generateOAuth2URI(const std::string &t_clientId, const std::string &t_scope,
                                              const std::string &t_redirectUri, const std::string &t_state,
                                              const std::string &t_loginHint, const std::string &t_oauth2Uri,
                                              const std::string &t_responseType) {
        // OAuth2
        std::string uri;
        std::string clientId = t_clientId;
        std::string redirectUri = t_redirectUri;
        std::string responseType = t_responseType;
        std::string scope = t_scope;
        std::string state = t_state; // FIXME: Not yet implemented
        std::string loginHint = t_loginHint; // FIXME: Not yet implemented
        std::string oauth2_uri = t_oauth2Uri;

        // Use config to set parameters that weren't passed a value.
        std::shared_ptr<ConfigHandler> cfg = std::make_shared<ConfigHandler>();

        if (clientId.empty() and cfg) {
            clientId = cfg->getString("youtube_auth/oauth2/client_id");

            if (clientId.empty()) {
                log->error("generateOAuth2URI ERROR: required value client_id is missing, unable to generate URI!");
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
                log->error("generateOAuth2URI ERROR: required value scope is empty, unable to generate URI!");
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

        // Return constructed OAuth2 Authentication URI.
        return uri;
    }

    /**
     * Step 2/3: Run a local loopback httplib server to catch Google's OAuth 2.0 response.
     *
     * @param t_redirectUri
     */
    void APIHandler::runOAuth2Server(const std::string &t_redirectUri) {
        std::shared_ptr<sane::LogHandler> logHandler = std::make_shared<sane::LogHandler>();
        std::shared_ptr<spdlog::logger> staticLog = logHandler->createLogger("api (static)");

        // Break down redirect URI into host and port.
        std::string strippedUri = std::regex_replace(t_redirectUri, std::regex("http:\\/\\/"), "");
        std::vector<std::string> tokens = tokenize(strippedUri, ':');
        std::string host = tokens[0];
        int port = std::stoi(tokens[1]);

        oauth2server.set_logger([](const auto &req, const auto &res) {
            oauth2CodeResponseCatcher(req, res);
        });

        try {
            staticLog->info("Starting OAuth2 httplib server on " + host + ":" + std::to_string(port) + ".");
            oauth2server.listen(host.c_str(), port);
        } catch (std::exception &exc) {
            staticLog->error("APIHandler::runOAuth2Server ERROR: Unexpected exception: " + std::string(exc.what()));
            std::cerr << "APIHandler::runOAuth2Server ERROR: Unexpected exception: " << exc.what() << std::endl;
        }
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
                log->error("authorizeOAuth2 ERROR: Required value 'code' is empty, unable to authorize!");
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
                    log->error("authorizeOAuth2 API request failed with error "
                              + std::to_string(responseCode) + ": " + readBuffer + "\n");
                    return responseTokens;
                }
            } else {
                log->error("authorizeOAuth2 cURL easy perform failed with non-zero code: " + std::to_string(result)
                         + "!");
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
                    log->error("Skipping APIHandler::authorizeOAuth2 due to Exception: " + std::string(exc.what())
                              + responseTokens.dump());
                } catch (const std::exception &exc) {
                    log->error("Skipping APIHandler::authorizeOAuth2 due to Unexpected Exception: "
                              + std::string(exc.what()) + responseTokens.dump() + "\n");
                }
            }
        }

        // Store access token and related to config.
        log->info("Updating OAuth2 Token config entries (new).");
        updateOAuth2TokenConfig(responseTokens);

        return responseTokens;
    }

    nlohmann::json APIHandler::refreshOAuth2Token(const std::string &t_tokenUri, const std::string &t_refreshToken,
                                                  const std::string &t_clientId, const std::string &t_clientSecret) {
        CURL *curl;
        std::string readBuffer;
        long responseCode;
        nlohmann::json accessTokenJson;
        std::string refreshToken = t_refreshToken;
        std::string clientId = t_clientId;
        std::string clientSecret = t_clientSecret;
        std::string tokenUri = t_tokenUri;

        // Use config to set parameters that weren't passed a value.
        std::shared_ptr<ConfigHandler> cfg = std::make_shared<ConfigHandler>();

        // Mutex lock the refresh action so it doesn't get run multiple times.
        log->debug("refreshOAuth2Token is at mutex lock");
        std::lock_guard<std::mutex> lock(refreshTokenMutex);
        log->debug("refreshOAuth2Token is past mutex lock");

        // Get current timestamp in seconds since epoch.
        auto now = std::chrono::system_clock::now();
        time_t timeSinceEpoch = std::chrono::system_clock::to_time_t(now);

        // Check expiry date of current access token.
        std::string confPath = "youtube_auth/oauth2/expires_at"; // For shortened line length.
        long int expiresAt = cfg->isNumber(confPath) ? cfg->getLongInt(confPath) : -1;

        // Check if token has become valid since function was called (due to mutex lock by a previous caller).
        if (expiresAt > (long int)timeSinceEpoch) {
            log->debug("OAuth2 access token has become valid (refreshed) since "
                       "refreshOAuth2Token was called, returning.");
            return nlohmann::json::object();
        }

        if (refreshToken.empty()) {
            refreshToken = cfg->getString("youtube_auth/oauth2/refresh_token");

            // If refreshToken is *still* empty, throw a tantrum.
            if (refreshToken.empty()) {
                log->error("refreshOAuth2Token ERROR: Required value 'refreshToken' is empty, unable to authorize!\n"
                           "Did you forget to authorize OAuth2?)");
                std::cerr << "refreshOAuth2Token ERROR: Required value 'refreshToken' is empty, unable to authorize!"
                          << std::endl << "(Did you forget to authorize OAuth2?)" << std::endl;
                return accessTokenJson;
            }
        }
        if (clientId.empty()) {
            clientId = cfg->getString("youtube_auth/oauth2/client_id");
        }
        if (clientSecret.empty()) {
            clientSecret = cfg->getString("youtube_auth/oauth2/client_secret");
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
                    log->error("refreshOAuth2Token: API request failed with error " + std::to_string(responseCode)
                             + ": " + readBuffer + "\n");
                }
            } else {
                log->error("refreshOAuth2Token: cURL easy perform failed with non-zero code: " + std::to_string(result)
                          + "!");
                return accessTokenJson;
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
                    accessTokenJson = nlohmann::json::parse(readBuffer);
                } catch (nlohmann::detail::parse_error &exc) {
                    log->error("Skipping APIHandler::refreshOAuth2Token due to Exception: " + std::string(exc.what())
                              + accessTokenJson.dump());
                } catch (const std::exception &exc) {
                    log->error("Skipping APIHandler::refreshOAuth2Token due to Unexpected Exception: "
                              + std::string(exc.what()) + accessTokenJson.dump() + "\n");
                }
            }
        }

        // Store access token and related to config.
        log->info("Updating OAuth2 Token config entries (refresh).");
        updateOAuth2TokenConfig(accessTokenJson);

        return accessTokenJson;
    }

    /**
     * Gets an OAuth2 YouTube API response via cURL.
     *
     * @param t_url   A const string of the full API route URL.
     * @return      Response parsed as JSON or - if cURL failed - an explicitly expressed empty object.
     */
    nlohmann::json APIHandler::getOAuth2Response(const std::string &t_url) {
        std::string accessToken;
        std::string refreshToken;

        nlohmann::json jsonData = nlohmann::json::object();
        std::shared_ptr<ConfigHandler> cfg = std::make_shared<ConfigHandler>();

        // Check config for instructions to retry failed requests, if any.
        unsigned int retryAttempts = 0;
        if (cfg->hasSection("youtube_auth/oauth2/retries")) {
            if (cfg->isUnsignedNumber("youtube_auth/oauth2/retries")) {
                retryAttempts = cfg->getUnsignedInt("youtube_auth/oauth2/retries");
            }
        }

        // Amount of attempts (1 initial run + retries).
        unsigned int attempts = 1 + retryAttempts;

        // Perform the request once and retry the given amount of times upon failure.
        for (unsigned int i = 0; i <= attempts; i++) {
            if (i > 0) {
                // If i > 0 then the loop is past its first iteration which means it's a retry.
                log->warn("getOAuth2Response: Retrying failed request (attempt: "
                        + std::to_string(i) + "/" + std::to_string(attempts) + ") for url: " + t_url + ".");
            }

            // Check that access and/or refresh tokens are valid.
            if (cfg->hasSection("youtube_auth/oauth2/access_token")) {
                if (cfg->isString("youtube_auth/oauth2/access_token")) {
                    accessToken = cfg->getString("youtube_auth/oauth2/access_token");
                }
            }
            if (cfg->hasSection("youtube_auth/oauth2/refresh_token")) {
                if (cfg->isString("youtube_auth/oauth2/refresh_token")) {
                    refreshToken = cfg->getString("youtube_auth/oauth2/refresh_token");
                }
            }

            // If both tokens are missing it is not possible to proceed, log error and abort.
            if (accessToken.empty() and refreshToken.empty()) {
                log->error("getOAuth2Response: Both access and refresh tokens are empty!\n\n"
                           "Did you forget to authenticate OAuth2?");

                return jsonData;
            } else if (refreshToken.empty()) {
                // A missing refresh token may not yet be critical, but could prove troublesome.
                log->warn("getOAuth2Response: refresh token is empty!\n"
                          "This means that once the access token expires you won't be able to renew it.");
            }

            // Get current timestamp in seconds since epoch.
            auto now = std::chrono::system_clock::now();
            time_t timeSinceEpoch = std::chrono::system_clock::to_time_t(now);

            // Check expiry date of current access token.
            std::string confPath = "youtube_auth/oauth2/expires_at"; // For shortened line length.
            long int expiresAt = cfg->isNumber(confPath) ? cfg->getLongInt(confPath) : -1;

            // If access token has expired or has invalid config entry, get a new one.
            if (expiresAt < (long int) timeSinceEpoch) {
                // This will either be the new access token JSON or an empty JSON object (if already valid).
                nlohmann::json accessTokenJson = refreshOAuth2Token();

                // Get the refreshed OAuth2 access token from config instead of above JSON.
                // NB: Due to mutex lock, the above JSON could either be the token or an empty object.
                cfg->getString("youtube_auth/oauth2/access_token");
            }

            // Proceed with the original cURL request.
            CURL *curl;
            std::string readBuffer;
            long responseCode;

            // Start a libcURL easy session and assign the returned handle.
            // NB: Implicitly calls curl_global_init, which is *NOT* thread-safe!
            curl = curl_easy_init();
            if (curl) {
                CURLcode result;

                // Custom headers
                struct curl_slist *chunk = nullptr;

                std::string auth_header = "Authorization: Bearer " + accessToken;

                chunk = curl_slist_append(chunk, auth_header.c_str());
                chunk = curl_slist_append(chunk, "Content-type: application/json");

                curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);
                curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
                curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
                curl_easy_setopt(curl, CURLOPT_URL, t_url.c_str());
                curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
                curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

                // Perform a blocking file transfer
                result = curl_easy_perform(curl);

                if (result == CURLE_OK) {
                    // All fine. Proceed as usual.
                    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
                    if (responseCode != 200) {
                        log->error("getOAuth2Response: API request failed with error " + std::to_string(responseCode) +
                                   ": "
                                   + readBuffer + "\nurl: " + t_url);
                        continue;
                    }
                } else {
                    log->error(
                            "getOAuth2Response: cURL easy perform failed with non-zero code: " + std::to_string(result)
                            + "!");
//                    return jsonData;
                    continue;
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
                        log->error("Skipping APIHandler::getOAuth2Response due to Exception: " + std::string(exc.what())
                                   + jsonData.dump());
                        continue;
                    } catch (const std::exception &exc) {
                        log->error("Skipping APIHandler::getOAuth2Response due to Unexpected Exception: "
                                   + std::string(exc.what()) + jsonData.dump() + "\n");
                        continue;
                    }
                }
            }

            break;
        } // for retryAttempts

        return jsonData;
    }
} // namespace sane.
