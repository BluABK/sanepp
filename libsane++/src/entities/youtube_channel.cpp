#include <iostream>
#include <map>
#include <unicode/unistr.h>

// 3rd party libraries.
#include <nlohmann/json.hpp>

#include <entities/youtube_channel.hpp>

#define GET_VARIABLE_NAME(Variable) (#Variable)

namespace sane {
    // An empty constructor if you want to populate it later.
    YoutubeChannel::YoutubeChannel() = default;

    /**
     * Attempts to assign a JSON value to a given string and handle any bogus data.
     * If the input JSON value isn't a string it sets MISSING_VALUE.
     *
     * This function passes stringToAssignValue by reference (directly modififying it).
     *
     * @param stringToAssignValue   String to assign a value (modified directly).
     * @param unknownJsonTypeValue  The undetermined JSON value to assign the string.
     * @param fullJson              The complete JSON object, for use in debugging/error reporting.
     */
    void YoutubeChannel::assignJsonStringValue(std::string &stringToAssignValue,
            nlohmann::json &unknownJsonTypeValue, nlohmann::json &t_json) {
        if (unknownJsonTypeValue.is_null()) {
                addWarning("WARNING: YoutubeChannel::addFromJson." + std::string(GET_VARIABLE_NAME(stringToAssignValue))
                + " is NULL not string, setting '" + MISSING_VALUE + "' string in its stead!", t_json);
            stringToAssignValue = MISSING_VALUE;
        }
        else if (!unknownJsonTypeValue.is_string()) {
            addWarning("WARNING: YoutubeChannel::addFromJson.favouritesPlaylist is " +
            std::string(unknownJsonTypeValue.type_name()) + " not string, setting'" +
            MISSING_VALUE + "' string in its stead!", t_json);
            stringToAssignValue = MISSING_VALUE;
        } else {
            // If it actually is a string, then explicitly cast it.
            stringToAssignValue = unknownJsonTypeValue.get<std::string>();
        }
    }

    void YoutubeChannel::assignJsonStringValue(icu::UnicodeString &stringToAssignValue,
                                               nlohmann::json &unknownJsonTypeValue, nlohmann::json &t_json) {
        if (unknownJsonTypeValue.is_null()) {
            addWarning("WARNING: YoutubeChannel::addFromJson." + std::string(GET_VARIABLE_NAME(stringToAssignValue))
                       + " is NULL not string, setting '" + MISSING_VALUE + "' string in its stead!", t_json);
            stringToAssignValue = icu::UnicodeString(MISSING_VALUE);
        }
        else if (!unknownJsonTypeValue.is_string()) {
            addWarning("WARNING: YoutubeChannel::addFromJson.favouritesPlaylist is " +
                       std::string(unknownJsonTypeValue.type_name()) + " not string, setting'" +
                       MISSING_VALUE + "' string in its stead!", t_json);
            stringToAssignValue = icu::UnicodeString(MISSING_VALUE);
        } else {
            // If it actually is a string, then explicitly cast it.
            std::string jsonStr = unknownJsonTypeValue.get<std::string>();

            // Convert the std::string to unicode.
            stringToAssignValue = icu::UnicodeString::fromUTF8(icu::StringPiece(jsonStr.c_str()));
        }
    }

    void YoutubeChannel::addFromJson(nlohmann::json t_json) {
        try {
            // Relevant JSON response values. See header for explanations.
            assignJsonStringValue(m_id, t_json["snippet"]["resourceId"]["channelId"], t_json);

            // If channel ID is missing, check if this is a Channel resource not a Subscription resource.
            if (getId() == MISSING_VALUE) {
                assignJsonStringValue(m_id, t_json["id"], t_json);
            }

            // If channelId was valid, strip non-unique channel prefix from its head to produce the actual ID.
            if (getId() != MISSING_VALUE) {
                m_id = getId().substr(2);
            } else {
                addError("Missing Channel ID!", t_json);
                std::cerr << "Missing Channel ID!" << std::endl;
            }

            // Playlists
            std::string _;
            assignJsonStringValue(_, t_json["contentDetails"]["relatedPlaylists"]["favorites"], t_json);
            m_hasFavouritesPlaylist = _ != MISSING_VALUE;
            assignJsonStringValue(_, t_json["contentDetails"]["relatedPlaylists"]["likes"], t_json);
            m_hasLikesPlaylist = _ != MISSING_VALUE;
            assignJsonStringValue(_, t_json["contentDetails"]["relatedPlaylists"]["uploads"], t_json);
            m_hasUploadsPlaylist = _ != MISSING_VALUE;

            assignJsonStringValue(m_description, t_json["snippet"]["description"], t_json);
            assignJsonStringValue(m_publishedAt, t_json["snippet"]["publishedAt"], t_json);
            assignJsonStringValue(m_thumbnails["default"], t_json["snippet"]["thumbnails"]["default"]["url"], t_json);
            assignJsonStringValue(m_thumbnails["high"], t_json["snippet"]["thumbnails"]["high"]["url"], t_json);
            assignJsonStringValue(m_thumbnails["medium"], t_json["snippet"]["thumbnails"]["medium"]["url"], t_json);
            assignJsonStringValue(m_title, t_json["snippet"]["title"], t_json);
        } catch (nlohmann::detail::type_error &exc) {
            addError("Skipping YoutubeChannel::addFromJson due to Exception: " + std::string(exc.what()), t_json);
        } catch (const std::exception &exc) {
            addError("Skipping YoutubeChannel::addFromJson due to Unexpected Exception: " +
            std::string(exc.what()), t_json);
        }
    }

