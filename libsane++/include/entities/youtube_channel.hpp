#ifndef SANE_YOUTUBE_CHANNEL_HPP
#define SANE_YOUTUBE_CHANNEL_HPP

#include <map>
#include <list>

// 3rd party libraries.
#include <nlohmann/json.hpp>
#include <log_handler/log_handler.hpp>

#include <entities/common.hpp>

namespace sane {
    /**
     * Notes:
       channel id and uploads playlist id are identical if you remove their static prefixes:

       channel = UC, uploads playlist = UU, favourites playlist = FL
     */
    class YoutubeChannel {
    public:
        // Create an empty instance, to be populated later.
        explicit YoutubeChannel();

        // Create an instance and feed it values through a JSON.
        explicit YoutubeChannel(nlohmann::json &t_data) {
            addFromJson(t_data);
        }

        // Create an instance and feed it values through a map of strings.
        explicit YoutubeChannel(std::map<std::string, std::string> &t_map) {
            addFromMap(t_map);
        }

        // Create an instance and feed it values through a map of unsigned chars.
        explicit YoutubeChannel(std::map<std::string, const unsigned char*> &t_map) {
            addFromMap(t_map);
        }

        YoutubeChannel(const char* t_id, const char* t_uploadsPlaylist, const char* t_favouritesPlaylist,
                       const char* t_likesPlaylist, const char* t_title,
                       const char* t_description,
                       const char* t_thumbnailDefault, const char* t_thumbnailHigh,
                       const char* t_thumbnailMedium, bool t_subscribedOnYoutube, bool t_subscribedLocalOverride) {
            addFromValues(t_id, t_uploadsPlaylist, t_favouritesPlaylist, t_likesPlaylist, t_title, t_description,
                          t_thumbnailDefault, t_thumbnailHigh, t_thumbnailMedium, t_subscribedOnYoutube,
                          t_subscribedLocalOverride);
        }

        YoutubeChannel(const char* t_id, bool t_hasUploadsPlaylist, bool t_hasFavouritesPlaylist,
                       bool t_hasLikesPlaylist, const char* t_title,
                       const char* t_description, const char* t_thumbnailDefault,
                       const char* t_thumbnailHigh, const char* t_thumbnailMedium, bool t_subscribedOnYoutube,
                       bool t_subscribedLocalOverride) {
            addFromValues(t_id, t_hasUploadsPlaylist, t_hasFavouritesPlaylist, t_hasLikesPlaylist, t_title,
                          t_description, t_thumbnailDefault, t_thumbnailHigh, t_thumbnailMedium, t_subscribedOnYoutube,
                          t_subscribedLocalOverride);
        }

        YoutubeChannel(const std::string &t_id, bool t_hasUploadsPlaylist,
                       bool t_hasFavouritesPlaylist, bool t_hasLikesPlaylist,
                       const std::string &t_title, const std::string &t_description,
                       const std::string &t_thumbnailDefault, const std::string &t_thumbnailHigh,
                       const std::string &t_thumbnailMedium, bool t_subscribedOnYoutube,
                       bool t_subscribedLocalOverride) {
            addFromValues(t_id, t_hasUploadsPlaylist, t_hasFavouritesPlaylist, t_hasLikesPlaylist, t_title,
                          t_description, t_thumbnailDefault, t_thumbnailHigh, t_thumbnailMedium, t_subscribedOnYoutube,
                          t_subscribedLocalOverride);
        }

        void setupLogger(const std::string &t_facility = {});

        void addFromJson(nlohmann::json t_json);

        void addFromValues(const char* t_id, const char* t_uploadsPlaylist,
                           const char* t_favouritesPlaylist, const char* t_likesPlaylist,
                           const char* t_title, const char* t_description,
                           const char* t_thumbnailDefault, const char* t_thumbnailHigh,
                           const char* t_thumbnailMedium, bool t_subscribedOnYoutube,
                           bool t_subscribedLocalOverride);

        void addFromValues(const char* t_id, bool t_hasUploadsPlaylist, bool t_hasFavouritesPlaylist,
                           bool t_hasLikesPlaylist, const char* t_title,
                           const char* t_description, const char* t_thumbnailDefault,
                           const char* t_thumbnailHigh, const char* t_thumbnailMedium, bool t_subscribedOnYoutube,
                           bool t_subscribedLocalOverride);

