#ifndef SANE_COMMON_HPP
#define SANE_COMMON_HPP

#include <list>
#include <map>
#include <vector>
#include <string>
#include <list>

#include <nlohmann/json.hpp>


#define GET_VARIABLE_NAME(Variable) (#Variable)

#define MISSING_VALUE "N/A"
#define YOUTUBE_CHANNEL_PREFIX "UC"
#define YOUTUBE_UPLOADS_PLAYLIST_PREFIX "UU"
#define YOUTUBE_FAVOURITES_PLAYLIST_PREFIX "FL"
#define YOUTUBE_LIKES_PLAYLIST_PREFIX "LL"

#define JSON_VALUE_OK 0
#define JSON_VALUE_ERROR 1
#define JSON_VALUE_NULL 2
#define JSON_VALUE_NOT_STRING 3


namespace sane {
    int assignJsonStringValue(std::string &stringToAssignValue,
                              nlohmann::json &unknownJsonTypeValue);

    bool isBool(nlohmann::json &t_bool);

    bool getJsonBoolValue(nlohmann::json &t_bool);
} // namespace sane
#endif //SANE_COMMON_HPP