    void YoutubeChannel::addFromPureStringList(const std::list<std::string> &t_values) {  // FIXME: IMPLEMENT
        std::cerr << "YoutubeChannel::addFromPureStringList NOT IMPLEMENTED!" << std::endl;
    }

    void YoutubeChannel::addFromValues(const char* t_id, const char* t_uploadsPlaylist,
                                       const char* t_favouritesPlaylist, const char* t_likesPlaylist,
                                       const icu::UnicodeString &t_title, const icu::UnicodeString &t_description,
                                       const char* t_thumbnailDefault, const char* t_thumbnailHigh,
                                       const char* t_thumbnailMedium, bool t_subscribedOnYoutube,
                                       bool t_subscribedLocalOverride) {

        m_id                        = t_id;
        m_title                     = t_title;
        m_hasUploadsPlaylist        = *t_uploadsPlaylist != 0;
        m_hasFavouritesPlaylist     = *t_favouritesPlaylist != 0;
        m_hasLikesPlaylist          = *t_likesPlaylist != 0;
        m_description               = t_description;
        m_thumbnails["default"]     = t_thumbnailDefault;
        m_thumbnails["high"]        = t_thumbnailHigh;
        m_thumbnails["medium"]      = t_thumbnailMedium;
        m_subscribedOnYoutube       = t_subscribedOnYoutube;
        m_subscribedLocalOverride   = t_subscribedLocalOverride;
    }

    void YoutubeChannel::addFromValues(const char* t_id, bool t_hasUploadsPlaylist, bool t_hasFavouritesPlaylist,
                                       bool t_hasLikesPlaylist, const icu::UnicodeString &t_title,
                                       const icu::UnicodeString &t_description, const char* t_thumbnailDefault,
                                       const char* t_thumbnailHigh, const char* t_thumbnailMedium,
                                       bool t_subscribedOnYoutube, bool t_subscribedLocalOverride) {
        m_id                        = t_id;
        m_title                     = t_title;
        m_hasUploadsPlaylist        = t_hasUploadsPlaylist;
        m_hasFavouritesPlaylist     = t_hasFavouritesPlaylist;
        m_hasLikesPlaylist          = t_hasLikesPlaylist;
        m_description               = t_description;
        m_thumbnails["default"]     = t_thumbnailDefault;
        m_thumbnails["high"]        = t_thumbnailHigh;
        m_thumbnails["medium"]      = t_thumbnailMedium;
        m_subscribedOnYoutube       = t_subscribedOnYoutube;
        m_subscribedLocalOverride   = t_subscribedLocalOverride;
    }

    void YoutubeChannel::addFromMixedValues(const char* t_id, const char* t_uploadsPlaylist,
            const char* t_favouritesPlaylist, const char* t_likesPlaylist, void const * t_title,
            void const * t_description, const char* t_thumbnailDefault, const char* t_thumbnailHigh,
            const char* t_thumbnailMedium, bool t_subscribedOnYoutube, bool t_subscribedLocalOverride) {

        m_id                        = t_id;
        m_title                     = icu::UnicodeString::fromUTF8(icu::StringPiece(
                static_cast<const char *>(t_title)));
        m_hasUploadsPlaylist        = t_uploadsPlaylist;
        m_hasFavouritesPlaylist     = t_favouritesPlaylist;
        m_hasLikesPlaylist          = t_likesPlaylist;
        m_description               = icu::UnicodeString::fromUTF8(icu::StringPiece(
                static_cast<const char *>(t_description)));
        m_thumbnails["default"]     = t_thumbnailDefault;
        m_thumbnails["high"]        = t_thumbnailHigh;
        m_thumbnails["medium"]      = t_thumbnailMedium;
        m_subscribedOnYoutube       = t_subscribedOnYoutube;
        m_subscribedLocalOverride   = t_subscribedLocalOverride;
    }

