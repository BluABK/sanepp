#include <iostream>

// 3rd party libraries.
#include <nlohmann/json.hpp>

#include "youtube_subscription.hpp"

using json = nlohmann::json;

//bool jsonIsValid() {
//
//}

void YoutubeSubscription::addFromJson(json t_data) {
    std::cout << t_data.dump(4);
//    if (true) {
//        std::cout << "ERROR: Empty JSON given, skipping add!";
//        std::cout << t_data["contentDetails"];
//        std::cout << t_data["contentDetails"]["favorites"];
//    }
    try {
        // Relevant JSON response values. See header for explanations.
        favouritesPlaylist = t_data["contentDetails"]["relatedPlaylists"]["favorites"].get<std::string>();
        uploadsPlaylist = t_data["contentDetails"]["relatedPlaylists"]["uploads"].get<std::string>();
        etag = t_data["etag"].get<std::string>();
        subscriptionId = t_data["id"].get<std::string>();
        // NB: Grab the one inside resourceId, the outer one is *yours*.
        channelId = t_data["snippet"]["resourceId"]["channelId"].get<std::string>();
        description = t_data["snippet"]["description"].get<std::string>();
        publishedAt = t_data["snippet"]["publishedAt"].get<std::string>();
        thumbnails["default"] = t_data["snippet"]["thumbnails"]["default"]["url"].get<std::string>();
        thumbnails["high"] = t_data["snippet"]["thumbnails"]["high"]["url"].get<std::string>();
        thumbnails["medium"] = t_data["snippet"]["thumbnails"]["medium"]["url"].get<std::string>();
        title = t_data["snippet"]["title"].get<std::string>();
    } catch (nlohmann::detail::type_error &exc) {
        std::cerr << "WARNING: Skipping YoutubeSubscription::addFromJson due to Exception: " << exc.what() << std::endl;
    } catch (const std::exception &exc) {
        std::cerr << "WARNING: Skipping YoutubeSubscription::addFromJson due to Unexpected Exception: " << exc.what() << std::endl;
    }
}

//YoutubeSubscription YoutubeSubscription::get() {
//    return YoutubeSubscription();
//}

void YoutubeSubscription::print(int indentationLevel = 0) {
    std::string indentation;

    for (int i = 0; i <= indentationLevel; ++i) {
        indentation += "\t";
    }

    std::cout << indentation << "Title: " << title << std::endl;
    std::cout << indentation << "Subscription ID: " << subscriptionId << std::endl;
    std::cout << indentation << "Channel ID: " << channelId << std::endl;
    std::cout << indentation << "Etag: " << etag << std::endl;
    std::cout << indentation << "Published: " << publishedAt << std::endl;
    std::cout << indentation << "Description: " << description << std::endl;
    std::cout << indentation << "Favourites Playlist: " << favouritesPlaylist << std::endl;
    std::cout << indentation << "Uploads Playlist: " << uploadsPlaylist << std::endl;
    std::cout << indentation << "Thumbnail URL (default): " << thumbnails["default"] << std::endl;
    std::cout << indentation << "Thumbnail URL (high): " << thumbnails["high"] << std::endl;
    std::cout << indentation << "Thumbnail URL (medium): " << thumbnails["medium"] << std::endl;
}

//YoutubeSubscription::YoutubeSubscription(const std::shared_ptr<YoutubeSubscription>& sharedPtr) {
//
//}
