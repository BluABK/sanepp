/*
 *  SaneAPI handling via libcURL -- Headers.
 */

#ifndef SANEPP_API_HANDLER_HEADER
#define SANEPP_API_HANDLER_HEADER

#include <list>

#include <entities/youtube_channel.hpp>

namespace sane {
    std::list<std::shared_ptr<YoutubeChannel>> sapiGetSubscriptions();
} // namespace sane.
#endif // Header guards.