    /**
     * Populates properties based on a std::string map of values.
     *
     * Map keys: ID, Title, UploadsPlaylist, FavouritesPlaylist, LikesPlaylist,
     *           Description, ThumbnailDefault, ThumbnailHigh, ThumbnailMedium.
     *
     * @param t_map
     */
    void YoutubeChannel::addFromMap(std::map<std::string, std::string> &t_map) {
        // Add values from given value map.
        m_id                        = t_map["ID"];
        m_title                     = icu::UnicodeString::fromUTF8(icu::StringPiece(t_map["Title"].c_str()));
        m_hasUploadsPlaylist        = !t_map["UploadsPlaylist"].empty();
        m_hasFavouritesPlaylist     = !t_map["FavouritesPlaylist"].empty();
        m_hasLikesPlaylist          = !t_map["LikesPlaylist"].empty();
        m_description               = icu::UnicodeString::fromUTF8(icu::StringPiece(t_map["Description"].c_str()));
        m_thumbnails["default"]     = t_map["ThumbnailDefault"];
        m_thumbnails["high"]        = t_map["ThumbnailHigh"];
        m_thumbnails["medium"]      = t_map["ThumbnailMedium"];
        m_subscribedOnYoutube       = t_map["SubscribedOnYouTube"] == "true";
        m_subscribedLocalOverride   = t_map["SubscribedLocalOverride"] == "true";
    }

    /**
     * Populates properties based on a SQLite UTF-8 map of values.
     *
     * Map keys: ID, Title, UploadsPlaylist, FavouritesPlaylist, LikesPlaylist,
     *           Description, ThumbnailDefault, ThumbnailHigh, ThumbnailMedium.
     *
     * @param t_map
     */
    void YoutubeChannel::addFromMap(std::map<std::string, const unsigned char*> &t_map) {
        // Add values from given value map.
        m_id                        = reinterpret_cast<const char *>(t_map["ID"]);
        m_title                     = (const char *)t_map["Title"];
        m_hasUploadsPlaylist        = !std::string(reinterpret_cast<const char *>(t_map["UploadsPlaylist"])).empty();
        m_hasFavouritesPlaylist     = !std::string(reinterpret_cast<const char *>(t_map["FavouritesPlaylist"])).empty();
        m_hasLikesPlaylist          = !std::string(reinterpret_cast<const char *>(t_map["LikesPlaylist"])).empty();
        m_description               = (const char *)t_map["Description"];
        m_thumbnails["default"]     = reinterpret_cast<const char *>(t_map["ThumbnailDefault"]);
        m_thumbnails["high"]        = reinterpret_cast<const char *>(t_map["ThumbnailHigh"]);
        m_thumbnails["medium"]      = reinterpret_cast<const char *>(t_map["ThumbnailMedium"]);
    }

    const std::string YoutubeChannel::getFavouritesPlaylist() {
        if (getId() != MISSING_VALUE and hasFavouritesPlaylist()) {
            return std::string("FL" + getId());
        } else {
            return MISSING_VALUE;
        }
    }

//    const char* YoutubeChannel::getFavouritesPlaylistAsCString() {
//        if (getId() != MISSING_VALUE and hasFavouritesPlaylist()) {
//            char buf[100];
//            strcpy(buf, "FL");
//            strcat(buf, getIdAsCString());
//            return buf;
//        } else {
//            return MISSING_VALUE;
//        }
//    }

    const std::string YoutubeChannel::getLikesPlaylist() {
        if (getId() != MISSING_VALUE and hasLikesPlaylist()) {
            return std::string("LL" + getId());
        } else {
            return MISSING_VALUE;
        }
    }

    const std::string YoutubeChannel::getUploadsPlaylist() {
        if (getId() != MISSING_VALUE and hasUploadsPlaylist()) {
            return std::string("UU" + getId());
        } else {
            return MISSING_VALUE;
        }
    }

    const std::string YoutubeChannel::getChannelId() {
        if (getId() != MISSING_VALUE) {
            return std::string("UC" + getId());
        } else {
            return MISSING_VALUE;
        }
    }

    const std::string YoutubeChannel::getId() {
        return m_id;
    }

    const char* YoutubeChannel::getIdAsCString() {
        return m_id.c_str();
    }

