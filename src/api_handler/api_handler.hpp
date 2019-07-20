/*
 *  SaneAPI handling via libcURL -- Headers.
 */

#ifndef SANEPP_API_HANDLER_HEADER
#define SANEPP_API_HANDLER_HEADER

#include <list>

#include <sapi_objs/sapi_test_static_json.hpp>
#include "youtube_subscription.hpp"

SapiTestStaticJson sapiTestStaticJsonResponse();

std::list <std::shared_ptr<YoutubeSubscription>> sapiGetSubscriptions();

#endif
