#ifndef SANE_YOUTUBE_VIDEO_HPP
#define SANE_YOUTUBE_VIDEO_HPP

#include <list>
#include <map>
#include <vector>
#include <string>
#include <list>

#include <nlohmann/json.hpp>

#include "common.hpp"

// TODO: Change a lot of string variables to ints and define some constants instead?

namespace sane {
    struct thumbnail_t {
        std::string url;
        unsigned int width{};
        unsigned int height{};
    };

    struct videoStream_t {
        unsigned int widthPixels{};
        unsigned int heightPixels{};
        double frameRateFps{};
        double aspectRatio{};
        std::string codec;

        // The video stream's bitrate, in bits per second.
        unsigned long bitrateBps{};

        // The amount that YouTube needs to rotate the original source content to properly display the video.
        // Valid values: "clockwise", "counterClockwise", "none", "other" and "upsideDown".
        std::string rotation;

        // A value that uniquely identifies a video vendor. Typically, the value is a four-letter vendor code.
        std::string vendor;
    };

    struct audioStream_t {
        unsigned int channelCount{};

        std::string codec;

        // The audio stream's bitrate, in bits per second.
        unsigned long bitrateBps{};

        // A value that uniquely identifies a video vendor. Typically, the value is a four-letter vendor code.
        std::string vendor;
    };

    struct processingProgress_t {
        // An estimate of the total number of parts that need to be processed for the video.
        // The number may be updated with more precise estimates while YouTube processes the video.
        unsigned long partsTotal{};

        // The number of parts of the video that YouTube has already processed.
        unsigned long partsProcessed{};

        // An estimate of the amount of time, in millseconds, that YouTube needs to finish processing the video.
        unsigned long timeLeftMs{};
    };

    struct liveStreamingDetails_t {
        // The time that the broadcast actually started. The value is specified in ISO 8601
        std::string actualStartTime; // TODO: Make it proper datetime type?

        // The time that the broadcast actually ended. The value is specified in ISO 8601.
        std::string actualEndTime; // TODO: Make it proper datetime type?

        // The time that the broadcast is scheduled to begin. The value is specified in ISO 8601.
        std::string scheduledStartTime; // TODO: Make it proper datetime type?

        // The time that the broadcast is scheduled to end. The value is specified in ISO 8601.
        // If the value is empty or the property is not present, then the broadcast is scheduled
        // to continue indefinitely.
        std::string scheduledEndTime; // TODO: Make it proper datetime type?

        // The number of viewers currently watching the broadcast.
        // The property and its value will be present if the broadcast has current viewers and
        // the broadcast owner has not hidden the viewcount for the video. Note that YouTube stops
        // tracking the number of concurrent viewers for a broadcast when the broadcast ends.
        unsigned long concurrentViewers{};

        // The ID of the currently active live chat attached to this video.
        // This field is filled only if the video is a currently live broadcast that has live chat.
        std::string activeLiveChatId;
    };

    struct tagSuggestions_t {
        // The keyword tag suggested for the video.
        std::string tag;

        //A set of video categories for which the tag is relevant.
        // You can use this information to display appropriate tag suggestions based
        // on the video category that the video uploader associates with the video.
        // By default, tag suggestions are relevant for all categories
        // if there are no restricts defined for the keyword.
        std::list<std::string> categoryRestricts;
    };

    /**
     * Contains translation of the video's metadata.
     */
    struct localization_t {
        // Localized video title.
        std::string title;

        // Localized video description.
        std::string description;
    };

    struct player_t {
        // An <iframe> tag that embeds a player that plays the video.
        std::string embedHtml;

        // The height and width of the embedded player returned in the player.embedHtml property.
        long embedHeight{};
        long embedWidth{};
    };

    class YoutubeVideo {
    public:
        // Create an empty instance, to be populated later.
        explicit YoutubeVideo();

        // Create an instance and feed it values through a JSON.
        explicit YoutubeVideo(nlohmann::json &t_data) {
            addFromJson(t_data);
        }

        void addFromJson(nlohmann::json t_json);

    private:
        std::string m_id;

        /**
         * SNIPPET
         *
         * Contains basic details about the video, such as its title, description, and category.
         */
        // The value is specified in ISO 8601 (YYYY-MM-DDThh:mm:ss.sZ) format.
        std::string m_publishedAt;    // TODO: Make it proper datetime type?

        std::string m_channelId;

        std::string m_title;

        std::string m_description;

        // Valid keys: "default", "medium", "high", "standard" and "maxres".
        std::map<std::string, thumbnail_t> m_thumbnails;

