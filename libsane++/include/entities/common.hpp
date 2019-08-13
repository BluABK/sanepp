#ifndef SANE_COMMON_HPP
#define SANE_COMMON_HPP

#include <list>
#include <map>
#include <vector>
#include <string>
#include <list>
#include <sstream>

#include <nlohmann/json.hpp>


#define GET_VARIABLE_NAME(Variable) (#Variable)

#define YOUTUBE_CHANNEL_PREFIX "UC"
#define YOUTUBE_UPLOADS_PLAYLIST_PREFIX "UU"
#define YOUTUBE_FAVOURITES_PLAYLIST_PREFIX "FL"
#define YOUTUBE_LIKES_PLAYLIST_PREFIX "LL"

namespace sane {
    struct thumbnail_t {
        std::string url;
        unsigned int height{};
        unsigned int width{};
    };

    bool isBool(nlohmann::json &t_bool);

    bool isDigits(nlohmann::json &t_json);

    std::string getJsonStringValue(nlohmann::json &t_string, const std::string &t_funcName,
            std::map<std::string, std::string> &t_problems);

    bool getJsonBoolValue(nlohmann::json &t_bool, const std::string &t_funcName,
            std::map<std::string, std::string> &t_problems);

    long getJsonLongValue(nlohmann::json &t_long, const std::string &t_funcName,
            std::map<std::string, std::string> &t_problems);

    unsigned long getJsonULongValue(nlohmann::json &t_ulong, const std::string &t_funcName,
            std::map<std::string, std::string> &t_problems);

    void printIndentedString(int t_spacing, const std::string &t_string, std::string t_title = {});
} // namespace sane
#endif //SANE_COMMON_HPP
