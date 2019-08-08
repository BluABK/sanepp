/*
 *  SaneAPI handling via libcURL -- Headers.
 */
#ifndef SANEPP_API_HANDLER_HEADER
#define SANEPP_API_HANDLER_HEADER

#include <list>

#include <nlohmann/json.hpp>

#include <entities/youtube_channel.hpp>

#define CLEAR_PROBLEMS true
#define DONT_CLEAR_PROBLEMS false
#define IS_NOT_BEGINNING false
#define IS_BEGINNING true

/**
SaneAPI (SAPI) REST API URLs, see SaneAPI documentation
for the meaning of Local, Remote and YouTube.
*/

/** Local: SAPI Internal or local operations. */
#define SAPI_LOCAL_ADD_SUBSCRIPTION             "http://127.0.0.1:5002/api/v1/local/add_subscription"

/** Remote: Requests to the YouTube API with some extra functionality added on. */
#define SAPI_REMOTE_GET_SUBSCRIPTIONS           "http://127.0.0.1:5002/api/v1/remote/subscriptions"
// TODO: Port SAPI_REMOTE_GET_SUBFEED functionality to native C++
#define SAPI_REMOTE_GET_SUBFEED                 "http://127.0.0.1:5002/api/v1/remote/subfeed"
#define SAPI_REMOTE_GET_CHANNEL                 "http://127.0.0.1:5002/api/v1/remote/channel"

/** YouTube: Pass-through kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/ */
#define SAPI_YT_ACTIVITIES_LIST                 "http://127.0.0.1:5002/api/v1/youtube/activities/list"
#define SAPI_YT_ACTIVITIES_INSERT               "http://127.0.0.1:5002/api/v1/youtube/activities/insert"
#define SAPI_YT_CAPTIONS_LIST                   "http://127.0.0.1:5002/api/v1/youtube/captions/list"
#define SAPI_YT_CAPTIONS_INSERT                 "http://127.0.0.1:5002/api/v1/youtube/captions/insert"
#define SAPI_YT_CAPTIONS_UPDATE                 "http://127.0.0.1:5002/api/v1/youtube/captions/update"
#define SAPI_YT_CAPTIONS_DOWNLOAD               "http://127.0.0.1:5002/api/v1/youtube/captions/download"
#define SAPI_YT_CAPTIONS_DELETE                 "http://127.0.0.1:5002/api/v1/youtube/captions/delete"
#define SAPI_YT_CHANNEL_BANNERS_INSERT          "http://127.0.0.1:5002/api/v1/youtube/channelBanners/insert"
#define SAPI_YT_CHANNELS_LIST                   "http://127.0.0.1:5002/api/v1/youtube/channels/list"
#define SAPI_YT_CHANNEL_SECTIONS_LIST            "http://127.0.0.1:5002/api/v1/youtube/channelSections/list"
#define SAPI_YT_CHANNEL_SECTIONS_INSERT          "http://127.0.0.1:5002/api/v1/youtube/channelSection/insert"
#define SAPI_YT_CHANNEL_SECTIONS_UPDATE          "http://127.0.0.1:5002/api/v1/youtube/channelSection/update"
#define SAPI_YT_CHANNEL_SECTIONS_DELETE          "http://127.0.0.1:5002/api/v1/youtube/channelSection/delete"
#define SAPI_YT_COMMENTS_LIST                    "http://127.0.0.1:5002/api/v1/youtube/comments/list"
#define SAPI_YT_COMMENTS_INSERT                  "http://127.0.0.1:5002/api/v1/youtube/comments/insert"
#define SAPI_YT_COMMENTS_UPDATE                  "http://127.0.0.1:5002/api/v1/youtube/comments/update"
#define SAPI_YT_COMMENTS_MARK_AS_SPAM            "http://127.0.0.1:5002/api/v1/youtube/comments/markAsSpam"
#define SAPI_YT_COMMENTS_SET_MODERATION_STATUS   "http://127.0.0.1:5002/api/v1/youtube/comments/setModerationStatus"
#define SAPI_YT_COMMENTS_DELETE                  "http://127.0.0.1:5002/api/v1/youtube/comments/delete"
#define SAPI_YT_COMMENT_THREADS_LIST             "http://127.0.0.1:5002/api/v1/youtube/commentThreads/list"
#define SAPI_YT_COMMENT_THREADS_INSERT           "http://127.0.0.1:5002/api/v1/youtube/commentThreads/insert"
#define SAPI_YT_COMMENT_THREADS_UPDATE           "http://127.0.0.1:5002/api/v1/youtube/commentThreads/update"
#define SAPI_YT_GUIDE_CATEGORIES_LIST           "http://127.0.0.1:5002/api/v1/youtube/guideCategories/list"
#define SAPI_YT_I18N_LANGUAGES_LIST            "http://127.0.0.1:5002/api/v1/youtube/i18nLanguages/list"
#define SAPI_YT_I18N_REGIONS_LIST               "http://127.0.0.1:5002/api/v1/youtube/i18nRegions/list"
#define SAPI_YT_PLAYLIST_ITEMS_LIST             "http://127.0.0.1:5002/api/v1/youtube/playlistItems/list"
#define SAPI_YT_PLAYLIST_ITEMS_INSERT           "http://127.0.0.1:5002/api/v1/youtube/playlistItems/insert"
#define SAPI_YT_PLAYLIST_ITEMS_UPDATE           "http://127.0.0.1:5002/api/v1/youtube/playlistItems/update"
#define SAPI_YT_PLAYLIST_ITEMS_DELETE           "http://127.0.0.1:5002/api/v1/youtube/playlistItems/delete"
#define SAPI_YT_PLAYLISTS_LIST                  "http://127.0.0.1:5002/api/v1/youtube/playlists/list"
#define SAPI_YT_PLAYLISTS_INSERT                "http://127.0.0.1:5002/api/v1/youtube/playlists/insert"
#define SAPI_YT_PLAYLISTS_UPDATE                "http://127.0.0.1:5002/api/v1/youtube/playlists/update"
#define SAPI_YT_PLAYLISTS_DELETE                "http://127.0.0.1:5002/api/v1/youtube/playlists/delete"
#define SAPI_YT_SEARCH_LIST                     "http://127.0.0.1:5002/api/v1/youtube/search/list"
#define SAPI_YT_SUBSCRIPTIONS_LIST              "http://127.0.0.1:5002/api/v1/youtube/subscriptions/list"
#define SAPI_YT_THUMBNAILS_SET                  "http://127.0.0.1:5002/api/v1/youtube/thumbnails/set"
#define SAPI_YT_VIDEO_ABUSE_REPORT_REASONS_LIST "http://127.0.0.1:5002/api/v1/youtube/videoAbuseReportReasons/list"
#define SAPI_YT_VIDEO_CATEGORIES_LIST           "http://127.0.0.1:5002/api/v1/youtube/videoCategories/list"
#define SAPI_YT_VIDEOS_LIST                     "http://127.0.0.1:5002/api/v1/youtube/videos/list"
#define SAPI_YT_VIDEOS_INSERT                   "http://127.0.0.1:5002/api/v1/youtube/videos/insert"
#define SAPI_YT_VIDEOS_UPDATE                   "http://127.0.0.1:5002/api/v1/youtube/videos/update"
#define SAPI_YT_VIDEOS_RATE                     "http://127.0.0.1:5002/api/v1/youtube/videos/rate"
#define SAPI_YT_VIDEOS_GET_RATING               "http://127.0.0.1:5002/api/v1/youtube/videos/getRating"
#define SAPI_YT_VIDEOS_REPORT_ABUSE             "http://127.0.0.1:5002/api/v1/youtube/videos/reportAbuse"
#define SAPI_YT_VIDEOS_DELETE                   "http://127.0.0.1:5002/api/v1/youtube/videos/delete"
#define SAPI_YT_WATERMARKS_SET                  "http://127.0.0.1:5002/api/v1/youtube/watermarks/set"
#define SAPI_YT_WATERMARKS_UNSET                "http://127.0.0.1:5002/api/v1/youtube/watermarks/unset"

