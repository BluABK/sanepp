#ifndef SANE_YOUTUBE_SUBSCRIPTION_HPP
#define SANE_YOUTUBE_SUBSCRIPTION_HPP

#include <map>

// 3rd party libraries.
#include <nlohmann/json.hpp>

namespace sane {
    class YoutubeSubscription {
    public:
        void assignJsonStringValue(std::string &stringToAssignValue,
                                   const nlohmann::json &unknownJsonTypeValue, const nlohmann::json &fullJson);

        void addFromJson(nlohmann::json t_data);

        void print(int indentationLevel);

        int getErrorCount();

        int getWarningCount();

        void enableWarnings(bool b);

        void enableErrors(bool b);

        bool wasAborted();

    private:
        // Relevant JSON response values.

        // Important playlists.
        std::string favouritesPlaylist;
        std::string uploadsPlaylist;

        // YouTube resource Etag
        std::string etag;

        // The ID that YouTube uses to uniquely identify the subscription.
        std::string subscriptionId;

        // The value that YouTube uses to uniquely identify the channel that the user subscribed to.
        // NB: Grab the one inside resourceId, the outer one is *yours*.
        std::string channelId;

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

#endif //SANE_YOUTUBE_SUBSCRIPTION_HPP
