#include <catch2/catch.hpp>

#include <entities/youtube_channel.hpp>

#define ENTITY_TEST_001_STRING_USER_ID                  "Lozjflf3i84bu_2jLTK2rA"
#define ENTITY_TEST_001_STRING_PREFIX_TITLE             "BluABK~"
#define ENTITY_TEST_001_STRING_PREFIX_DESCRIPTION       "This \"channel\" consists entirely of random stuff, stash, misc and videos favourited for later reference. At this time I really don't care if you sub or not, but feel free, who knows what sort of content I could suddenly come up with? =P"
#define ENTITY_TEST_001_STRING_PREFIX_THUMBNAIL_DEFAULT "https://yt3.ggpht.com/a/AGF-l79E64c4JTxdJufrBDoNKupFB_67gBU0zAeFGg=s88-c-k-c0xffffffff-no-rj-mo"
#define ENTITY_TEST_001_STRING_PREFIX_THUMBNAIL_HIGH    "https://yt3.ggpht.com/a/AGF-l79E64c4JTxdJufrBDoNKupFB_67gBU0zAeFGg=s800-c-k-c0xffffffff-no-rj-mo"
#define ENTITY_TEST_001_STRING_PREFIX_THUMBNAIL_MEDIUM  "https://yt3.ggpht.com/a/AGF-l79E64c4JTxdJufrBDoNKupFB_67gBU0zAeFGg=s240-c-k-c0xffffffff-no-rj-mo"

TEST_CASE ("1: Testing sane::entities: Create and instantiate a YoutubeChannel entity from map.") {
    // Create a map of YouTube channel properties.
    std::map<std::string, std::string> channelMap;
    channelMap["ID"]                 = ENTITY_TEST_001_STRING_USER_ID;
    channelMap["Title"]              = ENTITY_TEST_001_STRING_PREFIX_TITLE;
    channelMap["UploadsPlaylist"]    = "UU" ENTITY_TEST_001_STRING_USER_ID;
    channelMap["FavouritesPlaylist"] = "FL" ENTITY_TEST_001_STRING_USER_ID;
    channelMap["LikesPlaylist"]      = "LL" ENTITY_TEST_001_STRING_USER_ID;
    channelMap["Description"]        = ENTITY_TEST_001_STRING_PREFIX_DESCRIPTION;
    channelMap["ThumbnailDefault"]   = ENTITY_TEST_001_STRING_PREFIX_THUMBNAIL_DEFAULT;
    channelMap["ThumbnailHigh"]      = ENTITY_TEST_001_STRING_PREFIX_THUMBNAIL_HIGH;
    channelMap["ThumbnailMedium"]    = ENTITY_TEST_001_STRING_PREFIX_THUMBNAIL_MEDIUM;

    // Create a YoutubeChannel entity based on the above map.
    std::shared_ptr<sane::YoutubeChannel> channelEntity = std::make_shared<sane::YoutubeChannel>(channelMap);

    // Check that the entity actually was created.
    REQUIRE( channelEntity->getId()                 == ENTITY_TEST_001_STRING_USER_ID                  );
    REQUIRE( channelEntity->getChannelId()          == "UC" ENTITY_TEST_001_STRING_USER_ID             );
    REQUIRE( channelEntity->getTitle()              == ENTITY_TEST_001_STRING_PREFIX_TITLE             );
    REQUIRE( channelEntity->getUploadsPlaylist()    == "UU" ENTITY_TEST_001_STRING_USER_ID             );
    REQUIRE( channelEntity->getFavouritesPlaylist() == "FL" ENTITY_TEST_001_STRING_USER_ID             );
    REQUIRE( channelEntity->getLikesPlaylist()      == "LL" ENTITY_TEST_001_STRING_USER_ID             );
    REQUIRE( channelEntity->getDescription()        == ENTITY_TEST_001_STRING_PREFIX_DESCRIPTION       );
    REQUIRE( channelEntity->getThumbnailDefault()   == ENTITY_TEST_001_STRING_PREFIX_THUMBNAIL_DEFAULT );
    REQUIRE( channelEntity->getThumbnailHigh()      == ENTITY_TEST_001_STRING_PREFIX_THUMBNAIL_HIGH    );
    REQUIRE( channelEntity->getThumbnailMedium()    == ENTITY_TEST_001_STRING_PREFIX_THUMBNAIL_MEDIUM  );
}