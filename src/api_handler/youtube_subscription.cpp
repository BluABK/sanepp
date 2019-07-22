#include <iostream>

// 3rd party libraries.
#include <nlohmann/json.hpp>

#include "youtube_subscription.hpp"

#define GET_VARIABLE_NAME(Variable) (#Variable)

namespace sane {
    /**
     * Attempts to assign a JSON value to a given string and handle any bogus data.
     * If the input JSON value isn't a string it sets "N/A".
     *
     * This function passes stringToAssignValue by reference (directly modififying it).
     *
     * @param stringToAssignValue   String to assign a value (modified directly).
     * @param unknownJsonTypeValue  The undetermined JSON value to assign the string.
     * @param fullJson              The complete JSON object, for use in debugging/error reporting.
     */
    void YoutubeSubscription::assignJsonStringValue(std::string &stringToAssignValue,
            const nlohmann::json &unknownJsonTypeValue, const nlohmann::json &fullJson) {
        if (unknownJsonTypeValue.is_null()) {
            if (reportWarnings) {
                std::cerr << "WARNING: YoutubeSubscription::addFromJson." << GET_VARIABLE_NAME(stringToAssignValue) <<
                     " is NULL not string, setting \"N/A\" string in its stead!" << std::endl;
            }
            stringToAssignValue = "N/A";
            warningCount++;
        }
        else if (!unknownJsonTypeValue.is_string()) {
            if (reportWarnings) {
                std::cerr << "WARNING: YoutubeSubscription::addFromJson.favouritesPlaylist is " <<
                unknownJsonTypeValue.type_name() << " not string, setting \"N/A\" string in its stead!" <<
                std::endl;
            }
            stringToAssignValue = "N/A";
            std::cerr << fullJson.dump(4);
            warningCount++;
        } else {
            // If it actually is a string, then explicitly cast it.
            stringToAssignValue = unknownJsonTypeValue.get<std::string>();
        }
    }

    void YoutubeSubscription::addFromJson(nlohmann::json t_data) {
        try {
            // Relevant JSON response values. See header for explanations.
            assignJsonStringValue(favouritesPlaylist, t_data["contentDetails"]["relatedPlaylists"]["favorites"], t_data);
            assignJsonStringValue(uploadsPlaylist, t_data["contentDetails"]["relatedPlaylists"]["uploads"], t_data);
            assignJsonStringValue(etag, t_data["etag"], t_data);
            assignJsonStringValue(subscriptionId, t_data["id"], t_data);
            assignJsonStringValue(channelId, t_data["snippet"]["resourceId"]["channelId"], t_data);
            assignJsonStringValue(description, t_data["snippet"]["description"], t_data);
            assignJsonStringValue(publishedAt, t_data["snippet"]["publishedAt"], t_data);
            assignJsonStringValue(thumbnails["default"], t_data["snippet"]["thumbnails"]["default"]["url"], t_data);
            assignJsonStringValue(thumbnails["high"], t_data["snippet"]["thumbnails"]["high"]["url"], t_data);
            assignJsonStringValue(thumbnails["medium"], t_data["snippet"]["thumbnails"]["medium"]["url"], t_data);
            assignJsonStringValue(title, t_data["snippet"]["title"], t_data);
        } catch (nlohmann::detail::type_error &exc) {
            if (reportErrors) {
                std::cerr << "ERROR: Skipping YoutubeSubscription::addFromJson "
                        "due to Exception: " << exc.what() << std::endl;
                std::cerr << t_data.dump(4);
            }
        } catch (const std::exception &exc) {
            if (reportErrors) {
                std::cerr << "ERROR: Skipping YoutubeSubscription::addFromJson "
                        "due to Unexpected Exception: " << exc.what() << std::endl;
                std::cerr << t_data.dump(4);
            }
        }
    }

    const std::string &YoutubeSubscription::getFavouritesPlaylist() const {
        return favouritesPlaylist;
    }

    const std::string &YoutubeSubscription::getUploadsPlaylist() const {
        return uploadsPlaylist;
    }

    const std::string &YoutubeSubscription::getEtag() const {
        return etag;
    }

    const std::string &YoutubeSubscription::getSubscriptionId() const {
        return subscriptionId;
    }

    const std::string &YoutubeSubscription::getChannelId() const {
        return channelId;
    }

    const std::string &YoutubeSubscription::getDescription() const {
        return description;
    }

    const std::string &YoutubeSubscription::getPublishedAt() const {
        return publishedAt;
    }

    const std::map<std::string, std::string> &YoutubeSubscription::getThumbnails() const {
        return thumbnails;
    }

    const std::string &YoutubeSubscription::getTitle() const {
        return title;
    }


    void YoutubeSubscription::print(int indentationSpacing = 0) {
        std::string indentation(indentationSpacing, ' ');

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

    int YoutubeSubscription::getErrorCount() {
        return errorCount;
    }

    int YoutubeSubscription::getWarningCount() {
        return warningCount;
    }

    bool YoutubeSubscription::wasAborted() {
        return aborted;
    }

    void YoutubeSubscription::enableWarnings(bool b) {
        reportWarnings = b;
    }

    void YoutubeSubscription::enableErrors(bool b) {
        reportErrors = b;
    }
} // namespace sane