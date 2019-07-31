#include <catch2/catch.hpp>

#define CUSTOM_DATABASE_NAME "sane_test.db"

#define TEST_STRING_PREFIX_TITLE "Test Channel #"
#define TEST_STRING_PREFIX_DESCRIPTION "This is a test description for Test Channel #"
#define TEST_STRING_PREFIX_THUMBNAIL_DEFAULT "http://example.com/default"
#define TEST_STRING_PREFIX_THUMBNAIL_HIGH "http://example.com/high"
#define TEST_STRING_PREFIX_THUMBNAIL_MEDIUM "http://example.com/medium"

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
        channelMap["ID"]                 = "Lozjflf3i84bu_2jLTK2r"              + std::to_string(i);
        channelMap["Title"]              = TEST_STRING_PREFIX_TITLE             + std::to_string(i);
        channelMap["UploadsPlaylist"]    = "UULozjflf3i84bu_2jLTK2r"            + std::to_string(i);
        channelMap["FavouritesPlaylist"] = "FLLozjflf3i84bu_2jLTK2r"            + std::to_string(i);
        channelMap["LikesPlaylist"]      = "LLLozjflf3i84bu_2jLTK2r"            + std::to_string(i);
        channelMap["Description"]        = TEST_STRING_PREFIX_DESCRIPTION       + std::to_string(i);
        channelMap["ThumbnailDefault"]   = TEST_STRING_PREFIX_THUMBNAIL_DEFAULT + std::to_string(i) + ".jpg";
        channelMap["ThumbnailHigh"]      = TEST_STRING_PREFIX_THUMBNAIL_HIGH    + std::to_string(i) + ".jpg";
        channelMap["ThumbnailMedium"]    = TEST_STRING_PREFIX_THUMBNAIL_MEDIUM  + std::to_string(i) + ".jpg";

        // Create a YoutubeChannel entity based on the above map.
        std::shared_ptr<sane::YoutubeChannel> channelEntity = std::make_shared<sane::YoutubeChannel>(channelMap);

        // Check that the entity actually was created.
        REQUIRE( channelEntity->getId()                 == "Lozjflf3i84bu_2jLTK2r"   + std::to_string(i) );
        REQUIRE( channelEntity->getTitle()              == TEST_STRING_PREFIX_TITLE  + std::to_string(i) );
        REQUIRE( channelEntity->getUploadsPlaylist()    == "UULozjflf3i84bu_2jLTK2r" + std::to_string(i) );
        REQUIRE( channelEntity->getFavouritesPlaylist() == "FLLozjflf3i84bu_2jLTK2r" + std::to_string(i) );
        REQUIRE( channelEntity->getLikesPlaylist()      == "LLLozjflf3i84bu_2jLTK2r" + std::to_string(i) );
        REQUIRE( channelEntity->getDescription()        == TEST_STRING_PREFIX_DESCRIPTION
                                                                                     + std::to_string(i) );
        REQUIRE( channelEntity->getThumbnailDefault()   == TEST_STRING_PREFIX_THUMBNAIL_DEFAULT
                                                                                     + std::to_string(i) + ".jpg" );
        REQUIRE( channelEntity->getThumbnailHigh()      == TEST_STRING_PREFIX_THUMBNAIL_HIGH
                                                                                     + std::to_string(i) + ".jpg" );
        REQUIRE( channelEntity->getThumbnailMedium()    == TEST_STRING_PREFIX_THUMBNAIL_MEDIUM
                                                                                     + std::to_string(i) + ".jpg" );


        // Append it to the channels list.
        channels.push_back(channelEntity);

        // Clear the channel map in preparation for next iteration.
        channelMap.clear();
    }

    // Add the test entities to DB.
    rc = sane::addChannelsToDB(channels);

    REQUIRE(rc == SQLITE_OK);
}