#ifndef SANE_CLI_HPP
#define SANE_CLI_HPP

#include <map>
#include <list>
#include <string>
#include <iostream>

#include <lexical_analysis.hpp>
#include <types.hpp>
#include <entities/common.hpp>
#include <entities/youtube_channel.hpp>
#include <entities/youtube_video.hpp>
#include <api_handler/api_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>

#define DEFAULT_INDENT  4
#define PRINT_FULL_INFO true
#define PRINT_ONLY_VALID_INFO false

// Command categories
#define UNCATEGORISED   -1
#define CORE_CATEGORY    0
#define ENTITY_CATEGORY  1
#define JSON_CATEGORY    2
#define DB_CATEGORY      3

namespace sane {
    struct command_t {
        std::string name;
        std::string description;
        std::string usageSyntax;
        std::string usage;
        int category;
    };

    class CLI {
    public:
        explicit CLI();

        static std::map<std::string, std::string> stringToMap(const std::string &t_string);

        void addCommand(const std::string &t_name, const std::string &t_description,
                const int &t_category = UNCATEGORISED);

        void addCommand(const std::string &t_name, const std::string &t_description,
                        const std::string &t_usageSyntax = std::string(), const int &t_category = UNCATEGORISED);

        void addCommand(const std::string &t_name, const std::string &t_description,
                        const std::string &t_usageSyntax = std::string(), const std::string &t_usage = std::string(),
                        const int &t_category = UNCATEGORISED);
        
        void interactive();

        static const std::string padStringValue(const std::string &string_t, std::size_t maxLength);

        void exit();

        void help(bool extended = false);

        void executeCommand(const std::vector<std::string> &t_tokenizedInput);

        void authenticateOAuth2();

