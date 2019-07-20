#ifndef SANE_YOUTUBE_SUBSCRIPTION_HPP
#define SANE_YOUTUBE_SUBSCRIPTION_HPP

#include <map>

// 3rd party libraries.
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class YoutubeSubscription {
public:
//    explicit YoutubeSubscription(const std::shared_ptr<YoutubeSubscription>& sharedPtr);

    void addFromJson(json t_data);

//    YoutubeSubscription get();

    void print(int indentationLevel);
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
    std::map<std::string,std::string> thumbnails;

    // Subscription/Channel title.
    std::string title;
};


#endif //SANE_YOUTUBE_SUBSCRIPTION_HPP
