/*
 *  SaneAPI handling via libcURL.
 */

#include <iostream>
#include <string>
#include <curl/curl.h>

using namespace std;

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string sapiTestStaticJson(void)
{
  CURL *curl;
  CURLcode res;
  string readBuffer;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:5002/api/v1/test/static/json");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

	return readBuffer;
  }
  return "sapiTestStaticJson ERROR: !cURL";
}
