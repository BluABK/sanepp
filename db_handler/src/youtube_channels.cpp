#include <iostream>
#include <vector>
#include <list>
#include <src/youtube_subscription.hpp>

#include "db_handler.hpp"
#include "youtube_channels.hpp"

namespace sane {

    std::string validateInput(const std::string &t_string) {
        if (t_string == "N/A" or t_string.empty()) {
            return "NULL";
        } else {
            return t_string;
        }
    }

    std::string compileValues(std::list<std::string> listOfValues) {
        std::string compiledString;
        for (auto & value : listOfValues) {
            // appendd strings with whitespace
        }
    }

    void addChannelsToDB(std::list <std::shared_ptr<YoutubeSubscription>> &subscriptions) {
        // Setup
        const std::string databaseFilename = "sane_db_handler_test.db";
        std::string sqlStatement;
        int returnStatus;

        // Notes:
        // channel id and uploads playlist id are identical if you remove their static prefixes:
        // channel = UC, uploads playlist = UU, favourites playlist = FL

        // Create the table, if it doesn't already exist.
        sqlStatement = "CREATE TABLE IF NOT EXISTS youtube_channels ("
                       "ID INTEGER,"
                       "Title TEXT,"
                       "Description TEXT,"
                       "ThumbnailDefault TEXT,"
                       "ThumbnailHigh TEXT,"
                       "ThumbnailMedium TEXT,"
                       "SubscribedOnYouTube BOOLEAN,"
                       "SubscribedLocalOverride BOOLEAN"
                       ");";
        returnStatus = sane::execSqlStatement(databaseFilename, sqlStatement);

        // Iterate through the subscription objects and add relevant fields to DB.
        for (auto & subscription : subscriptions) {
            // Figure out and sanitize the values.
            std::string id = validateInput(subscription->getChannelId());
            std::string title = validateInput(subscription->getTitle());
            std::string description = validateInput(subscription->getDescription());
            std::map<std::string, std::string> thumbnails = subscription->getThumbnails();
            std::string thumbnailDefault = validateInput(thumbnails["default"]);
            std::string thumbnailHigh = validateInput(thumbnails["high"]);
            std::string thumbnailMedium = validateInput(thumbnails["medium"]);
            std::string subscribedOnYouTube = "TRUE";
            std::string subscribedLocalOverride = "FALSE";

            // Strip out non-unique prefix from ID (2 chars)
            id = id.substr(1);



            // Construct the SQL statement.
            sqlStatement = std::string("INSERT INTO youtube_channels ("
                           "ID, Title, Description, ThumbnailDefault, ThumbnailHigh, ThumbnailMedium, "
                           "SubscribedOnYouTube, SubscribedLocalOverride"
                           ") VALUES (")
                           + subscription->getChannelId()
                             + std::string(");");

            std::cout << "Sub#" << ":" << std::endl;
            subscription->print(4);
        }
    }
} // namespace sane