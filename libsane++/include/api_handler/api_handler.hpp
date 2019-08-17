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
#define SAPI_REMOTE_GET_CHANNEL                 "http://127.0.0.1:5002/api/v1/remote/channel"

/** YouTube API https://www.googleapis.com/youtube/v3/ */
#define YOUTUBE_API_ACTIVITIES                     "https://www.googleapis.com/youtube/v3/activities"
#define YOUTUBE_API_CAPTIONS                       "https://www.googleapis.com/youtube/v3/captions"
#define YOUTUBE_API_CHANNEL_BANNERS_INSERT         "https://www.googleapis.com/youtube/v3/channelBanners/insert"
#define YOUTUBE_API_CHANNELS                       "https://www.googleapis.com/youtube/v3/channels"
#define YOUTUBE_API_CHANNEL_SECTIONS               "https://www.googleapis.com/youtube/v3/channelSections"
#define YOUTUBE_API_COMMENTS                       "https://www.googleapis.com/youtube/v3/comments"
#define YOUTUBE_API_COMMENTS_MARK_AS_SPAM          "https://www.googleapis.com/youtube/v3/comments/markAsSpam"
#define YOUTUBE_API_COMMENTS_SET_MODERATION_STATUS "https://www.googleapis.com/youtube/v3/comments/setModerationStatus"
#define YOUTUBE_API_COMMENT_THREADS                "https://www.googleapis.com/youtube/v3/commentThreads"
#define YOUTUBE_API_GUIDE_CATEGORIES               "https://www.googleapis.com/youtube/v3/guideCategories"
#define YOUTUBE_API_I18N_LANGUAGES                 "https://www.googleapis.com/youtube/v3/i18nLanguages"
#define YOUTUBE_API_I18N_REGIONS                   "https://www.googleapis.com/youtube/v3/i18nRegions"
#define YOUTUBE_API_PLAYLIST_ITEMS                 "https://www.googleapis.com/youtube/v3/playlistItems"
#define YOUTUBE_API_PLAYLISTS                      "https://www.googleapis.com/youtube/v3/playlists"
#define YOUTUBE_API_SEARCH                         "https://www.googleapis.com/youtube/v3/search"
#define YOUTUBE_API_SUBSCRIPTIONS                  "https://www.googleapis.com/youtube/v3/subscriptions"
#define YOUTUBE_API_THUMBNAILS_SET                 "https://www.googleapis.com/youtube/v3/thumbnails/set"
#define YOUTUBE_API_VIDEO_ABUSE_REPORT_REASONS     "https://www.googleapis.com/youtube/v3/videoAbuseReportReasons"
#define YOUTUBE_API_VIDEO_CATEGORIES               "https://www.googleapis.com/youtube/v3/videoCategories"
#define YOUTUBE_API_VIDEOS                         "https://www.googleapis.com/youtube/v3/videos"
#define YOUTUBE_API_VIDEOS_RATE                    "https://www.googleapis.com/youtube/v3/videos/rate"
#define YOUTUBE_API_VIDEOS_GET_RATING              "https://www.googleapis.com/youtube/v3/videos/getRating"
#define YOUTUBE_API_VIDEOS_REPORT_ABUSE            "https://www.googleapis.com/youtube/v3/videos/reportAbuse"
#define YOUTUBE_API_WATERMARKS_SET                 "https://www.googleapis.com/youtube/v3/watermarks/set"
#define YOUTUBE_API_WATERMARKS_UNSET               "https://www.googleapis.com/youtube/v3/watermarks/unset"

namespace sane {
    class APIHandler {
    public:
        nlohmann::json getOauth2Token(const std::string &t_tokenUri = {}, const std::string &t_refreshToken = {},
                                      const std::string &t_clientId = {}, const std::string &t_clientSecret = {});

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

        /** YouTube API https://www.googleapis.com/youtube/v3/ */
        nlohmann::json youtubeListActivities(const std::string &t_part,
                                             const std::map<std::string, std::string> &t_filter,
                                             const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListCaptions(const std::string &t_part, const std::string &t_videoId,
                                           const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListChannels(const std::string &t_part,
                                           const std::map<std::string, std::string> &t_filter,
                                           const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListChannelSections(const std::string &t_part,
                                                  const std::map<std::string, std::string> &t_filter,
                                                  const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListComments(const std::string &t_part,
                                           const std::map<std::string, std::string> &t_filter,
                                           const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListCommentThreads(const std::string &t_part,
                                                 const std::map<std::string, std::string> &t_filter,
                                                 const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListGuideCategories(const std::string &t_part,
                                                  const std::map<std::string, std::string> &t_filter,
                                                  const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListI18nLanguages(const std::string &t_part,
                                                const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListI18nRegions(const std::string &t_part,
                                              const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListPlaylistItems(const std::string &t_part,
                                                const std::map<std::string, std::string> &t_filter,
                                                const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListPlaylists(const std::string &t_part,
                                            const std::map<std::string, std::string> &t_filter,
                                            const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeSearch(
                const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeSearchFiltered(const std::map<std::string, std::string> &t_filter,
                                             const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListSubscriptions(const std::string &t_part,
                                                const std::map<std::string, std::string> &t_filter,
                                                const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListVideoAbuseReportReasons(const std::string &t_part,
                                                          const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListVideoCategories(const std::string &t_part,
                                                  const std::map<std::string, std::string> &t_filter,
                                                  const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

        nlohmann::json youtubeListVideos(const std::string &t_part,
                                         const std::map<std::string, std::string> &t_filter,
                                         const std::map<std::string, std::string> &t_optParams = std::map<std::string, std::string>());

    private:
    };
} // namespace sane.
#endif // Header guards.