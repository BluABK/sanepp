#include <iostream>
#include <map>

// 3rd party libraries.
#include <nlohmann/json.hpp>

#include <entities/common.hpp>
#include <entities/youtube_channel.hpp>

namespace sane {
    // An empty constructor if you want to populate it later.
    YoutubeChannel::YoutubeChannel() = default;

    void YoutubeChannel::addFromJson(nlohmann::json t_json) {
        try {
            // Relevant JSON response values. See header for explanations.
            setId(t_json["snippet"]["resourceId"]["channelId"]);

            // If channel ID is missing, check if this is a Channel resource not a Subscription resource.
            if (getId().empty()) {
                setId(t_json["id"]);
            }

            // If channelId was valid, strip non-unique channel prefix from its head to produce the actual ID.
            if (!getId().empty()) {
                m_id = getId().substr(2);
            } else {
                addError("Missing Channel ID!", t_json);
                std::cerr << "Missing Channel ID!" << std::endl;
            }

            // Playlists
            nlohmann::json relatedPlaylists = t_json["contentDetails"]["relatedPlaylists"];
            setHasFavouritesPlaylist(relatedPlaylists["favorites"].is_string());
            setHasUploadsPlaylist(relatedPlaylists["uploads"].is_string());
            setHasLikesPlaylist(relatedPlaylists["likes"].is_string());

            setDescription(t_json["snippet"]["description"]);
            setPublishedAt(t_json["snippet"]["publishedAt"]);
            setThumbnails(t_json["snippet"]["thumbnails"]);
            setTitle(t_json["snippet"]["title"]);
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

        setId(std::string(t_id));
        setTitle(std::string(t_title));
        setHasUploadsPlaylist(*t_uploadsPlaylist != 0);
        setHasFavouritesPlaylist(*t_favouritesPlaylist != 0);
        setHasLikesPlaylist(*t_likesPlaylist != 0);
        setDescription(std::string(t_description));

        // Thumbnails
        std::map<std::string, thumbnail_t> thumbs;
        thumbnail_t thumbnailDefault;
        thumbnail_t thumbnailHigh;
        thumbnail_t thumbnailMedium;

        thumbnailDefault.url = std::string(t_thumbnailDefault);
        thumbnailHigh.url = std::string(t_thumbnailHigh);
        thumbnailMedium.url = std::string(t_thumbnailMedium);

        thumbs["default"] = thumbnailDefault;
        thumbs["high"] = thumbnailHigh;
        thumbs["medium"] = thumbnailMedium;

        setThumbnails(thumbs);

        setIsSubscribedOnYoutube(t_subscribedOnYoutube);
        setHasSubscribedLocalOverride(t_subscribedLocalOverride);
    }

    void YoutubeChannel::addFromValues(const char* t_id, bool t_hasUploadsPlaylist, bool t_hasFavouritesPlaylist,
                                       bool t_hasLikesPlaylist, const char* t_title,
                                       const char* t_description, const char* t_thumbnailDefault,
                                       const char* t_thumbnailHigh, const char* t_thumbnailMedium,
                                       bool t_subscribedOnYoutube, bool t_subscribedLocalOverride) {
        setId(std::string(t_id));
        setTitle(std::string(t_title));
        setHasUploadsPlaylist(t_hasUploadsPlaylist);
        setHasFavouritesPlaylist(t_hasFavouritesPlaylist);
        setHasLikesPlaylist(t_hasLikesPlaylist);
        setDescription(std::string(t_description));

        // Thumbnails
        std::map<std::string, thumbnail_t> thumbs;
        thumbnail_t thumbnailDefault;
        thumbnail_t thumbnailHigh;
        thumbnail_t thumbnailMedium;

        thumbnailDefault.url = std::string(t_thumbnailDefault);
        thumbnailHigh.url = std::string(t_thumbnailHigh);
        thumbnailMedium.url = std::string(t_thumbnailMedium);

        thumbs["default"] = thumbnailDefault;
        thumbs["high"] = thumbnailHigh;
        thumbs["medium"] = thumbnailMedium;

        setThumbnails(thumbs);

        setIsSubscribedOnYoutube(t_subscribedOnYoutube);
        setHasSubscribedLocalOverride(t_subscribedLocalOverride);
    }

    void YoutubeChannel::addFromValues(const std::string &t_id, bool t_hasUploadsPlaylist,
                                       bool t_hasFavouritesPlaylist, bool t_hasLikesPlaylist,
                                       const std::string &t_title, const std::string &t_description,
                                       const std::string &t_thumbnailDefault, const std::string &t_thumbnailHigh,
                                       const std::string &t_thumbnailMedium, bool t_subscribedOnYoutube,
                                       bool t_subscribedLocalOverride) {

        setId(t_id);
        setTitle(t_title);
        setHasUploadsPlaylist(t_hasUploadsPlaylist);
        setHasFavouritesPlaylist(t_hasFavouritesPlaylist);
        setHasLikesPlaylist(t_hasLikesPlaylist);
        setDescription(t_description);

        // Thumbnails
        std::map<std::string, thumbnail_t> thumbs;
        thumbnail_t thumbnailDefault;
        thumbnail_t thumbnailHigh;
        thumbnail_t thumbnailMedium;

        thumbnailDefault.url = t_thumbnailDefault;
        thumbnailHigh.url = t_thumbnailHigh;
        thumbnailMedium.url = t_thumbnailMedium;

        thumbs["default"] = thumbnailDefault;
        thumbs["high"] = thumbnailHigh;
        thumbs["medium"] = thumbnailMedium;

        setThumbnails(thumbs);

        setIsSubscribedOnYoutube(t_subscribedOnYoutube);
        setHasSubscribedLocalOverride(t_subscribedLocalOverride);
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
        addFromValues(t_map["ID"], !t_map["UploadsPlaylist"].empty(), !t_map["FavouritesPlaylist"].empty(),
                      !t_map["LikesPlaylist"].empty(), t_map["Title"], t_map["Description"],
                      t_map["ThumbnailDefault"], t_map["ThumbnailHigh"],
                      t_map["ThumbnailMedium"], t_map["SubscribedOnYouTube"] == "true",
                      t_map["SubscribedLocalOverride"] == "true");

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
        setId(std::string(reinterpret_cast<const char *>(t_map["ID"])));
        setTitle(std::string((const char *)t_map["Title"]));
        setHasUploadsPlaylist(!std::string(reinterpret_cast<const char *>(t_map["UploadsPlaylist"])).empty());
        setHasFavouritesPlaylist(!std::string(reinterpret_cast<const char *>(t_map["FavouritesPlaylist"])).empty());
        setHasLikesPlaylist(!std::string(reinterpret_cast<const char *>(t_map["LikesPlaylist"])).empty());
        setDescription(std::string((const char *)t_map["Description"]));

        // Thumbnails
        std::map<std::string, thumbnail_t> thumbs;
        thumbnail_t thumbnailDefault;
        thumbnail_t thumbnailHigh;
        thumbnail_t thumbnailMedium;

        thumbnailDefault.url = reinterpret_cast<const char *>(t_map["ThumbnailDefault"]);
        thumbnailHigh.url = reinterpret_cast<const char *>(t_map["ThumbnailHigh"]);
        thumbnailMedium.url = reinterpret_cast<const char *>(t_map["ThumbnailMedium"]);

        thumbs["default"] = thumbnailDefault;
        thumbs["high"] = thumbnailHigh;
        thumbs["medium"] = thumbnailMedium;

        setThumbnails(thumbs);
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

    const std::string &YoutubeChannel::getId() const {
        return m_id;
    }

    void YoutubeChannel::setId(const std::string &t_id) {
        m_id = t_id;
    }

    void YoutubeChannel::setId(nlohmann::json t_id) {
        if (t_id.is_string()) {
            std::map<std::string, std::string> problems;
            
            setId(getJsonStringValue(t_id, "setId", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeChannel::getDescription() const {
        return m_description;
    }

    void YoutubeChannel::setDescription(const std::string &t_description) {
        m_description = t_description;
    }

    void YoutubeChannel::setDescription(nlohmann::json &t_description) {
        if (t_description.is_string()) {
            std::map<std::string, std::string> problems;

            setDescription(getJsonStringValue(t_description, "setDescription", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeChannel::getPublishedAt() const {
        return m_publishedAt;
    }

    void YoutubeChannel::setPublishedAt(const std::string &t_publishedAt) {
        m_publishedAt = t_publishedAt;
    }

    void YoutubeChannel::setPublishedAt(nlohmann::json &t_publishedAt) {
        if (t_publishedAt.is_string()) {
            std::map<std::string, std::string> problems;

            setPublishedAt(getJsonStringValue(t_publishedAt, "setPublishedAt", problems));
            reportProblems(problems);
        }
    }

    const std::map<std::string, thumbnail_t> &YoutubeChannel::getThumbnails() const {
        return m_thumbnails;
    }

    void YoutubeChannel::setThumbnails(const std::map<std::string, thumbnail_t> &t_thumbnails) {
        m_thumbnails = t_thumbnails;
    }

    void YoutubeChannel::setThumbnails(nlohmann::json &t_thumbnails) {
        // Define variables.
        std::map<std::string, thumbnail_t> thumbnails;

        // Create each thumbnail struct.
        thumbnail_t defaultThumbnail = thumbnail_t();
        if (t_thumbnails["default"]["url"].is_string()) {
            std::map<std::string, std::string> problems;

            defaultThumbnail.url = getJsonStringValue(t_thumbnails["default"]["url"],
                                                                    "setThumbnails [default]", problems);
            reportProblems(problems);
        }

        thumbnail_t highThumbnail = thumbnail_t();
        if (t_thumbnails["high"]["url"].is_string()) {
            std::map<std::string, std::string> problems;

            highThumbnail.url = getJsonStringValue(t_thumbnails["high"]["url"],
                                                                 "setThumbnails [high]", problems);
            reportProblems(problems);
        }

        thumbnail_t mediumThumbnail = thumbnail_t();
        if (t_thumbnails["medium"]["url"].is_string()) {
            std::map<std::string, std::string> problems;

            mediumThumbnail.url = getJsonStringValue(t_thumbnails["medium"]["url"],
                                                                   "setThumbnails [medium]", problems);
            reportProblems(problems);
        }

        // Add thumbnail structs to map.
        thumbnails["default"] = defaultThumbnail;
        thumbnails["high"] = highThumbnail;
        thumbnails["medium"] = mediumThumbnail;

        // Finally assign the thumbnails map to the object property.
        setThumbnails(thumbnails);
    }

    const std::string &YoutubeChannel::getTitle() const {
        return m_title;
    }

    void YoutubeChannel::setTitle(const std::string &t_title) {
        m_title = t_title;
    }

    void YoutubeChannel::setTitle(nlohmann::json &t_title) {
        if (t_title.is_string()) {
            std::map<std::string, std::string> problems;

            setTitle(getJsonStringValue(t_title, "setTitle", problems));
            reportProblems(problems);
        }
    }

    void YoutubeChannel::print(int indentationSpacing = 0) {
        std::string indentation(indentationSpacing, ' ');
        std::map<std::string, thumbnail_t> thumbnails = getThumbnails();

        std::cout << indentation << "Title: " << getTitle() << std::endl;
        std::cout << indentation << "ID: " << getId() << std::endl;
        std::cout << indentation << "Channel ID: " << getChannelId() << std::endl;
        std::cout << indentation << "Published: " << getPublishedAt() << std::endl;
        std::cout << indentation << "Description: " << getDescription() << std::endl;
        std::cout << indentation << "Favourites Playlist: " << getFavouritesPlaylist() << std::endl;
        std::cout << indentation << "Uploads Playlist: " << getUploadsPlaylist() << std::endl;

        std::cout << indentation << "Thumbnail URL (default): " << thumbnails["default"].url << std::endl;
        std::cout << indentation << "Thumbnail URL (high): " << thumbnails["high"].url << std::endl;
        std::cout << indentation << "Thumbnail URL (medium): " << thumbnails["medium"].url << std::endl;
    }

    void YoutubeChannel::addError(const std::string &t_errorMsg, nlohmann::json t_json) {
        std::map<std::string, nlohmann::json> _;
        _[t_errorMsg] = t_json;

        m_errors.push_back(_);
    }

    void YoutubeChannel::addWarning(const std::string &t_warningMsg, nlohmann::json t_json) {
        std::map<std::string, nlohmann::json> _;
        _[t_warningMsg] = t_json;

        m_warnings.push_back(_);
    }

    void YoutubeChannel::reportProblems(std::map<std::string, std::string> &t_problems) {
        if (!t_problems.empty()) {
            addWarning(t_problems["warning"]);
            addError(t_problems["error"]);
        }
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
                if (withJson and !map.second.empty()) {
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
                if (withJson and !map.second.empty()) {
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

    void YoutubeChannel::setHasFavouritesPlaylist(bool t_bool) {
        m_hasFavouritesPlaylist = t_bool;
    }

    bool YoutubeChannel::hasUploadsPlaylist() {
        return m_hasUploadsPlaylist;
    }

    void YoutubeChannel::setHasUploadsPlaylist(bool t_bool) {
        m_hasUploadsPlaylist = t_bool;
    }

    bool YoutubeChannel::hasLikesPlaylist() {
        return m_hasLikesPlaylist;
    }

    void YoutubeChannel::setHasLikesPlaylist(bool t_bool) {
        m_hasLikesPlaylist = t_bool;
    }

    bool YoutubeChannel::hasSubscribedLocalOverride() {
        return m_subscribedLocalOverride;
    }

    void YoutubeChannel::setHasSubscribedLocalOverride(bool t_bool) {
        m_subscribedLocalOverride = t_bool;
    }

    bool YoutubeChannel::isSubscribedOnYoutube() {
        return m_subscribedOnYoutube;
    }

    void YoutubeChannel::setIsSubscribedOnYoutube(bool t_bool) {
        m_subscribedOnYoutube = t_bool;
    }
} // namespace sane