    const icu::UnicodeString YoutubeChannel::getDescription() {
        return m_description;
    }

    /**
     * Converts the UTF-8 value to std::string.
     * BEWARE: Can cause out of scope issues.
     *
     * @return std::string m_titleAsString
     */
    const std::string YoutubeChannel::getDescriptionAsString() {
        std::string descriptionAsString;

        // Convert the UnicodeString to UTF-8 and append the result to a standard string.
        getDescription().toUTF8String(descriptionAsString);

        return descriptionAsString;
    }

    const std::string YoutubeChannel::getPublishedAt() {
        return m_publishedAt;
    }

    const char* YoutubeChannel::getPublishedAtAsCString() {
        return m_publishedAt.c_str();
    }

    const std::map<std::string, std::string> YoutubeChannel::getThumbnails() {
        return m_thumbnails;
    }

    const std::string YoutubeChannel::getThumbnailDefault()  {
        return m_thumbnails["default"];
    }

    const char*YoutubeChannel::getThumbnailDefaultAsCString()  {
        return m_thumbnails["default"].c_str();
    }

    const std::string YoutubeChannel::getThumbnailHigh() {
        return m_thumbnails["high"];
    }

    const char* YoutubeChannel::getThumbnailHighAsCString() {
        return m_thumbnails["high"].c_str();
    }

    const std::string YoutubeChannel::getThumbnailMedium() {
        return m_thumbnails["medium"];
    }

    const char* YoutubeChannel::getThumbnailMediumAsCString() {
        return m_thumbnails["medium"].c_str();
    }

    const icu::UnicodeString YoutubeChannel::getTitle() {
        return m_title;
    }

    /**
     * Converts the UTF-8 value to std::string.
     * BEWARE: Can cause out of scope issues.
     *
     * @return std::string m_titleAsString
     */
    const std::string YoutubeChannel::getTitleAsString() {
        std::string titleAsString;

        // Convert the UnicodeString to UTF-8 and append the result to a standard string.
        getTitle().toUTF8String(titleAsString);

        return titleAsString;
    }

    void YoutubeChannel::print(int indentationSpacing = 0) {
        std::string indentation(indentationSpacing, ' ');

        std::cout << indentation << "Title: " << getTitleAsString() << std::endl;
        std::cout << indentation << "ID: " << getId() << std::endl;
        std::cout << indentation << "Channel ID: " << getChannelId() << std::endl;
        std::cout << indentation << "Published: " << getPublishedAt() << std::endl;
        std::cout << indentation << "Description: " << getDescriptionAsString() << std::endl;
        std::cout << indentation << "Favourites Playlist: " << getFavouritesPlaylist() << std::endl;
        std::cout << indentation << "Uploads Playlist: " << getUploadsPlaylist() << std::endl;
        std::cout << indentation << "Thumbnail URL (default): " << getThumbnailDefault()<< std::endl;
        std::cout << indentation << "Thumbnail URL (high): " << getThumbnailHigh() << std::endl;
        std::cout << indentation << "Thumbnail URL (medium): " << getThumbnailMedium() << std::endl;
    }

    void YoutubeChannel::addError(const std::string &t_errorMsg, nlohmann::json &t_json) {
        std::map<std::string, nlohmann::json> _;
        _["error"] = t_errorMsg;
        _["json"] = t_json;

        m_errors.push_back(_);
    }

    void YoutubeChannel::addWarning(const std::string &t_warningMsg, nlohmann::json &t_json) {
        std::map<std::string, nlohmann::json> _;
        _["error"] = t_warningMsg;
        _["json"] = t_json;

        m_warnings.push_back(_);
    }

    std::list<std::map<std::string, nlohmann::json>> YoutubeChannel::getErrors() {
        return m_errors;
    }

    std::list<std::map<std::string, nlohmann::json>> YoutubeChannel::getWarnings() {
        return m_warnings;
    }

    bool YoutubeChannel::wasAborted() {
        return m_aborted;
    }

    void YoutubeChannel::clearWarnings() {
        m_warnings.clear();
    }

    void YoutubeChannel::clearErrors() {
        m_errors.clear();
    }

    void YoutubeChannel::clearErrorsAndWarnings() {
        clearWarnings();
        clearErrors();
    }

    bool YoutubeChannel::hasFavouritesPlaylist() {
        return m_hasFavouritesPlaylist;
    }

    bool YoutubeChannel::hasUploadsPlaylist() {
        return m_hasUploadsPlaylist;
    }

    bool YoutubeChannel::hasLikesPlaylist() {
        return m_hasLikesPlaylist;
    }


} // namespace sane