        std::string m_channelTitle;

        // A list of keyword tags associated with the video. Tags may contain spaces.
        // The property value has a maximum length of 500 characters
        std::list<std::string> m_tags;

        std::string m_categoryId;

        // Indicates if the video is an upcoming/active live broadcast.
        // Or it's "none" if the video is not an upcoming/active live broadcast.
        // Valid values: "live", "none" and "upcoming"
        std::string m_liveBroadcastContent;

        // The language of the text in the video resource's snippet.title and snippet.description properties.
        std::string m_defaultLanguage;
        std::string m_localizedTitle;
        std::string m_localizedDescription;
        std::string m_defaultAudioLanguage;

        /**
         * CONTENT DETAILS
         *
         * Contains information about the video content, including the length of the video
         * and an indication of whether captions are available for the video.
         */

        // Video length in ISO 8601.
        std::string m_duration;

        // contentDetails properties as boolean instead of string.
        bool m_is3D = false;
        bool m_is2D = false;
        bool m_isHD = false;
        bool m_hasCaptions = false;

        // Indicates whether the video represents licensed content, which means that the content was
        // uploaded to a channel linked to a YouTube content partner and then claimed by that partner.
        bool m_isLicensedContent = false;

        // A list of region codes that identify countries where the video is viewable.
        std::list<std::string> m_regionRestrictionWhitelist;

        // A list of region codes that identify countries where the video is blocked
        std::list<std::string> m_regionRestrictionBlacklist;

        // TODO: SKIPPING content ratings for now.

        // Projection format of the video.
        bool m_is360 = false;
        bool m_isRectanguar = false;

        // Indicates whether the video uploader has provided a custom thumbnail image for the video.
        // This property is only visible to the video uploader.
        bool m_hasCustomThumbnail = false;

        /**
         * STATUS
         *
         * Contains information about the video's uploading, processing, and privacy statuses.
         * */
        // The status of the uploaded video. Valid values: "deleted", "failed", "processed", "rejected" and "uploaded".
        std::string m_uploadStatus;

        // This value explains why a video failed to upload.
        // This property is only present if the uploadStatus property indicates that the upload failed.
        // Valid values: "codec", "conversion", "emptyFile", "invalidFile", "tooSmall" and "uploadAborted".
        std::string m_failureReason;

        // This value explains why YouTube rejected an uploaded video.
        // This property is only present if the uploadStatus property indicates that the upload was rejected.
        // Valid values: "claim", "copyright", "duplicate", "inappropriate", "legal", "length", "termsOfUse",
        //               "trademark", "uploaderAccountClosed" and "uploaderAccountSuspended"
        std::string m_rejectionReason;

        // Valid values: "private", "public" and "unlisted".
        std::string m_privacyStatus;

        // The date and time when the video is scheduled to publish.
        // It can be set only if the privacy status of the video is private. The value is specified in ISO 8601.
        std::string m_publishAt;  // TODO: Make it proper datetime type?

        // Valid values: "creativeCommon" and "youtube".
        std::string m_license;

        // This value indicates whether the video can be embedded on another website.
        bool m_isEmbeddable = false;

        // This value indicates whether the extended video statistics on the video's watch page are publicly viewable.
        // By default, those statistics are viewable, and statistics like a video's viewcount and ratings will still
        // be publicly visible even if this property's value is set to false.
        bool m_publicStatsViewable = false;

        /** STATISTICS */
        unsigned long m_viewCount{};
        unsigned long m_likeCount{};
        unsigned long m_dislikeCount{};
        unsigned long commentCount{};

        /**
         * PLAYER
         *
         * Contains information that you would use to play the video in an embedded player.
         */
        player_t m_player;

        /**
         * TOPIC DETAILS
         *
         * Information about topics associated with the video.
         * */
        // A list of Wikipedia URLs that provide a high-level description of the video's content.
        std::list<std::string> m_topicCategories;

        /**
         * RECORDING DETAILS
         *
         * Information about the location, date and address where the video was recorded.
         * */
        // The geolocation information associated with the video.
        std::string m_location;
        // The date and time when the video was recorded. The value is specified in ISO 8601.
        std::string m_recordingDate; // TODO: Make it proper datetime type?

