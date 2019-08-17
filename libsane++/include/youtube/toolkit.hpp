#ifndef SANE_TOOLKIT_HPP
#define SANE_TOOLKIT_HPP

#include <api_handler/api_handler.hpp>
#include <entities/youtube_video.hpp>
#include <entities/youtube_channel.hpp>
#include <types.hpp>

namespace sane {
    bool hasItems(const nlohmann::json &t_jsonList);
}

#endif //SANE_TOOLKIT_HPP
