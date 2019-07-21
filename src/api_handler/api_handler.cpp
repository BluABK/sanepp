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
#include <sapi_objs/sapi_test_static_json.hpp>
#include "api_handler.hpp"
#include "youtube_subscription.hpp"


using json = nlohmann::json;
using namespace std;


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
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
/**
 * Gets a SaneAPI response via cURL.
 *
 * @param url   A const string of the full API route URL.
 * @return      Response parsed as JSON or - if cURL failed - an explicitly expressed empty object.
 */
json getSapiResponse(const string& url) {
    CURL *curl;
    string readBuffer;
    json jsonData = json::object();

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
        jsonData = json::parse(readBuffer);
    }
    return jsonData;
}

/**
 * SaneAPI mock route to test if client handles a basic JSON response.
 *
 * @return SapiTestStaticJson   With parsed JSON.
 */
SapiTestStaticJson sapiTestStaticJsonResponse() {
	// Instantiate the SapiTestStaticJson class.
	SapiTestStaticJson staticJsonResponse;

	// Parse the JSON response from the API.
    json jsonData = getSapiResponse("http://127.0.0.1:5002/api/v1/test/static/json");
	staticJsonResponse.parse(jsonData);

	// Return the parsed SapiTestStaticJson object.
	return staticJsonResponse;
}

list <shared_ptr<YoutubeSubscription>> sapiGetSubscriptions() {
    list <shared_ptr<YoutubeSubscription>> subscriptions;

    // Parse the JSON response from the API.
    json jsonData = getSapiResponse("http://127.0.0.1:5002/api/v1/remote/subscriptions");

    // iterate the JSON array of multiple subscriptions and append a YoutubeSubscription.
    int counter = 1;  // Humanized counting.
    int warnings = 0;
    int errors = 0;
    for (auto & subscriptionJson : jsonData) {
        // Create a new YoutubeSubscription object for each subscription.
        shared_ptr<YoutubeSubscription> subscription = make_shared<YoutubeSubscription>();
//        // Report warnings (errors are on by default).
//        subscription->enableWarnings(true);

        cout << "Sub#" << counter << ":\n";
        subscription->addFromJson(subscriptionJson);

        if (subscription->wasAborted()) {
            // Explicitly delete the broken subscription object now instead of waiting for smart ptr deallocation.
            subscription.reset();
            cerr << "\tERROR: Creation of the following subscription was aborted:" << endl;
            cerr << jsonData.dump(4);
        } else {
            subscription->print(1);
            warnings += subscription->getWarningCount();
            errors += subscription->getErrorCount();

            subscriptions.push_back(subscription);
        }

        // Append subscriptions list with the new YoutubeSubscription object.
        counter++;
    }

    if (warnings > 0) {
        cerr << warnings << " Warnings." << endl;
    }
    if (errors > 0) {
        cerr << errors << " Errors." << endl;
    }
	// Return the parsed SapiTestStaticJson object.
	return subscriptions;
}