        void addFromValues(const std::string &t_id, bool t_hasUploadsPlaylist,
                            bool t_hasFavouritesPlaylist, bool t_hasLikesPlaylist,
                            const std::string &t_title, const std::string &t_description,
                            const std::string &t_thumbnailDefault, const std::string &t_thumbnailHigh,
                            const std::string &t_thumbnailMedium, bool t_subscribedOnYoutube,
                            bool t_subscribedLocalOverride);

        void addFromMap(std::map<std::string, std::string>& t_map);

        void logSqliteMap(std::map<std::string, const unsigned char*> &t_map);

        void addFromMap(std::map<std::string, const unsigned char*>& t_map);

        void print(int indentationSpacing);

        // Each error/warning map has error message as key and JSON as value.
        void addError(const std::string &t_errorMsg, nlohmann::json t_json = nlohmann::json::object());

        void addWarning(const std::string &t_warningMsg, nlohmann::json t_json = nlohmann::json::object());

        void reportProblems(std::map<std::string, std::string> &t_problems);

        std::list<std::map<std::string, nlohmann::json>> getErrors();

        void printErrors(int indent = 0, bool withJson = false, int jsonIndent = 0);

        void logErrors(int jsonIndent = 0);

        std::list<std::map<std::string, nlohmann::json>> getWarnings();

        void printWarnings(int indent = 0, bool withJson = false, int jsonIndent = 0);

        void logWarnings(int jsonIndent = 0);

        void clearWarnings();

        void clearErrors();

        void clearErrorsAndWarnings();

        bool wasAborted();

        const std::string getUploadsPlaylist();

        const std::string getLikesPlaylist();

        const std::string getFavouritesPlaylist();

        const std::string &getId() const;

        void setId(const std::string &t_id);

        void setId(nlohmann::json t_id);

        const std::string getChannelId();

        const std::string &getDescription() const;

        void setDescription(const std::string &t_description);

        void setDescription(nlohmann::json &t_description);

        const std::string &getPublishedAt() const;

        void setPublishedAt(const std::string &t_publishedAt);

        void setPublishedAt(nlohmann::json &t_publishedAt);

        const std::map<std::string, thumbnail_t> &getThumbnails() const;

        void setThumbnails(const std::map<std::string, thumbnail_t> &t_thumbnails);

        void setThumbnails(nlohmann::json &t_thumbnails);

        const std::string &getTitle() const;

        void setTitle(const std::string &t_title);

        void setTitle(nlohmann::json &t_title);

        bool hasFavouritesPlaylist();

        void setHasFavouritesPlaylist(bool t_bool);

        bool hasUploadsPlaylist();

        void setHasUploadsPlaylist(bool t_bool);

        bool hasLikesPlaylist();

        void setHasLikesPlaylist(bool t_bool);

        bool hasSubscribedLocalOverride();

        void setHasSubscribedLocalOverride(bool t_bool);

        bool isSubscribedOnYoutube();

        void setIsSubscribedOnYoutube(bool t_bool);
    private:
        std::shared_ptr<spdlog::logger> log;

        // Relevant JSON response values.

        // The value that YouTube uses to uniquely identify the channel that the user subscribed to.
        // NB: Grab the one inside resourceId, the outer one is *yours*.
        std::string m_id;

        // Playlists (only need bool as they are supersets of ID)
        bool m_hasFavouritesPlaylist = false;
        bool m_hasUploadsPlaylist = false;
        bool m_hasLikesPlaylist = false;

        // Other bools
        bool m_subscribedOnYoutube = false;
        bool m_subscribedLocalOverride = false;

        // The subscription's details.
        std::string m_description;

        // The date and time that the subscription was created.
        // The value is specified in ISO 8601 (YYYY-MM-DDThh:mm:ss.sZ) format.
        std::string m_publishedAt;

        // A map of thumbnail images associated with the subscription.
        // Should in most cases contain keys: "default", "high" and "medium".
        std::map<std::string, thumbnail_t> m_thumbnails;

        // Subscription/Channel title.
        std::string m_title;

        // Track errors that occur
        std::list<std::map<std::string, nlohmann::json>> m_errors;

        // Track warnings that occur
        std::list<std::map<std::string, nlohmann::json>> m_warnings;

        // Indicate whether the operation was aborted
        bool m_aborted = false;
    };
} // namespace sane

#endif //SANE_YOUTUBE_CHANNEL_HPP