        void printPlaylistVideos(const std::string &t_playlistId,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        size_t getLongestChannelTitleLength();

        void printSubscriptionsFeed(int t_videoLimit,
                const std::string &t_part,
                const std::map<std::string, std::string> &t_filter,
                const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        void getSubscriptionsFromApi();

        void printSubscriptionsJsonFromApi(int jsonIndent = DEFAULT_INDENT);

        void printSubscriptionsFull();

        void printSubscriptionsBasic();

        void printChannelFromApiByName(const std::string &t_input);

        void printChannelFromApiByName(const std::vector<std::string> &t_input);

        void printChannelFromApiById(const std::string &t_input);

        void printChannelFromApiById(const std::vector<std::string> &t_input);

        void printChannelJsonFromApiByName(const std::string &t_input, int jsonIndent = DEFAULT_INDENT);

        void printChannelJsonFromApiByName(const std::vector<std::string> &t_input,
                                                  int jsonIndent = DEFAULT_INDENT);

        void printChannelJsonFromApiById(const std::string &t_input, int jsonIndent = DEFAULT_INDENT);

        void printChannelJsonFromApiById(const std::vector<std::string> &t_input,
                                                int jsonIndent = DEFAULT_INDENT);

        void listActivitiesJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listCaptionsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listChannelsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listChannelSectionsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listCommentsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listCommentThreadsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listGuideCategoriesJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listi18nLanguagesJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listi18nRegionsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listPlaylistItemsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listPlaylistsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listSearchJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listFilteredSearchJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listSubscriptionsJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void listVideoAbuseReportReasonsJsonFromApi(const std::vector<std::string> &t_input,
                                                    int jsonIndent = DEFAULT_INDENT);

        void listVideoCategoriesJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        nlohmann::json listVideosJsonFromApi(const std::vector<std::string> &t_input);

        void printVideosJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

        void printVideosFromApi(const std::vector<std::string> &t_input, bool t_printFullInfo = false);

    private:
        // Internal
        bool manuallyExit = false;
        bool isInteractive = false;

        // Define Command names.
        // Internal
        const std::string EXIT = "exit";
        const std::string HELP = "help";
        const std::string HELP_EXTENDED = "help-extended";
        // Authentication and OAuth2
        const std::string AUTHENTICATE_OAUTH2 = "auth-oauth2";

        // Activities
        // -- JSON
        const std::string LIST_ACTIVITIES_JSON = "list-activities-json";
        // Captions
        // -- JSON
        const std::string LIST_CAPTIONS_JSON = "list-captions-json";
        // Channel
        // -- Entity
        const std::string PRINT_CHANNEL_BY_USERNAME = "get-channel-by-name";
        const std::string PRINT_CHANNEL_BY_ID = "get-channel-by-id";
        // Channels
        // -- JSON
        const std::string PRINT_CHANNEL_JSON_BY_USERNAME = "get-channel-json-by-name";
        const std::string PRINT_CHANNEL_JSON_BY_ID = "get-channel-json-by-id";
        const std::string LIST_CHANNELS_JSON = "list-channels-json";
        // Channel sections
        // -- JSON
        const std::string LIST_CHANNEL_SECTIONS_JSON = "list-channel-sections-json";
        // Comments
        // -- JSON
        const std::string LIST_COMMENTS_JSON = "list-comments-json";
        // Comment threads
        // -- JSON
        const std::string LIST_COMMENT_THREADS_JSON = "list-comment-threads-json";
        // Guide Categories
        // -- JSON
        const std::string LIST_GUIDE_CATEGORIES_JSON = "list-guide-categories-json";
        // i18n Languages
        // -- JSON
        const std::string LIST_I18N_LANGUAGES_JSON = "list-i18n-languages-json";
        // i18n Regions
        // -- JSON
        const std::string LIST_I18N_REGIONS_JSON = "list-i18n-regions-json";
        // Playlist items
        // -- Print
        const std::string PRINT_PLAYLIST_ITEMS = "print-playlist-items";
        // -- JSON
        const std::string LIST_PLAYLIST_ITEMS_JSON = "list-playlist-items-json";
        // Playlists
        // -- JSON
        const std::string LIST_PLAYLISTS_JSON = "list-playlists-json";
        // Search
        // -- JSON
        const std::string SEARCH_JSON = "search-json";
        const std::string FILTERED_SEARCH_JSON = "search-filtered-json";
        // Subscriptions
        // -- Entity
        const std::string GET_SUBSCRIPTIONS_FROM_API = "get-subscriptions";
        const std::string PRINT_SUBSCRIPTIONS_PREFIX = "list-subscriptions-";
        const std::string PRINT_SUBSCRIPTIONS_FULL = PRINT_SUBSCRIPTIONS_PREFIX + "full";
        const std::string PRINT_SUBSCRIPTIONS_BASIC = PRINT_SUBSCRIPTIONS_PREFIX + "basic";
        // -- JSON
        const std::string LIST_SUBSCRIPTIONS_JSON = "list-subscriptions-json";
        const std::string PRINT_SUBSCRIPTIONS_JSON_FROM_API = "print-subscriptions-json";
        // Video Abuse Report Reasons
        // -- JSON
        const std::string LIST_VIDEO_ABUSE_REPORT_REASONS_JSON = "list-video-abuse-report-reasons-json";
        // Video categories
        // -- JSON
        const std::string LIST_VIDEO_CATEGORIES_JSON = "list-video-categories-json";
        // Videos
        // -- Entity
        const std::string PRINT_VIDEOS = "print-videos";
        const std::string PRINT_FULL_VIDEOS = "print-videos-full";
        // -- JSON
        const std::string PRINT_VIDEOS_JSON = "print-videos-json";
        // Subscriptions Feed
        // -- Print
        const std::string PRINT_SUBSCRIPTIONS_FEED = "print-subsfeed";


        // Map of commands (to be populated)
        std::map<std::string, command_t> m_commands;
        std::list<int> m_commandCategories;

        // Semantics
        const std::string COMMAND_PROMPT_STYLE = ">: ";
        size_t longestLine = 0;
        size_t longestLineWithUsageSyntax = 0;
        bool usageSyntaxEntryExists = false;
        int spacingLength = 4;

        std::shared_ptr<sane::APIHandler> api;
    };

} // namespace sane
#endif //SANE_CLI_HPP
