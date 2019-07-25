#ifndef SANE_YOUTUBE_CHANNEL_HPP
#define SANE_YOUTUBE_CHANNEL_HPP

#include <map>

// 3rd party libraries.
#include <nlohmann/json.hpp>

namespace sane {
    /**
     * Notes:
       channel id and uploads playlist id are identical if you remove their static prefixes:

       channel = UC, uploads playlist = UU, favourites playlist = FL
     */
    class YoutubeChannel {
    public:
        void assignJsonStringValue(std::string &stringToAssignValue,
                                   const nlohmann::json &unknownJsonTypeValue, const nlohmann::json &fullJson);

        void addFromJson(nlohmann::json t_data);

        void print(int indentationSpacing);

        int getErrorCount();

        int getWarningCount();

        void enableWarnings(bool b);

        void enableErrors(bool b);

        bool wasAborted();

        const std::string getUploadsPlaylist();

        const std::string getLikesPlaylist();

        const std::string getFavouritesPlaylist();

        const std::string getId();

        const std::string getChannelId();

        const std::string getDescription();

        const std::string getPublishedAt();

        const std::map<std::string, std::string> getThumbnails();

        const std::string getThumbnailDefault();

        const std::string getThumbnailHigh();

        const std::string getThumbnailMedium();

        const std::string getTitle();

        bool hasFavouritesPlaylist();

        bool hasUploadsPlaylist();

        bool hasLikesPlaylist();
    private:
        const std::string MISSING_VALUE = "N/A";
        // Relevant JSON response values.

        // The value that YouTube uses to uniquely identify the channel that the user subscribed to.
        // NB: Grab the one inside resourceId, the outer one is *yours*.
        std::string id;

        // Playlists (only need bool as they are supersets of ID)
        bool m_hasFavouritesPlaylist = false;
        bool m_hasUploadsPlaylist = false;
        bool m_hasLikesPlaylist = false;

        // The subscription's details.
        std::string description;

        // The date and time that the subscription was created.
        // The value is specified in ISO 8601 (YYYY-MM-DDThh:mm:ss.sZ) format.
        std::string publishedAt;

        // A map of thumbnail images associated with the subscription.
        // Should in most cases contain keys: "default", "high" and "medium".
        std::map<std::string, std::string> thumbnails;

        // Subscription/Channel title.
        std::string title;

        // Track amount of errors that occur
        int errorCount;

        // Track amount of warnings that occur
        int warningCount;

        // Whether or not to print warnings
        bool reportWarnings = false;

        // Whether or not to print warnings
        bool reportErrors = true;

        // Indicate whether the operation was aborted
        bool aborted = false;

    };

} // namespace sane

#endif //SANE_YOUTUBE_CHANNEL_HPP
