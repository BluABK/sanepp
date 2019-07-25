#include <iostream>
#include <vector>
#include <list>
#include <entities/youtube_channel.hpp>

#include "db_handler.hpp"
#include "youtube_channels.hpp"

namespace sane {
    /**
     * Checks if a given string is valid for SQLite3 query, if not return "NULL".
     *
     * @param t_string  String to be validated.
     * @return          Input string wrapped in single-quotes or "NULL".
     */
    std::string validateSQLiteInput(const std::string &t_string) {
        if (t_string == "N/A" or t_string.empty()) {
            return "NULL";
        } else {
            // Wrap the string in single-quotes.
            return "'" + t_string + "'";
        }
    }
    /**
     * Compiles a list of VALID* values into SQLite3 VALUES(...) form.
     *
     * Example:
     *          In: {"This","is","an", "example"}
     *          Out "This, is an"
     *
     * * Remember to validate first, validation is not implied because of
     *   edge cases like a TRUE/FALSE BOOLEAN, INTEGERS and things that
     *   shouldn't be wrapped in single-quotes.
     *
     * @param listOfValues
     * @return
     */
    std::string compileValues(const std::list<std::string>& listOfValues) {
        std::string compiledString;
        unsigned long index = 0;

        for (auto & value : listOfValues) {
            compiledString += value;

            // Append separator as long as it isn't the last item in the list.
            if (index != listOfValues.size() -1) {
                compiledString += ", ";
            }
        }

        return compiledString;
    }

    int addChannelsToDB(std::list <std::shared_ptr<YoutubeChannel>> &subscriptions) {
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

        returnStatus = sane::execSqlStatementNoCallback(databaseFilename, sqlStatement);
        if (returnStatus != SQLITE_OK) {
            std::cerr << "sane::execSqlStatement(" << databaseFilename << ", " <<
            sqlStatement << ") ERROR: returned non-zero status: " << returnStatus << std::endl;
            return returnStatus;
        }

        // Iterate through the subscription objects and add relevant fields to DB.
        int counter = 0;
        for (auto & subscription : subscriptions) {
            // Figure out and sanitize the values.
            std::string id = validateSQLiteInput(subscription->getId());
            std::string title = validateSQLiteInput(subscription->getTitle());
            std::string description = validateSQLiteInput(subscription->getDescription());

            std::map<std::string, std::string> thumbnails_ = subscription->getThumbnails();
            std::string thumbnailDefault = validateSQLiteInput(thumbnails_["default"]);
            std::string thumbnailHigh = validateSQLiteInput(thumbnails_["high"]);
            std::string thumbnailMedium = validateSQLiteInput(thumbnails_["medium"]);

            std::string subscribedOnYouTube = "TRUE";
            std::string subscribedLocalOverride = "FALSE";

            // Strip out non-unique prefix from ID (2 chars)
            id = id.substr(1);

            const std::list<std::string> valuesToCompile = {id, title, description, thumbnailDefault, thumbnailHigh,
                                                            thumbnailMedium, subscribedOnYouTube,
                                                            subscribedLocalOverride};

            const std::string insertValues = compileValues(valuesToCompile) ;

            // Construct the SQL statement.
            sqlStatement = std::string("INSERT INTO youtube_channels ("
                           "ID, Title, Description, ThumbnailDefault, ThumbnailHigh, ThumbnailMedium, "
                           "SubscribedOnYouTube, SubscribedLocalOverride"
                           ") VALUES (") + insertValues + std::string(");");

            std::cout << "Sub #" << counter << ": " << sqlStatement << std::endl;

            returnStatus = sane::execSqlStatementNoCallback(databaseFilename, sqlStatement);
            if (returnStatus != SQLITE_OK) {
                std::cerr << "sane::execSqlStatement(" << databaseFilename << ", " <<
                          sqlStatement << ") ERROR: returned non-zero status: " << returnStatus << std::endl;
                return returnStatus;
            }

            counter++;
        }

        return 0;
    }


} // namespace sane