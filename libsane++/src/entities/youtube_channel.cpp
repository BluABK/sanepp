#include <iostream>

// 3rd party libraries.
#include <nlohmann/json.hpp>

#include "youtube_channel.hpp"

#define GET_VARIABLE_NAME(Variable) (#Variable)

namespace sane {
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
            const nlohmann::json &unknownJsonTypeValue, const nlohmann::json &fullJson) {
        if (unknownJsonTypeValue.is_null()) {
            if (reportWarnings) {
                std::cerr << "WARNING: YoutubeChannel::addFromJson." << GET_VARIABLE_NAME(stringToAssignValue) <<
                     " is NULL not string, setting '" <<
                     MISSING_VALUE << "' string in its stead!" << std::endl;
            }
            stringToAssignValue = MISSING_VALUE;
            warningCount++;
        }
        else if (!unknownJsonTypeValue.is_string()) {
            if (reportWarnings) {
                std::cerr << "WARNING: YoutubeChannel::addFromJson.favouritesPlaylist is " <<
                unknownJsonTypeValue.type_name() << " not string, setting'" <<
                MISSING_VALUE << "' string in its stead!" << std::endl;
            }
            stringToAssignValue = "N/A";
            std::cerr << fullJson.dump(4);
            warningCount++;
        } else {
            // If it actually is a string, then explicitly cast it.
            stringToAssignValue = unknownJsonTypeValue.get<std::string>();
        }
    }

    void YoutubeChannel::addFromJson(nlohmann::json t_data) {
        try {
            // Relevant JSON response values. See header for explanations.
            assignJsonStringValue(id, t_data["snippet"]["resourceId"]["channelId"], t_data);
            // If channelId was valid, strip non-unique channel prefix from its head to produce the actual ID.
            if (id != MISSING_VALUE) {
                id = id.substr(2);
            }

            // Playlists
            std::string _;
            assignJsonStringValue(_, t_data["contentDetails"]["relatedPlaylists"]["favorites"], t_data);
            m_hasFavouritesPlaylist = _ != MISSING_VALUE;
            assignJsonStringValue(_, t_data["contentDetails"]["relatedPlaylists"]["likes"], t_data);
            m_hasLikesPlaylist = _ != MISSING_VALUE;
            assignJsonStringValue(_, t_data["contentDetails"]["relatedPlaylists"]["uploads"], t_data);
            m_hasUploadsPlaylist = _ != MISSING_VALUE;

            assignJsonStringValue(description, t_data["snippet"]["description"], t_data);
            assignJsonStringValue(publishedAt, t_data["snippet"]["publishedAt"], t_data);
            assignJsonStringValue(thumbnails["default"], t_data["snippet"]["thumbnails"]["default"]["url"], t_data);
            assignJsonStringValue(thumbnails["high"], t_data["snippet"]["thumbnails"]["high"]["url"], t_data);
            assignJsonStringValue(thumbnails["medium"], t_data["snippet"]["thumbnails"]["medium"]["url"], t_data);
            assignJsonStringValue(title, t_data["snippet"]["title"], t_data);
        } catch (nlohmann::detail::type_error &exc) {
            if (reportErrors) {
                std::cerr << "ERROR: Skipping YoutubeChannel::addFromJson "
                        "due to Exception: " << exc.what() << std::endl;
                std::cerr << t_data.dump(4);
            }
        } catch (const std::exception &exc) {
            if (reportErrors) {
                std::cerr << "ERROR: Skipping YoutubeChannel::addFromJson "
                        "due to Unexpected Exception: " << exc.what() << std::endl;
                std::cerr << t_data.dump(4);
            }
        }
    }

    const std::string YoutubeChannel::getFavouritesPlaylist() {
        if (id != MISSING_VALUE and hasFavouritesPlaylist()) {
            return std::string("FL" + id);
        } else {
            return MISSING_VALUE;
        }
    }

    const std::string YoutubeChannel::getLikesPlaylist() {
        if (id != MISSING_VALUE and hasLikesPlaylist()) {
            return std::string("LL" + id);
        } else {
            return MISSING_VALUE;
        }
    }

    const std::string YoutubeChannel::getUploadsPlaylist() {
        if (id != MISSING_VALUE and hasUploadsPlaylist()) {
            return std::string("UU" + id);
        } else {
            return MISSING_VALUE;
        }
    }

    const std::string YoutubeChannel::getChannelId() {
        if (id != MISSING_VALUE) {
            return std::string("UC" + id);
        } else {
            return MISSING_VALUE;
        }
    }

    const std::string YoutubeChannel::getId() {
        return id;
    }

    const std::string YoutubeChannel::getDescription() {
        return description;
    }

    const std::string YoutubeChannel::getPublishedAt() {
        return publishedAt;
    }

    const std::map<std::string, std::string> YoutubeChannel::getThumbnails() {
        return thumbnails;
    }

    const std::string YoutubeChannel::getThumbnailDefault()  {
        return thumbnails["default"];
    }

    const std::string YoutubeChannel::getThumbnailHigh() {
        return thumbnails["high"];
    }

    const std::string YoutubeChannel::getThumbnailMedium() {
        return thumbnails["medium"];
    }

    const std::string YoutubeChannel::getTitle() {
        return title;
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

    int YoutubeChannel::getErrorCount() {
        return errorCount;
    }

    int YoutubeChannel::getWarningCount() {
        return warningCount;
    }

    bool YoutubeChannel::wasAborted() {
        return aborted;
    }

    void YoutubeChannel::enableWarnings(bool b) {
        reportWarnings = b;
    }

    void YoutubeChannel::enableErrors(bool b) {
        reportErrors = b;
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