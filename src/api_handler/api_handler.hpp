/*
 *  SaneAPI handling via libcURL -- Headers.
 */

#ifndef SANEPP_API_HANDLER_HEADER
#define SANEPP_API_HANDLER_HEADER

#include <sapi_objs/sapi_test_static_json.hpp>
#include "youtube_subscription.hpp"

SapiTestStaticJson sapiTestStaticJsonResponse();

std::shared_ptr<YoutubeSubscription> sapiGetSubscriptions();

#endif
