#include <catch2/catch.hpp>

#define CUSTOM_DATABASE_NAME "sane_test.db"
#include <db_handler/db_handler.hpp>


TEST_CASE ("1: Testing sane::DBHandler: CREATE TABLE youtube_channels") {
    // Setup
    int rc;
    std::shared_ptr<sane::DBHandler> db;
    std::string sqlStatement;

    // Acquire database handle.
    db = std::make_shared<sane::DBHandler>(CUSTOM_DATABASE_NAME);

    // Create the table, if it doesn't already exist.
    rc = db->runSqlStatement("CREATE TABLE IF NOT EXISTS youtube_channels ("
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
                             "ThumbnailMedium TEXT)");

    REQUIRE(rc == SQLITE_OK);
}