        /**
         * FILE DETAILS
         *
         * Information about the video file that was uploaded to YouTube, including the file's resolution, duration,
         * audio and video codecs, stream bitrates, and more. **This data can only be retrieved by the video owner**.
         * */
        std::string m_fileName;
        unsigned long m_fileSize{};
        // Valid values: "archive", "audio", "document", "image", "other", "project" and "video".
        std::string m_fileType;
        // The uploaded video file's container format.
        std::string m_container;
        std::list<videoStream_t> m_videoStreams;
        std::list<audioStream_t> m_audioStreams;
        // The length of the uploaded video in milliseconds.
        unsigned long m_durationMs{};
        // The uploaded video file's combined (video and audio) bitrate in bits per second.
        unsigned long m_bitrateBps{};
        // The date and time when the uploaded video file was created. The value is specified in ISO 8601.
        std::string m_creationTime; // TODO: Make it proper datetime type?

        /**
         * PROCESSING DETAILS
         *
         * Information about YouTube's progress in processing the uploaded video file.
         * Identifies the current processing status and an estimate of the time remaining until
         * YouTube finishes processing the video. This part also indicates whether different
         * types of data or content, such as file details or thumbnail images, are available for the video.
         */
         // The video's processing status. This value indicates whether YouTube was able to process the video or
         // if the video is still being processed. Valid values: "failed", "processing", "succeeded", "terminated".
        std::string m_processingStatus;

        processingProgress_t m_processingProgress;

        // The reason that YouTube failed to process the video.
        // This property will only have a value if the processingStatus property's value is failed.
        // Valid values: "other", "streamingFailed", "transcodeFailed" and "uploadFailed".
        std::string m_processingFailureReason;

        // This value indicates whether the video processing engine has generated suggestions that might
        // improve YouTube's ability to process the the video, warnings that explain video processing
        // problems, or errors that cause video processing problems.
        std::string m_processingIssuesAvailability;

        // This value indicates whether keyword (tag) suggestions are available for the video.
        // Tags can be added to a video's metadata to make it easier for other users to find the video.
        std::string m_tagSuggestionsAvailability;

        // This value indicates whether video editing suggestions, which might improve video quality
        // or the playback experience, are available for the video.
        std::string m_editorSuggestionsAvailability;

        // This value indicates whether thumbnail images have been generated for the video.
        std::string m_thumbnailsAvailability;

        /**
         * SUGGESTIONS
         *
         * **This data can only be retrieved by the video owner.**
         *
         * Identifies opportunities to improve the video quality or the metadata for the uploaded video.
         * The suggestions object will only be returned if the processingDetails.tagSuggestionsAvailability property
         * or the processingDetails.editorSuggestionsAvailability property has a value of available.
         */
        // A list of errors that will prevent YouTube from successfully processing the uploaded video.
        // These errors indicate that, regardless of the video's current processing status, eventually,
        // that status will almost certainly be failed.
        // Valid values: "archiveFile", "audioFile", "docFile", "imageFile", "notAVideoFile" and "projectFile"
        std::list<std::string> m_processingErrors;

        // A list of reasons why YouTube may have difficulty transcoding the uploaded video or that might
        // result in an erroneous transcoding. These warnings are generated before YouTube actually processes
        // the uploaded video file. In addition, they identify issues that do not necessarily indicate that
        // video processing will fail but that still might cause problems such as sync issues, video artifacts,
        // or a missing audio track.
        // Valid values: "hasEditlist", "inconsistentResolution", "problematicAudioCodec", "problematicVideoCodec",
        //               "unknownAudioCodec", "unknownContainer" and "unknownVideoCodec".
        std::list<std::string> m_processingWarnings;

        // A list of suggestions that may improve YouTube's ability to process the video.
        // Valid values: "nonStreamableMov" and "sendBestQualityVideo".
        std::list<std::string> m_processingHints;

        // A list of keyword tags that could be added to the video's metadata to increase the likelihood
        // that users will locate your video when searching or browsing on YouTube.
        std::list<tagSuggestions_t> m_tagSuggestions;

        // A list of video editing operations that might improve the
        // video quality or playback experience of the uploaded video.
        // Valid values: "audioQuietAudioSwap", "videoAutoLevels", "videoCrop" and "videoStabilize".
        std::list<std::string> m_editorSuggestions;

        /**
         * LIVE STREAMING DETAILS
         *
         * Contains metadata about a live video broadcast. Will only be present in a video resource if the
         * video is an upcoming, live, or completed live broadcast.
         */
        std::list<liveStreamingDetails_t> m_liveStreamingDetails;

        /**
         * LOCALIZATIONS
         *
         * Contains translations of the video's metadata.
         */
        // The language of the localized text associated with the key value.
        // The value is a string that contains a BCP-47 language code.
        std::list<localization_t> m_localizations;

    };
} // namespace sane



#endif //SANE_YOUTUBE_VIDEO_HPP
