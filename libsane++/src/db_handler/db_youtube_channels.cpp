#include <iostream>
#include <vector>
#include <list>
#include <entities/youtube_channel.hpp>

#include <db_handler/db_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>

namespace sane {
    /**
     * Checks if a given string is valid for SQLite3 query, if not return "NULL".
     *
     * @param t_string  String to be validated.
     * @return          Input string or "NULL" converted to SQLite compatible C-String.
     */
    const char* validateSQLiteInput(const char *t_cString) {
        // If t_cString is an empty string, nullptr or equals MISSING_VALUE
        if (strcmp(t_cString, MISSING_VALUE) == 0 or (t_cString && !t_cString[0])) {
            return "NULL";
        } else {
            // Convert the std::string to SQLite3 compatible C-String.
            return t_cString;
        }
    }
    int createTable(const std::shared_ptr<DBHandler> &t_db = nullptr) {
        // Setup
        std::shared_ptr<DBHandler> db;
        sqlite3_stmt *preparedStatement = nullptr;
        std::string sqlStatement;
        int rc;

        if (t_db == nullptr) {
            // Acquire database handle.
            std::cout << "Acquiring DB handle..." << std::endl;
            db = std::make_shared<DBHandler>();
            std::cout << "Acquired DB handle." << std::endl;
        } else {
            db = t_db;
        }

        // Create the table, if it doesn't already exist.
        sqlStatement = "CREATE TABLE IF NOT EXISTS youtube_channels ("
                       "ID TEXT, "
                       "Title TEXT, "
                       "HasUploadsPlaylist BOOLEAN, "
                       "HasFavouritesPlaylist BOOLEAN, "
                       "HasLikesPlaylist BOOLEAN, "
                       "Description TEXT, "
                       "SubscribedOnYouTube BOOLEAN, "
                       "SubscribedLocalOverride BOOLEAN, "
                       "ThumbnailDefault TEXT, "
                       "ThumbnailHigh TEXT, "
                       "ThumbnailMedium TEXT"
                       ")";

        // 1/3: Create a prepared statement
        preparedStatement = db->prepareSqlStatement(sqlStatement);
        if (db->lastStatus() != SQLITE_OK) {
            std::cerr << "create_table(" << db->getDBFilename() << ", " <<
                      sqlStatement << ") ERROR: returned non-zero status: " << db->lastStatus()  << std::endl;
            return db->lastStatus();
        }

        // 2/3: Step through, and do nothing because this is a CREATE statement with no VALUE bindings.
        while ((rc = sqlite3_step(preparedStatement)) == SQLITE_ROW) {}

        // 3/3: Finalize prepared statement
        db->finalizePreparedSqlStatement(rc, preparedStatement);

        return SQLITE_OK;
    }

