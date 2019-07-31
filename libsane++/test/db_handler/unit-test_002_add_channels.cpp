#include <catch2/catch.hpp>

#define CUSTOM_DATABASE_NAME "sane_test.db"

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
        channelMap["ID"]                 = "Lozjflf3i84bu_2jLTK2r"      + std::to_string(i);
        channelMap["Title"]              = "Test Channel #"             + std::to_string(i);
        channelMap["UploadsPlaylist"]    = "UULozjflf3i84bu_2jLTK2r"    + std::to_string(i);
        channelMap["FavouritesPlaylist"] = "FLLozjflf3i84bu_2jLTK2r"    + std::to_string(i);
        channelMap["LikesPlaylist"]      = "LLLozjflf3i84bu_2jLTK2r"    + std::to_string(i);
        channelMap["Description"]        = "This is a test description "
                                           "for Test Channel #"         + std::to_string(i);
        channelMap["ThumbnailDefault"]   = "http://example.com/default" + std::to_string(i) + ".jpg";
        channelMap["ThumbnailHigh"]      = "http://example.com/high"    + std::to_string(i) + ".jpg";
        channelMap["ThumbnailMedium"]    = "http://example.com/medium"  + std::to_string(i) + ".jpg";

        // Create a YoutubeChannel entity based on the above map.
        std::shared_ptr<sane::YoutubeChannel> channelEntity = std::make_shared<sane::YoutubeChannel>(channelMap);

        // Check that the entity actually was created.
        REQUIRE(channelEntity->getId() == "Lozjflf3i84bu_2jLTK2r" + std::to_string(i));

        // Append it to the channels list.
        channels.push_back(channelEntity);

        // Clear the channel map in preparation for next iteration.
        channelMap.clear();
    }

    // Add the test entities to DB.
    rc = sane::addChannelsToDB(channels);

    REQUIRE(rc == SQLITE_OK);
}