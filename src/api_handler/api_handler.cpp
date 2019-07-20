/*
 *  SaneAPI handling via libcURL.
 */

// Standard libraries.
#include <iostream>
#include <string>

// 3rd party libraries.
#include <curl/curl.h>
#include <nlohmann/json.hpp>

// Project specific libraries.
#include <sapi_objs/sapi_test_static_json.hpp>

using json = nlohmann::json;
using namespace std;

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


SapiTestStaticJson sapiTestStaticJsonResponse(void)
{
	// Instantiate the SapiTestStaticJson class.
	SapiTestStaticJson staticJsonResponse;

	// JSON stuff.
	// Explicitly express the empty object.
	json jsonData = json::object();

	// cURL stuff.
	CURL *curl;
	CURLcode res;
	//json readBuffer = json::object();
	string readBuffer;

	curl = curl_easy_init();
	if(curl) {
		curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5002/api/v1/test/static/json");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);

		// Convert readBuffer to json
		jsonData = json::parse(readBuffer);
	
	}
	// cout << jsonData.dump(4) << endl;
	
	// Parse the JSON response from the API, or an empty json object.
	staticJsonResponse.parse(jsonData);

	// Return the SapiTestStaticJson object.
	return staticJsonResponse;
}
