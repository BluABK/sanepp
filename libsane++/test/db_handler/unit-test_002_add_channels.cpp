#include <catch2/catch.hpp>

#define CUSTOM_DATABASE_NAME "sane_test.db"

#define DB_YT_CHANS_TEST_002_STR_USER_ID              "Lozjflf3i84bu_2jLTK2rA"
#define DB_YT_CHANS_TEST_002_STR_PREFIX_TITLE         "Test Channel #"
#define DB_YT_CHANS_TEST_002_STR_PREFIX_DESC          "This is a test description for Test Channel #"
#define DB_YT_CHANS_TEST_002_STR_PREFIX_THUMB_DEFAULT "http://example.com/default"
#define DB_YT_CHANS_TEST_002_STR_PREFIX_THUMB_HIGH    "http://example.com/high"
#define DB_YT_CHANS_TEST_002_STR_PREFIX_THUMB_MEDIUM  "http://example.com/medium"

#include <db_handler/db_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>
#include <entities/youtube_channel.hpp>


TEST_CASE ("2: Testing sane::DBHandler: Add YouTube channels to DB") {
    // Setup
    int rc;
    std::shared_ptr<sane::DBHandler> db;
    std::string sqlStatement;

    // Acquire database handle.
    db = std::make_shared<sane::DBHandler>(CUSTOM_DATABASE_NAME);

    // Create some YouTube channels
    std::map<std::string, std::string> channelMap;
    std::list <std::shared_ptr<sane::YoutubeChannel>> channels;
    int channelAmount = 5;

    for (int i = 0; i <= channelAmount; ++i) {
        // Add some sample values required to generate a YoutubeChannel entity.
        channelMap["ID"]                 = DB_YT_CHANS_TEST_002_STR_USER_ID              + std::to_string(i);
        channelMap["Title"]              = DB_YT_CHANS_TEST_002_STR_PREFIX_TITLE         + std::to_string(i);
        channelMap["UploadsPlaylist"]    = "UU" DB_YT_CHANS_TEST_002_STR_USER_ID         + std::to_string(i);
        channelMap["FavouritesPlaylist"] = "FL" DB_YT_CHANS_TEST_002_STR_USER_ID         + std::to_string(i);
        channelMap["LikesPlaylist"]      = "LL" DB_YT_CHANS_TEST_002_STR_USER_ID         + std::to_string(i);
        channelMap["Description"]        = DB_YT_CHANS_TEST_002_STR_PREFIX_DESC          + std::to_string(i);
        channelMap["ThumbnailDefault"]   = DB_YT_CHANS_TEST_002_STR_PREFIX_THUMB_DEFAULT + std::to_string(i) + ".jpg";
        channelMap["ThumbnailHigh"]      = DB_YT_CHANS_TEST_002_STR_PREFIX_THUMB_HIGH    + std::to_string(i) + ".jpg";
        channelMap["ThumbnailMedium"]    = DB_YT_CHANS_TEST_002_STR_PREFIX_THUMB_MEDIUM  + std::to_string(i) + ".jpg";

        // Create a YoutubeChannel entity based on the above map.
        std::shared_ptr<sane::YoutubeChannel> channelEntity = std::make_shared<sane::YoutubeChannel>(channelMap);

        // Get the thumbnails map
        std::map<std::string, sane::thumbnail_t> thumbnails = channelEntity->getThumbnails();

        // Check that the entity actually was created.
        REQUIRE( channelEntity->getId()                 == DB_YT_CHANS_TEST_002_STR_USER_ID      + std::to_string(i) );
        REQUIRE(channelEntity->getTitle()               == DB_YT_CHANS_TEST_002_STR_PREFIX_TITLE + std::to_string(i) );
        REQUIRE( channelEntity->getUploadsPlaylist()    == "UU" DB_YT_CHANS_TEST_002_STR_USER_ID + std::to_string(i) );
        REQUIRE( channelEntity->getFavouritesPlaylist() == "FL" DB_YT_CHANS_TEST_002_STR_USER_ID + std::to_string(i) );
        REQUIRE( channelEntity->getLikesPlaylist()      == "LL" DB_YT_CHANS_TEST_002_STR_USER_ID + std::to_string(i) );
        REQUIRE(channelEntity->getDescription()         == DB_YT_CHANS_TEST_002_STR_PREFIX_DESC  + std::to_string(i) );

        REQUIRE( thumbnails["default"].url              == DB_YT_CHANS_TEST_002_STR_PREFIX_THUMB_DEFAULT
                                                                                                 + std::to_string(i)
                                                                                                 + ".jpg" );
        REQUIRE( thumbnails["high"].url                 == DB_YT_CHANS_TEST_002_STR_PREFIX_THUMB_HIGH
                                                                                                 + std::to_string(i)
                                                                                                 + ".jpg" );
        REQUIRE( thumbnails["medium"].url               == DB_YT_CHANS_TEST_002_STR_PREFIX_THUMB_MEDIUM
                                                                                                 + std::to_string(i)
                                                                                                 + ".jpg" );


        // Append it to the channels list.
        channels.push_back(channelEntity);

        // Clear the channel map in preparation for next iteration.
        channelMap.clear();
    }

    // Add the test entities to DB.
    std::list<std::string> t_errors;
    // Travis Edition; without the 'ON' part (UPSERT).
    rc = sane::addChannelsToDB(channels, &t_errors);

    for (const auto& error : t_errors) {
        std::cout << error << std::endl;
    }

    REQUIRE(rc == SQLITE_OK);
}