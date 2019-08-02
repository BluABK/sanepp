#include <iostream>
#include <vector>
#include <list>

#include <unicode/unistr.h>
#include <db_handler/db_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>
#include <entities/youtube_channel.hpp>

#define DEFAULT_CODEPAGE NULL
#define UTF8_CODEPAGE "UTF-8"

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
                       "ID TEXT PRIMARY KEY, "
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
        db->finalizePreparedSqlStatement(preparedStatement);

        return SQLITE_OK;
    }

    /**
     * Adds a list of YoutubeChannel objects to an SQLite3 Database.
     *
     * Conflict policy:     Overwrite existing.
     *
     * Conflict handling:   If an entry already exists it will be overwritten with the new values.
     *
     * @param t_channels    A list of smart (shared) pointers to instantiated YoutubeChannel entities.
     * @param t_errors      Pointer to a string list to put errors in, send in nullptr to disable.
     * @return
     */
    int addChannelsToDB(const std::list <std::shared_ptr<YoutubeChannel>> &t_channels,
                        std::list<std::string> *t_errors) {
        // Setup
        sqlite3_stmt *preparedStatement = nullptr;
        std::string sqlStatement;
        int counter = 0;

        // Acquire database handle.
        std::cout << "Acquiring DB handle..." << std::endl;
        std::shared_ptr<DBHandler> db = std::make_shared<DBHandler>();
        std::cout << "Acquired DB handle." << std::endl;

        // Creates the table if it does not already exist.
        createTable(db);

        // Iterate through the subscription objects and add relevant fields to DB.
        for (auto &channel : t_channels) {
            int rc = SQLITE_NEVER_RUN;

            // Figure out and sanitize the values.
            const char* id = channel->getIdAsCString();
            int hasUploadsPlaylist = channel->hasFavouritesPlaylist() ? 1 : 0;
            int hasFavouritesPlaylist = channel->hasFavouritesPlaylist() ? 1 : 0;
            int hasLikesPlaylist = channel->hasLikesPlaylist() ? 1 : 0;

            // Convert the UnicodeStrings to UTF-8 and append the result to a standard string,
            // and then convert that to a C-String and store that in a const char*.
            std::string m_titleAsString;
            channel->getTitle().toUTF8String(m_titleAsString);
            const char* m_titleAsCString = m_titleAsString.c_str();

            std::string m_descriptionAsString;
            channel->getDescription().toUTF8String(m_descriptionAsString);
            const char* m_descriptionAsCString = m_descriptionAsString.c_str();

            const char* title = validateSQLiteInput(m_titleAsCString);
            const char* description = validateSQLiteInput(m_descriptionAsCString);
            const char* thumbnailDefault = validateSQLiteInput(channel->getThumbnailDefaultAsCString());
            const char* thumbnailHigh = validateSQLiteInput(channel->getThumbnailHighAsCString());
            const char* thumbnailMedium = validateSQLiteInput(channel->getThumbnailMediumAsCString());

            // Set default values.
            // Bools and ints are the same to SQLite, but the C API only has bind for ints.
            int subscribedOnYouTube = 1;

            // Construct the UPSERT SQL statement which updates an already existing row or inserts a new one.
            //
            // The "excluded." prefix causes the <VALUE> to refer to the value for <VALUE> that *would have* been
            // inserted had there been no conflict.
            //
            // Hence, the effect of the *upsert* is to insert a <value> if none exists, OR to overwrite any prior
            // <VALUE> with the new one.
            //
            // SubscribedLocalOverride is not set because that is a local override which isn't part of remote properties.
            //
            sqlStatement = std::string("INSERT INTO youtube_channels ("
                                       "ID, HasUploadsPlaylist, HasFavouritesPlaylist, HasLikesPlaylist, Title, "
                                       "Description, ThumbnailDefault, ThumbnailHigh, ThumbnailMedium, "
                                       "SubscribedOnYouTube) "
                                       "VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?) "
                                       "ON CONFLICT(ID) DO UPDATE SET "
                                       "HasUploadsPlaylist=excluded.HasUploadsPlaylist, "
                                       "HasFavouritesPlaylist=excluded.HasFavouritesPlaylist, "
                                       "HasLikesPlaylist=excluded.HasLikesPlaylist, "
                                       "Title=excluded.Title, "
                                       "Description=excluded.Description, "
                                       "ThumbnailDefault=excluded.ThumbnailDefault, "
                                       "ThumbnailHigh=excluded.ThumbnailHigh, "
                                       "ThumbnailMedium=excluded.ThumbnailMedium, "
                                       "SubscribedOnYouTube=excluded.SubscribedOnYouTube");

            // Create a prepared statement
            preparedStatement = db->prepareSqlStatement(sqlStatement);
            if (db->lastStatus()  != SQLITE_OK) {
                std::cerr << "sane::prepareSqlStatement(" << sqlStatement << ") ERROR: returned non-zero status: "
                          << std::to_string( db->lastStatus() ) << std::endl;
                t_errors->push_back("sane::prepareSqlStatement(" + sqlStatement + ") ERROR: returned non-zero status: "
                                    + std::to_string( db->lastStatus() ));
                return db->lastStatus();
            }

            //  Bind-parameter for VALUES (indexing is 1-based).
            rc = sqlite3_bind_text(preparedStatement, 1,  id, strlen(id), nullptr);
            rc = sqlite3_bind_int(preparedStatement,  2,  hasUploadsPlaylist);
            rc = sqlite3_bind_int(preparedStatement,  3,  hasFavouritesPlaylist);
            rc = sqlite3_bind_int(preparedStatement,  4,  hasLikesPlaylist);
            rc = sqlite3_bind_text(preparedStatement, 5,  title, strlen(title), nullptr);
            rc = sqlite3_bind_text(preparedStatement, 6,  description, strlen(description), nullptr);
            rc = sqlite3_bind_text(preparedStatement, 7,  thumbnailDefault, strlen(thumbnailDefault), nullptr);
            rc = sqlite3_bind_text(preparedStatement, 8,  thumbnailHigh, strlen(thumbnailHigh), nullptr);
            rc = sqlite3_bind_text(preparedStatement, 9,  thumbnailMedium, strlen(thumbnailMedium), nullptr);
            rc = sqlite3_bind_int(preparedStatement,  10, subscribedOnYouTube);

            // Step through, and do nothing, because this is an INSERT statement.
            while (sqlite3_step(preparedStatement) == SQLITE_ROW) {} // While query has result-rows.

            // Clear and Reset the statement after each bind.
            rc = sqlite3_clear_bindings(preparedStatement);
            rc = sqlite3_reset(preparedStatement);

            // Finalize prepared statement
            db->finalizePreparedSqlStatement(preparedStatement);

            counter++;
        }

        return SQLITE_OK;
    }

    std::list <std::shared_ptr<YoutubeChannel>> getChannelsFromDB(std::list<std::string> *t_errors) {
        // Setup
        sqlite3_stmt *preparedStatement = nullptr;
        std::string sqlStatement;
        int rc = -1;

        // Acquire database handle.
        std::cout << "Acquiring DB handle..." << std::endl;
        std::shared_ptr<DBHandler> db = std::make_shared<DBHandler>();
        std::cout << "Acquired DB handle." << std::endl;

        sqlStatement = std::string("SELECT "
                                       "ID, HasUploadsPlaylist, HasFavouritesPlaylist, HasLikesPlaylist, Title, "
                                       "Description, ThumbnailDefault, ThumbnailHigh, ThumbnailMedium, "
                                       "SubscribedOnYouTube, SubscribedLocalOverride "
                                   "FROM "
                                       "youtube_channels ");

        // Create a prepared statement
        preparedStatement = db->prepareSqlStatement(sqlStatement);
        if (db->lastStatus()  != SQLITE_OK) {
            std::cerr << "sane::prepareSqlStatement(" << sqlStatement << ") ERROR: returned non-zero status: "
                      << std::to_string( db->lastStatus() ) << std::endl;
            t_errors->push_back("sane::prepareSqlStatement(" + sqlStatement + ") ERROR: returned non-zero status: "
                                + std::to_string( db->lastStatus() ));
        }

            // Create list to hold YouTube channel entities.
            std::list <std::shared_ptr<YoutubeChannel>> channels;

            // Step through, and do ...
            while (sqlite3_step(preparedStatement) == SQLITE_ROW) { // While query has result-rows.
                // Create a map of YouTube channel properties.
                std::map<std::string, const unsigned char*> channelMap;

                // NB: ColId indexing is 0-based
                const char* id                       = (char*) sqlite3_column_text(preparedStatement, 0);
                bool hasUploadsPlaylist              = sqlite3_column_int(preparedStatement, 1) == 1;
                bool hasFavouritesPlaylist           = sqlite3_column_int(preparedStatement, 2) == 1;
                bool hasLikesPlaylist                = sqlite3_column_int(preparedStatement, 3) == 1;
                const icu::UnicodeString title       = (char*) sqlite3_column_text(preparedStatement, 4);
                const icu::UnicodeString description = (char*) sqlite3_column_text(preparedStatement, 5);
                const char* thumbnailDefault         = (char*) sqlite3_column_text(preparedStatement, 6);
                const char* thumbnailHigh            = (char*) sqlite3_column_text(preparedStatement, 7);
                const char* thumbnailMedium          = (char*) sqlite3_column_text(preparedStatement, 8);
                bool subscribedOnYouTube             = sqlite3_column_int(preparedStatement, 9) == 1;
                bool subscribedLocalOverride         = sqlite3_column_int(preparedStatement, 10) == 1;

                // Create a YoutubeChannel entity based on the above values
                std::shared_ptr<sane::YoutubeChannel> channelEntity;
                channelEntity = std::make_shared<sane::YoutubeChannel>(id, hasUploadsPlaylist, hasFavouritesPlaylist,
                        hasLikesPlaylist, title, description, thumbnailDefault, thumbnailHigh, thumbnailMedium,
                        subscribedOnYouTube, subscribedLocalOverride);

                // Add the YoutubeChannel entity to the list of channels.
                channels.push_back(channelEntity);
            }

            // Step, Clear and Reset the statement after each bind.
            rc = sqlite3_step(preparedStatement);
            rc = sqlite3_clear_bindings(preparedStatement);
            rc = sqlite3_reset(preparedStatement);

        // Finalize prepared statement
        db->finalizePreparedSqlStatement(preparedStatement);

        return channels;
    }
} // namespace sane