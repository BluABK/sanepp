#include <catch2/catch.hpp>

#include <entities/youtube_channel.hpp>

#define TEST_STRING_USER_ID                  "Lozjflf3i84bu_2jLTK2rA"
#define TEST_STRING_PREFIX_TITLE             "Test Channel #"
#define TEST_STRING_PREFIX_DESCRIPTION       "This is a test description for Test Channel #"
#define TEST_STRING_PREFIX_THUMBNAIL_DEFAULT "http://example.com/default.jpg"
#define TEST_STRING_PREFIX_THUMBNAIL_HIGH    "http://example.com/high.jpg"
#define TEST_STRING_PREFIX_THUMBNAIL_MEDIUM  "http://example.com/medium.jpg"

TEST_CASE ("1: Testing sane::entities: Create and instantiate a YoutubeChannel entity from map.") {
    // Create a map of YouTube channel properties.
    std::map<std::string, std::string> channelMap;
    channelMap["ID"]                 = TEST_STRING_USER_ID;
    channelMap["Title"]              = TEST_STRING_PREFIX_TITLE;
    channelMap["UploadsPlaylist"]    = "UU" TEST_STRING_USER_ID;
    channelMap["FavouritesPlaylist"] = "FL" TEST_STRING_USER_ID;
    channelMap["LikesPlaylist"]      = "LL" TEST_STRING_USER_ID;
    channelMap["Description"]        = TEST_STRING_PREFIX_DESCRIPTION;
    channelMap["ThumbnailDefault"]   = TEST_STRING_PREFIX_THUMBNAIL_DEFAULT;
    channelMap["ThumbnailHigh"]      = TEST_STRING_PREFIX_THUMBNAIL_HIGH;
    channelMap["ThumbnailMedium"]    = TEST_STRING_PREFIX_THUMBNAIL_MEDIUM;

    // Create a YoutubeChannel entity based on the above map.
    std::shared_ptr<sane::YoutubeChannel> channelEntity = std::make_shared<sane::YoutubeChannel>(channelMap);

    // Check that the entity actually was created.
    REQUIRE( channelEntity->getId()                 == TEST_STRING_USER_ID );
    REQUIRE( channelEntity->getTitle()              == TEST_STRING_PREFIX_TITLE );
    REQUIRE( channelEntity->getUploadsPlaylist()    == "UU" TEST_STRING_USER_ID );
    REQUIRE( channelEntity->getFavouritesPlaylist() == "FL" TEST_STRING_USER_ID );
    REQUIRE( channelEntity->getLikesPlaylist()      == "LL" TEST_STRING_USER_ID );
    REQUIRE( channelEntity->getDescription()        == TEST_STRING_PREFIX_DESCRIPTION );
    REQUIRE( channelEntity->getThumbnailDefault()   == TEST_STRING_PREFIX_THUMBNAIL_DEFAULT );
    REQUIRE( channelEntity->getThumbnailHigh()      == TEST_STRING_PREFIX_THUMBNAIL_HIGH );
    REQUIRE( channelEntity->getThumbnailMedium()    == TEST_STRING_PREFIX_THUMBNAIL_MEDIUM );
}