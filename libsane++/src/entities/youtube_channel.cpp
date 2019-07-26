#include <iostream>

// 3rd party libraries.
#include <nlohmann/json.hpp>

#include "youtube_channel.hpp"

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

    void YoutubeChannel::addFromJson(nlohmann::json t_json) {
        try {
            // Relevant JSON response values. See header for explanations.
            assignJsonStringValue(m_id, t_json["snippet"]["resourceId"]["channelId"], t_json);
            // If channelId was valid, strip non-unique channel prefix from its head to produce the actual ID.
            if (getId() != MISSING_VALUE) {
                m_id = getId().substr(2);
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

    void YoutubeChannel::addFromStringList(const std::list<std::string> &t_values) {  // FIXME: IMPLEMENT
        std::cerr << "YoutubeChannel::addFromStringList NOT IMPLEMENTED!" << std::endl;
    }

    void YoutubeChannel::addFromMap(const std::map<std::string, std::string> &t_map) {  // FIXME: IMPLEMENT
        std::cerr << "YoutubeChannel::addFromMap NOT IMPLEMENTED!" << std::endl;
    }

    const std::string YoutubeChannel::getFavouritesPlaylist() {
        if (getId() != MISSING_VALUE and hasFavouritesPlaylist()) {
            return std::string("FL" + getId());
        } else {
            return MISSING_VALUE;
        }
    }

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

    const std::string YoutubeChannel::getDescription() {
        return m_description;
    }

    const std::string YoutubeChannel::getPublishedAt() {
        return m_publishedAt;
    }

    const std::map<std::string, std::string> YoutubeChannel::getThumbnails() {
        return m_thumbnails;
    }

    const std::string YoutubeChannel::getThumbnailDefault()  {
        return m_thumbnails["default"];
    }

    const std::string YoutubeChannel::getThumbnailHigh() {
        return m_thumbnails["high"];
    }

    const std::string YoutubeChannel::getThumbnailMedium() {
        return m_thumbnails["medium"];
    }

    const std::string YoutubeChannel::getTitle() {
        return m_title;
    }


    void YoutubeChannel::print(int indentationSpacing = 0) {
        std::string indentation(indentationSpacing, ' ');

        std::cout << indentation << "Title: " << getTitle() << std::endl;
        std::cout << indentation << "ID: " << getId() << std::endl;
        std::cout << indentation << "Channel ID: " << getChannelId() << std::endl;
        std::cout << indentation << "Published: " << getPublishedAt() << std::endl;
        std::cout << indentation << "Description: " << getDescription() << std::endl;
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