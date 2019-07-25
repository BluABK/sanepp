/*
 *  SaneAPI handling via libcURL -- Headers.
 */

#ifndef SANEPP_API_HANDLER_HEADER
#define SANEPP_API_HANDLER_HEADER

#include <list>

#include <src/sapi_objs/sapi_test_static_json.hpp>
#include <entities/youtube_channel.hpp>

namespace sane {
    SapiTestStaticJson sapiTestStaticJsonResponse();

    std::list<std::shared_ptr<YoutubeChannel>> sapiGetSubscriptions();
} // namespace sane.
#endif // Header guards.
