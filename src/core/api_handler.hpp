/*
 *  SaneAPI handling via libcURL -- Headers.
 */

#ifndef SANEPP_API_HANDLER_HEADER
#define SANEPP_API_HANDLER_HEADER

#include <sapi_objs/sapi_test_static_json.hpp>

static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp);

SapiTestStaticJson sapiTestStaticJsonResponse();

#endif
