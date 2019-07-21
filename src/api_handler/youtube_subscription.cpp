#include <iostream>

// 3rd party libraries.
#include <nlohmann/json.hpp>

#include "youtube_subscription.hpp"

#define GET_VARIABLE_NAME(Variable) (#Variable)

using json = nlohmann::json;
using namespace std;

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
    void YoutubeSubscription::assignJsonStringValue(string &stringToAssignValue,
            const json &unknownJsonTypeValue, const json &fullJson) {
        if (unknownJsonTypeValue.is_null()) {
            if (reportWarnings) {
                cerr << "WARNING: YoutubeSubscription::addFromJson." << GET_VARIABLE_NAME(stringToAssignValue) <<
                     " is NULL not string, setting \"N/A\" string in its stead!" << endl;
            }
            stringToAssignValue = "N/A";
            warningCount++;
        }
        else if (!unknownJsonTypeValue.is_string()) {
            if (reportWarnings) {
                cerr << "WARNING: YoutubeSubscription::addFromJson.favouritesPlaylist is "
                     << unknownJsonTypeValue.type_name() << " not string, setting \"N/A\" string in its stead!" << endl;
            }
            stringToAssignValue = "N/A";
            cerr << fullJson.dump(4);
            warningCount++;
        } else {
            // If it actually is a string, then explicitly cast it.
            stringToAssignValue = unknownJsonTypeValue.get<string>();
        }
    }

    void YoutubeSubscription::addFromJson(json t_data) {
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
                cerr << "ERROR: Skipping YoutubeSubscription::addFromJson "
                        "due to Exception: " << exc.what() << endl;
                cerr << t_data.dump(4);
            }
        } catch (const exception &exc) {
            if (reportErrors) {
                cerr << "ERROR: Skipping YoutubeSubscription::addFromJson "
                        "due to Unexpected Exception: " << exc.what() << endl;
                cerr << t_data.dump(4);
            }
        }
    }


    void YoutubeSubscription::print(int indentationLevel = 0) {
        string indentation;

        for (int i = 0; i <= indentationLevel; ++i) {
            indentation += "\t";
        }

        cout << indentation << "Title: " << title << endl;
        cout << indentation << "Subscription ID: " << subscriptionId << endl;
        cout << indentation << "Channel ID: " << channelId << endl;
        cout << indentation << "Etag: " << etag << endl;
        cout << indentation << "Published: " << publishedAt << endl;
        cout << indentation << "Description: " << description << endl;
        cout << indentation << "Favourites Playlist: " << favouritesPlaylist << endl;
        cout << indentation << "Uploads Playlist: " << uploadsPlaylist << endl;
        cout << indentation << "Thumbnail URL (default): " << thumbnails["default"] << endl;
        cout << indentation << "Thumbnail URL (high): " << thumbnails["high"] << endl;
        cout << indentation << "Thumbnail URL (medium): " << thumbnails["medium"] << endl;
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