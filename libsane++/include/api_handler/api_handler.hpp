/*
 *  SaneAPI handling via libcURL -- Headers.
 */
#ifndef SANEPP_API_HANDLER_HEADER
#define SANEPP_API_HANDLER_HEADER

#include <list>

#include <entities/youtube_channel.hpp>

#define SAPI_GET_SUBSCRIPTIONS "http://127.0.0.1:5002/api/v1/remote/subscriptions"

namespace sane {
    std::list<std::shared_ptr<YoutubeChannel>> sapiGetSubscriptions();

} // namespace sane.
#endif // Header guards.