namespace sane {
    class APIHandler {
    public:
        nlohmann::json getSapiResponse(const std::string& url);

        void printReport(int t_warningsCount, int t_errorsCount);

        void printReport(std::shared_ptr<YoutubeChannel> &t_channel);

        static std::string compileUrlVariables(const std::list<std::map<std::string, std::string>> &t_variableMaps,
                bool t_isBeginning = false);

        static std::string compileUrlVariables(const std::list<std::string> &t_variableValues);

        /** Remote: Requests to the YouTube API with some extra functionality added on. */

        void sapiRemoteGetSubscriptions(bool clearProblems = CLEAR_PROBLEMS);

        nlohmann::json sapiRemoteGetSubscriptionsJson();

        std::shared_ptr<YoutubeChannel> sapiRemoteGetChannelByUsername(const std::string &t_username,
                                                                       bool clearProblems = CLEAR_PROBLEMS);

        nlohmann::json sapiRemoteGetChannelJsonByUsername(const std::string &t_username);

        std::shared_ptr<YoutubeChannel> sapiRemoteGetChannelById(const std::string &t_channelId,
                                                                 bool clearProblems = CLEAR_PROBLEMS);

        nlohmann::json sapiRemoteGetChannelJsonById(const std::string &t_channelId);

        /** YouTube: Pass-through kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/
         * "Youtube" prefix is implicit.
         */
        nlohmann::json sapiGetActivitiesList(const std::string &t_part,
                const std::map<std::string,std::string> & t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetCaptionsList(const std::string &t_part, const std::string &t_videoId,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetChannelsList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetChannelSectionsList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetCommentsList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetCommentThreadsList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetGuideCategoriesList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGeti18nLanguagesList(const std::string &t_part,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGeti18nRegionsList(const std::string &t_part,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetPlaylistItemsList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetPlaylistsList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetSearchList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetSubscriptionsList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetVideoAbuseReportReasonsList(const std::string &t_part,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetVideoCategoriesList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json sapiGetVideosList(const std::string &t_part,
                const std::map<std::string,std::string> &t_filter,
                const std::map<std::string,std::string> &t_optParams = std::map<std::string, std::string>());

    private:
    };
} // namespace sane.
#endif // Header guards.