    int addChannelsToDB(const std::list <std::shared_ptr<YoutubeChannel>>& t_channels) {
        // Setup
        sqlite3_stmt *preparedStatement = nullptr;
        std::string sqlStatement;
        int rc = -1;
        int counter = 0;

        // Acquire database handle.
        std::cout << "Acquiring DB handle..." << std::endl;
        std::shared_ptr<DBHandler> db = std::make_shared<DBHandler>();
        std::cout << "Acquired DB handle." << std::endl;

        // Creates the table if it does not already exist.
        createTable(db);

        // Iterate through the subscription objects and add relevant fields to DB.
        for (auto &channel : t_channels) {
            // Figure out and sanitize the values.
            const char* id = channel->getIdAsCString();
            int hasUploadsPlaylist = channel->hasFavouritesPlaylist() ? 1 : 0;
            int hasFavouritesPlaylist = channel->hasFavouritesPlaylist() ? 1 : 0;
            int hasLikesPlaylist = channel->hasLikesPlaylist() ? 1 : 0;
            const char* title = validateSQLiteInput(channel->getTitleAsCString());
            const char* description = validateSQLiteInput(channel->getDescriptionAsCString());

            const char* thumbnailDefault = validateSQLiteInput(channel->getThumbnailDefaultAsCString());
            const char* thumbnailHigh = validateSQLiteInput(channel->getThumbnailHighAsCString());
            const char* thumbnailMedium = validateSQLiteInput(channel->getThumbnailMediumAsCString());

            // Bool, int? Potato, Potáto to SQLite.
            int subscribedOnYouTube = 1;       // FIXME: Hardcoded True
            int subscribedLocalOverride = 0;  // FIXME: Harcoded False

            // Construct the SQL statement.
            sqlStatement = std::string("INSERT INTO youtube_channels ("
                           "ID, HasUploadsPlaylist, HasFavouritesPlaylist, HasLikesPlaylist, Title, Description, "
                           "ThumbnailDefault, ThumbnailHigh, ThumbnailMedium, SubscribedOnYouTube, "
                           "SubscribedLocalOverride) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

//            std::cout << "Sub #" << counter << " " << channel->getTitle() << ": " << sqlStatement << std::endl;
            std::cout << "Sub #" << counter << " " << channel->getTitle() << ": " <<  // FIXME: Remove debug print
              "INSERT INTO youtube_channels ("
              "ID, HasUploadsPlaylist, HasFavouritesPlaylist, HasLikesPlaylist, Title, Description, "
              "ThumbnailDefault, ThumbnailHigh, ThumbnailMedium, SubscribedOnYouTube, "
              "SubscribedLocalOverride) VALUES (" << id << ", " << hasUploadsPlaylist << ", " << hasFavouritesPlaylist
              << ", " << hasLikesPlaylist << ", " << title << ", " << "<desc>" << ", " << thumbnailDefault
              << ", " << thumbnailHigh << ", " << thumbnailMedium << ", " << subscribedOnYouTube << ", " <<
              subscribedLocalOverride << ")" << std::endl;

            // Create a prepared statement
            preparedStatement = db->prepareSqlStatement(sqlStatement);
            if (db->lastStatus()  != SQLITE_OK) {
                std::cerr << "sane::execSqlStatement(" << db->getDBFilename() << ", " <<
                          sqlStatement << ") ERROR: returned non-zero status: " << db->lastStatus() << std::endl;
                return db->lastStatus();
            }

            //  Bind-parameter indexing is 1-based.
            rc = sqlite3_bind_text(preparedStatement, 1, id, strlen(id), nullptr);
            rc = sqlite3_bind_int(preparedStatement, 2, hasUploadsPlaylist);
            rc = sqlite3_bind_int(preparedStatement, 3, hasFavouritesPlaylist);
            rc = sqlite3_bind_int(preparedStatement, 4, hasLikesPlaylist);
            rc = sqlite3_bind_text(preparedStatement, 5, title, strlen(title), nullptr);
            rc = sqlite3_bind_text(preparedStatement, 6, description, strlen(description), nullptr);
            rc = sqlite3_bind_text(preparedStatement, 7, thumbnailDefault, strlen(thumbnailDefault), nullptr);
            rc = sqlite3_bind_text(preparedStatement, 8, thumbnailHigh, strlen(thumbnailHigh), nullptr);
            rc = sqlite3_bind_text(preparedStatement, 9, thumbnailMedium, strlen(thumbnailMedium), nullptr);
            rc = sqlite3_bind_int(preparedStatement, 10, subscribedOnYouTube);
            rc = sqlite3_bind_int(preparedStatement, 11, subscribedLocalOverride);

            // Step through, and do nothing, because this is an INSERT statement.
            while (sqlite3_step(preparedStatement) == SQLITE_ROW) {} // While query has result-rows.

            // Step, Clear and Reset the statement after each bind.
            rc = sqlite3_clear_bindings(preparedStatement);
            rc = sqlite3_reset(preparedStatement);

            // Finalize prepared statement
            db->finalizePreparedSqlStatement(rc, preparedStatement);

            counter++;
        }

        return 0;
    }
} // namespace sane