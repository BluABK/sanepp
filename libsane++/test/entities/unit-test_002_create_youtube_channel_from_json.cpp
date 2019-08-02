#include <catch2/catch.hpp>
#include <nlohmann/json.hpp>

#include <entities/youtube_channel.hpp>

#define ENTITY_TEST_002_STRING_USER_ID             "Lozjflf3i84bu_2jLTK2rA"
#define ENTITY_TEST_002_STRING_DESCRIPTION         "This \"channel\" consists entirely of random stuff, stash, misc and videos favourited for later reference. At this time I really don't care if you sub or not, but feel free, who knows what sort of content I could suddenly come up with? =P"
#define ENTITY_TEST_002_STRING_THUMBNAIL_DEFAULT   "https://yt3.ggpht.com/a/AGF-l79E64c4JTxdJufrBDoNKupFB_67gBU0zAeFGg=s88-c-k-c0xffffffff-no-rj-mo"
#define ENTITY_TEST_002_STRING_THUMBNAIL_HIGH      "https://yt3.ggpht.com/a/AGF-l79E64c4JTxdJufrBDoNKupFB_67gBU0zAeFGg=s800-c-k-c0xffffffff-no-rj-mo"
#define ENTITY_TEST_002_STRING_THUMBNAIL_MEDIUM    "https://yt3.ggpht.com/a/AGF-l79E64c4JTxdJufrBDoNKupFB_67gBU0zAeFGg=s240-c-k-c0xffffffff-no-rj-mo"

TEST_CASE ("2: Testing sane::entities: Create and instantiate a YoutubeChannel entity from JSON.") {
    // Raw string literal JSON of a YouTube API channel.list() response resource.
    nlohmann::json channelJson =
    R"(
        {
            "contentDetails": {
                "relatedPlaylists": {
                    "likes": "LLLozjflf3i84bu_2jLTK2rA",
                    "favorites": "FLLozjflf3i84bu_2jLTK2rA",
                    "uploads": "UULozjflf3i84bu_2jLTK2rA",
                    "watchHistory": "HL",
                    "watchLater": "WL"
                }
            },
            "etag": "\"Bdx4f4ps3xCOOo1WZ91nTLkRZ_c/21sjVeam1AcEWVy8Oa1u0mPvYiI\"",
            "id": "UCLozjflf3i84bu_2jLTK2rA",
            "kind": "youtube#channel",
            "snippet": {
                "customUrl": "BluABK",
                "description": "This \"channel\" consists entirely of random stuff, stash, misc and videos favourited for later reference. At this time I really don't care if you sub or not, but feel free, who knows what sort of content I could suddenly come up with? =P",
                "localized": {
                    "description": "This \"channel\" consists entirely of random stuff, stash, misc and videos favourited for later reference. At this time I really don't care if you sub or not, but feel free, who knows what sort of content I could suddenly come up with? =P",
                    "title": "BluABK~"
                },
                "publishedAt": "2009-09-25T16:34:52.000Z",
                "thumbnails": {
                    "default": {
                        "height": 88,
                        "url": "https://yt3.ggpht.com/a/AGF-l79E64c4JTxdJufrBDoNKupFB_67gBU0zAeFGg=s88-c-k-c0xffffffff-no-rj-mo",
                        "width": 88
                    },
                    "high": {
                        "height": 800,
                        "url": "https://yt3.ggpht.com/a/AGF-l79E64c4JTxdJufrBDoNKupFB_67gBU0zAeFGg=s800-c-k-c0xffffffff-no-rj-mo",
                        "width": 800
                    },
                    "medium": {
                        "height": 240,
                        "url": "https://yt3.ggpht.com/a/AGF-l79E64c4JTxdJufrBDoNKupFB_67gBU0zAeFGg=s240-c-k-c0xffffffff-no-rj-mo",
                        "width": 240
                    }
                },
                "title": "BluABK~"
            }
        }
    )"_json;

    // Create a YoutubeChannel entity based on the above JSON.
    std::shared_ptr<sane::YoutubeChannel> channelEntity = std::make_shared<sane::YoutubeChannel>(channelJson);

    // Check that the entity actually was created.
    REQUIRE( channelEntity->getId()                 == ENTITY_TEST_002_STRING_USER_ID           );
    REQUIRE( channelEntity->getChannelId()          == "UC" ENTITY_TEST_002_STRING_USER_ID      );
    REQUIRE(channelEntity->getTitle()              == "BluABK~"                                );
    REQUIRE( channelEntity->getUploadsPlaylist()    == "UU" ENTITY_TEST_002_STRING_USER_ID      );
    REQUIRE( channelEntity->getFavouritesPlaylist() == "FL" ENTITY_TEST_002_STRING_USER_ID      );
    REQUIRE( channelEntity->getLikesPlaylist()      == "LL" ENTITY_TEST_002_STRING_USER_ID      );
    REQUIRE(channelEntity->getDescription()        == ENTITY_TEST_002_STRING_DESCRIPTION       );
    REQUIRE( channelEntity->getThumbnailDefault()   == ENTITY_TEST_002_STRING_THUMBNAIL_DEFAULT );
    REQUIRE( channelEntity->getThumbnailHigh()      == ENTITY_TEST_002_STRING_THUMBNAIL_HIGH    );
    REQUIRE( channelEntity->getThumbnailMedium()    == ENTITY_TEST_002_STRING_THUMBNAIL_MEDIUM  );
}