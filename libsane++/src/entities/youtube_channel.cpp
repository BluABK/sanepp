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
        // Set up a generic logger to log errors and exceptions (as this is pre-ID stage).
        setupLogger("YoutubeChannel");

        try {
            if (t_json.find("kind") != t_json.end()) {
                if (t_json["kind"].get<std::string>() == "youtube#channel") {
                    setId(t_json["id"]);
                } else if (t_json["kind"].get<std::string>() == "youtube#subscription") {
                    setId(t_json["snippet"]["resourceId"]["channelId"]);
                } else {
                    log->error("addFromJson: given channel resource has invalid kind!\n"
                             + t_json.dump(4));
                }
            } else {
                log->error("YoutubeChannel::addFromJson Error: given channel resource no kind!\n"
                         + t_json.dump(4));
            }

            // If channelId was valid, strip non-unique channel prefix from its head to produce the actual ID.
            if (!getId().empty()) {
                m_id = getId().substr(2);
                // Set up a logger suffixed with the channel's ID.
                setupLogger("YoutubeChannel:" + getId());
                log->debug("Created from JSON.");
            } else {
                // Set up a logger suffixed with MissingID.
                setupLogger("YoutubeChannel:MissingID");
                addError("Missing Channel ID!", t_json); // FIXME: is addError and addWarning still needed?
                log->error("Missing Channel ID:\n" + t_json.dump(4));
            }

            // Playlists
            nlohmann::json relatedPlaylists = t_json["contentDetails"]["relatedPlaylists"];
            log->debug("relatedPlaylists: " + t_json["contentDetails"]["relatedPlaylists"].dump());
            setHasFavouritesPlaylist(relatedPlaylists["favorites"].is_string());
            log->debug("hasFavouritesPlaylist: " + (hasFavouritesPlaylist() ? std::string("Yes.") : std::string("No.")));
            setHasUploadsPlaylist(relatedPlaylists["uploads"].is_string());
            log->debug("hasUploadsPlaylist: " + (hasUploadsPlaylist() ? std::string("Yes.") : std::string("No.")));
            setHasLikesPlaylist(relatedPlaylists["likes"].is_string());
            log->debug("hasLikesPlaylist: " + (hasLikesPlaylist() ? std::string("Yes.") : std::string("No.")));

            setDescription(t_json["snippet"]["description"]);
            log->debug("Description: " + getDescription());
            setPublishedAt(t_json["snippet"]["publishedAt"]);
            log->debug("Published at: " + getPublishedAt());
            setThumbnails(t_json["snippet"]["thumbnails"]);
            log->debug("Thumbnails: " + t_json["snippet"]["thumbnails"].dump());
            setTitle(t_json["snippet"]["title"]);
            log->debug("Title: " + getTitle());
        } catch (nlohmann::detail::type_error &exc) {
            addError("Skipping YoutubeChannel::addFromJson due to Exception: " + std::string(exc.what()), t_json);
            log->error("Skipping YoutubeChannel::addFromJson due to Exception: "
                     + std::string(exc.what()) + "\n"
                     + t_json.dump(4));
        } catch (const std::exception &exc) {
            addError("Skipping YoutubeChannel::addFromJson due to Unexpected Exception: " +
            std::string(exc.what()), t_json);
            log->error("Skipping YoutubeChannel::addFromJson due to Unexpected Exception: "
                     + std::string(exc.what()) + "\n"
                     + t_json.dump(4));
        }
    }

    void YoutubeChannel::addFromValues(const char* t_id, const char* t_uploadsPlaylist,
                                       const char* t_favouritesPlaylist, const char* t_likesPlaylist,
                                       const char* t_title, const char* t_description,
                                       const char* t_thumbnailDefault, const char* t_thumbnailHigh,
                                       const char* t_thumbnailMedium, bool t_subscribedOnYoutube,
                                       bool t_subscribedLocalOverride) {
        // Set up a logger suffixed with the channel's ID.
        setupLogger("YoutubeChannel:" + std::string(t_id));
        log->debug("Created from values (type 1).");
        log->debug("uploadsPlaylist: " + std::string(t_uploadsPlaylist));
        log->debug("favouritesPlaylist: " + std::string(t_favouritesPlaylist));
        log->debug("likesPlaylist: " + std::string(t_likesPlaylist));
        log->debug("Title: " + std::string(t_title));
        log->debug("Description: " + std::string(t_description));
        log->debug("Thumbnail (Default): " + std::string(t_thumbnailDefault));
        log->debug("Thumbnail (High): " + std::string(t_thumbnailHigh));
        log->debug("Thumbnail (Medium): " + std::string(t_thumbnailMedium));
        log->debug("subscribedOnYoutube: " + (t_subscribedOnYoutube ? std::string("Yes.") : std::string("No.")));
        log->debug("subscribedLocalOverride: "
                 + (t_subscribedLocalOverride ? std::string("Yes.") : std::string("No.")));

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
        // Set up a logger suffixed with the channel's ID.
        setupLogger("YoutubeChannel:" + std::string(t_id));
        log->debug("Created from values (type 2).");
        log->debug("hasUploadsPlaylist: " + (t_hasUploadsPlaylist ? std::string("Yes.") : std::string("No.")));
        log->debug("hasFavouritesPlaylist: " + (t_hasFavouritesPlaylist ? std::string("Yes.") : std::string("No.")));
        log->debug("hasLikesPlaylist: " + (t_hasLikesPlaylist ? std::string("Yes.") : std::string("No.")));
        log->debug("Title: " + std::string(t_title));
        log->debug("Description: " + std::string(t_description));
        log->debug("Thumbnail (Default): " + std::string(t_thumbnailDefault));
        log->debug("Thumbnail (High): " + std::string(t_thumbnailHigh));
        log->debug("Thumbnail (Medium): " + std::string(t_thumbnailMedium));
        log->debug("subscribedOnYoutube: " + (t_subscribedOnYoutube ? std::string("Yes.") : std::string("No.")));
        log->debug("subscribedLocalOverride: "
                   + (t_subscribedLocalOverride ? std::string("Yes.") : std::string("No.")));

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
        // Set up a logger suffixed with the channel's ID.
        setupLogger("YoutubeChannel:" + t_id);
        log->debug("Created from values (type 3).");
        log->debug("hasUploadsPlaylist: " + (t_hasUploadsPlaylist ? std::string("Yes.") : std::string("No.")));
        log->debug("hasFavouritesPlaylist: " + (t_hasFavouritesPlaylist ? std::string("Yes.") : std::string("No.")));
        log->debug("hasLikesPlaylist: " + (t_hasLikesPlaylist ? std::string("Yes.") : std::string("No.")));
        log->debug("Title: " + t_title);
        log->debug("Description: " + t_description);
        log->debug("Thumbnail (Default): " + t_thumbnailDefault);
        log->debug("Thumbnail (High): " + t_thumbnailHigh);
        log->debug("Thumbnail (Medium): " + t_thumbnailMedium);
        log->debug("subscribedOnYoutube: " + (t_subscribedOnYoutube ? std::string("Yes.") : std::string("No.")));
        log->debug("subscribedLocalOverride: "
                   + (t_subscribedLocalOverride ? std::string("Yes.") : std::string("No.")));

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

    void YoutubeChannel::logSqliteMap(std::map<std::string, const unsigned char*> &t_map) {
        // Set up a logger suffixed with the channel's ID.
        setupLogger("YoutubeChannel:" + std::string(reinterpret_cast<const char *>(t_map["ID"])));
        log->debug("Created from map (SQLite UTF-8).");

        log->debug("Title: " + std::string((const char *)t_map["Title"]));
        log->debug("hasUploadsPlaylist: "
                   + (!std::string(reinterpret_cast<const char *>(t_map["UploadsPlaylist"])).empty()
                      ? std::string("Yes.") : std::string("No.")));
        log->debug("hasFavouritesPlaylist: "
                   + (!std::string(reinterpret_cast<const char *>(t_map["FavouritesPlaylist"])).empty()
                      ? std::string("Yes.") : std::string("No.")));
        log->debug("hasLikesPlaylist: "
                   + (!std::string(reinterpret_cast<const char *>(t_map["LikesPlaylist"])).empty()
                      ? std::string("Yes.") : std::string("No.")));
        log->debug("Description: " + std::string((const char *)t_map["Description"]));
        log->debug("Thumbnail (Default): " + std::string(reinterpret_cast<const char *>(t_map["ThumbnailDefault"])));
        log->debug("Thumbnail (High): " + std::string(reinterpret_cast<const char *>(t_map["ThumbnailHigh"])));
        log->debug("Thumbnail (Medium): " + std::string(reinterpret_cast<const char *>(t_map["ThumbnailMedium"])));
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
        // Setup logger and log given map in a separate function (tidyness).
        logSqliteMap(t_map);

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
        log->info("Setting ID: " + t_id + ".");
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
        log->info("Setting description: " + t_description + ".");
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
        log->info("Setting Published At: " + t_publishedAt + ".");
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
        log->info("Setting Thumbnails from map.");
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
        log->info("Setting Thumbnail (Default): " + defaultThumbnail.url + ".");
        thumbnails["high"] = highThumbnail;
        log->info("Setting Thumbnail (High): " + highThumbnail.url + ".");
        thumbnails["medium"] = mediumThumbnail;
        log->info("Setting Thumbnail (Medium): " + mediumThumbnail.url + ".");

        // Finally assign the thumbnails map to the object property.
        setThumbnails(thumbnails);
    }

    const std::string &YoutubeChannel::getTitle() const {
        return m_title;
    }

    void YoutubeChannel::setTitle(const std::string &t_title) {
        log->info("Setting title: " + t_title + ".");
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
            log->warn("Problem: " + t_problems["warning"] + ".");
            addWarning(t_problems["warning"]);
            log->error("Problem: " + t_problems["error"] + "!");
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

    void YoutubeChannel::logErrors(int jsonIndent) {
        // For map in list
        for (auto const& item : getErrors()) {
            // For message, JSON in map
            for (auto const& map : item) {
                log->error(map.first);
                if (!map.second.empty()) {
                    log->debug(map.second.dump(jsonIndent));
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

    void YoutubeChannel::logWarnings(int jsonIndent) {
        // For map in list
        for (auto const& item : getWarnings()) {
            // For message, JSON in map
            for (auto const& map : item) {
                log->warn(map.first);
                if (!map.second.empty()) {
                    log->debug(map.second.dump(jsonIndent));
                }
            }
        }
    }

    bool YoutubeChannel::wasAborted() {
        return m_aborted;
    }

    void YoutubeChannel::clearWarnings() {
        log->trace("Clearing stored warnings.");
        m_warnings.clear();
    }

    void YoutubeChannel::clearErrors() {
        log->trace("Clearing stored errors.");
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
        log->debug("Setting hasFavouritesPlaylist: " + (t_bool ? std::string("Yes.") : std::string("No.")));
        m_hasFavouritesPlaylist = t_bool;
    }

    bool YoutubeChannel::hasUploadsPlaylist() {
        return m_hasUploadsPlaylist;
    }

    void YoutubeChannel::setHasUploadsPlaylist(bool t_bool) {
        log->debug("Setting hasUploadsPlaylist: " + (t_bool ? std::string("Yes.") : std::string("No.")));
        m_hasUploadsPlaylist = t_bool;
    }

    bool YoutubeChannel::hasLikesPlaylist() {
        return m_hasLikesPlaylist;
    }

    void YoutubeChannel::setHasLikesPlaylist(bool t_bool) {
        log->debug("Setting hasLikesPlaylist: " + (t_bool ? std::string("Yes.") : std::string("No.")));
        m_hasLikesPlaylist = t_bool;
    }

    bool YoutubeChannel::hasSubscribedLocalOverride() {
        return m_subscribedLocalOverride;
    }

    void YoutubeChannel::setHasSubscribedLocalOverride(bool t_bool) {
        log->debug("Setting subscribedLocalOverride: " + (t_bool ? std::string("Yes.") : std::string("No.")));
        m_subscribedLocalOverride = t_bool;
    }

    bool YoutubeChannel::isSubscribedOnYoutube() {
        return m_subscribedOnYoutube;
    }

    void YoutubeChannel::setIsSubscribedOnYoutube(bool t_bool) {
        log->debug("Setting subscribedOnYoutube: " + (t_bool ? std::string("Yes.") : std::string("No.")));
        m_subscribedOnYoutube = t_bool;
    }

    void YoutubeChannel::setupLogger(const std::string &t_facility) {
        // Set up logger
        std::shared_ptr<sane::LogHandler> logHandler = std::make_shared<sane::LogHandler>();
        log = logHandler->createLogger(t_facility);
    }
} // namespace sane