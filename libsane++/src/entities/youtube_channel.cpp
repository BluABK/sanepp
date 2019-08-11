#include <iostream>
#include <map>

// 3rd party libraries.
#include <nlohmann/json.hpp>

#include <entities/youtube_channel.hpp>

namespace sane {
    // An empty constructor if you want to populate it later.
    YoutubeChannel::YoutubeChannel() = default;

    void YoutubeChannel::addFromJson(nlohmann::json t_json) {
        try {
            // Relevant JSON response values. See header for explanations.
            assignJsonStringValue(m_id, t_json["snippet"]["resourceId"]["channelId"]);

            // If channel ID is missing, check if this is a Channel resource not a Subscription resource.
            if (getId().empty()) {
                assignJsonStringValue(m_id, t_json["id"]);
            }

            // If channelId was valid, strip non-unique channel prefix from its head to produce the actual ID.
            if (!getId().empty()) {
                m_id = getId().substr(2);
            } else {
                addError("Missing Channel ID!", t_json);
                std::cerr << "Missing Channel ID!" << std::endl;
            }

            // Playlists
            std::string _;
            assignJsonStringValue(_, t_json["contentDetails"]["relatedPlaylists"]["favorites"]);
            m_hasFavouritesPlaylist = !_.empty();
            assignJsonStringValue(_, t_json["contentDetails"]["relatedPlaylists"]["likes"]);
            m_hasLikesPlaylist = !_.empty();
            assignJsonStringValue(_, t_json["contentDetails"]["relatedPlaylists"]["uploads"]);
            m_hasUploadsPlaylist = !_.empty();

            assignJsonStringValue(m_description, t_json["snippet"]["description"]);
            assignJsonStringValue(m_publishedAt, t_json["snippet"]["publishedAt"]);
            assignJsonStringValue(m_thumbnails["default"], t_json["snippet"]["thumbnails"]["default"]["url"]);
            assignJsonStringValue(m_thumbnails["high"], t_json["snippet"]["thumbnails"]["high"]["url"]);
            assignJsonStringValue(m_thumbnails["medium"], t_json["snippet"]["thumbnails"]["medium"]["url"]);
            assignJsonStringValue(m_title, t_json["snippet"]["title"]);
        } catch (nlohmann::detail::type_error &exc) {
            addError("Skipping YoutubeChannel::addFromJson due to Exception: " + std::string(exc.what()), t_json);
        } catch (const std::exception &exc) {
            addError("Skipping YoutubeChannel::addFromJson due to Unexpected Exception: " +
            std::string(exc.what()), t_json);
        }
    }

    void YoutubeChannel::addFromValues(const char* t_id, const char* t_uploadsPlaylist,
                                       const char* t_favouritesPlaylist, const char* t_likesPlaylist,
                                       const char* t_title, const char* t_description,
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
                                       bool t_hasLikesPlaylist, const char* t_title,
                                       const char* t_description, const char* t_thumbnailDefault,
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
        m_title                     = t_map["Title"];
        m_hasUploadsPlaylist        = !t_map["UploadsPlaylist"].empty();
        m_hasFavouritesPlaylist     = !t_map["FavouritesPlaylist"].empty();
        m_hasLikesPlaylist          = !t_map["LikesPlaylist"].empty();
        m_description               = t_map["Description"];
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
        if (!getId().empty() and hasFavouritesPlaylist()) {
            return std::string("FL" + getId());
        } else {
            return {};
        }
    }

    const std::string YoutubeChannel::getLikesPlaylist() {
        if (!getId().empty() and hasLikesPlaylist()) {
            return std::string("LL" + getId());
        } else {
            return {};
        }
    }

    const std::string YoutubeChannel::getUploadsPlaylist() {
        if (!getId().empty() and hasUploadsPlaylist()) {
            return std::string("UU" + getId());
        } else {
            return {};
        }
    }

    const std::string YoutubeChannel::getChannelId() {
        if (!getId().empty()) {
            return std::string("UC" + getId());
        } else {
            return {};
        }
    }

    const std::string YoutubeChannel::getId() {
        return m_id;
    }

    const char* YoutubeChannel::getIdAsCString() {
        return m_id.c_str();
    }

    const std::string YoutubeChannel::getDescription() {
        return m_description;
    }

    const char* YoutubeChannel::getDescriptionAsCString() {
        return m_description.c_str();
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

    const std::string YoutubeChannel::getTitle() {
        return m_title;
    }

    const char* YoutubeChannel::getTitleAsCString() {
        return m_title.c_str();
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
        _[t_errorMsg] = t_json;

        m_errors.push_back(_);
    }

    void YoutubeChannel::addWarning(const std::string &t_warningMsg, nlohmann::json &t_json) {
        std::map<std::string, nlohmann::json> _;
        _[t_warningMsg] = t_json;

        m_warnings.push_back(_);
    }

    std::list<std::map<std::string, nlohmann::json>> YoutubeChannel::getErrors() {
        return m_errors;
    }

    void YoutubeChannel::printErrors(int indent, bool withJson, int jsonIndent) {
        // For map in list
        for (auto const& item : getErrors()) {
            // For message, JSON in map
            for (auto const& map : item) {
                std::cout << std::string(indent, ' ') << map.first << std::endl;
                if (withJson) {
                    std::cout << map.second.dump(jsonIndent) << std::endl;
                }
            }
        }
    }

    std::list<std::map<std::string, nlohmann::json>> YoutubeChannel::getWarnings() {
        return m_warnings;
    }

    void YoutubeChannel::printWarnings(int indent, bool withJson, int jsonIndent) {
        // For map in list
        for (auto const& item : getWarnings()) {
            // For message, JSON in map
            for (auto const& map : item) {
                std::cout << std::string(indent, ' ') << map.first << std::endl;
                if (withJson) {
                    std::cout << map.second.dump(jsonIndent) << std::endl;
                }
            }
        }
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