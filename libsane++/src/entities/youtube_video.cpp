#include <iostream>

#include <entities/youtube_video.hpp>

namespace sane {
    // An empty constructor if you want to populate it later.
    YoutubeVideo::YoutubeVideo() = default;

    /**
     * Attempts to assign a JSON value to a given string and handle any bogus data.
     * If the input JSON value isn't a string it sets MISSING_VALUE.
     *
     * This function passes stringToAssignValue by reference (directly modifying it).
     *
     * @param stringToAssignValue   String to assign a value (modified directly).
     * @param unknownJsonTypeValue  The undetermined JSON value to assign the string.
     * @param fullJson              The complete JSON object, for use in debugging/error reporting.
     */
    void YoutubeVideo::assignJsonStringValue(std::string &stringToAssignValue,
                                               nlohmann::json &unknownJsonTypeValue, nlohmann::json &t_json) {
        if (unknownJsonTypeValue.is_null()) {
            addWarning("WARNING: YoutubeVideo::addFromJson." + std::string(GET_VARIABLE_NAME(stringToAssignValue))
                       + " is NULL not string, setting '" + MISSING_VALUE + "' string in its stead!", t_json);
            stringToAssignValue = MISSING_VALUE;
        }
        else if (!unknownJsonTypeValue.is_string()) {
            addWarning("WARNING: YoutubeVideo::addFromJson.favouritesPlaylist is " +
                       std::string(unknownJsonTypeValue.type_name()) + " not string, setting'" +
                       MISSING_VALUE + "' string in its stead!", t_json);
            stringToAssignValue = MISSING_VALUE;
        } else {
            // If it actually is a string, then explicitly cast it.
            stringToAssignValue = unknownJsonTypeValue.get<std::string>();
        }
    }

    void YoutubeVideo::addError(const std::string &t_errorMsg, nlohmann::json &t_json) {
        std::map<std::string, nlohmann::json> _;
        _["error"] = t_errorMsg;
        _["json"] = t_json;

        m_errors.push_back(_);
    }

    void YoutubeVideo::addWarning(const std::string &t_warningMsg, nlohmann::json &t_json) {
        std::map<std::string, nlohmann::json> _;
        _["error"] = t_warningMsg;
        _["json"] = t_json;

        m_warnings.push_back(_);
    }

    std::list<std::map<std::string, nlohmann::json>> YoutubeVideo::getErrors() {
        return m_errors;
    }

    std::list<std::map<std::string, nlohmann::json>> YoutubeVideo::getWarnings() {
        return m_warnings;
    }

    bool YoutubeVideo::wasAborted() {
        return m_aborted;
    }

    void YoutubeVideo::clearWarnings() {
        m_warnings.clear();
    }

    void YoutubeVideo::clearErrors() {
        m_errors.clear();
    }

    void YoutubeVideo::clearErrorsAndWarnings() {
        clearWarnings();
        clearErrors();
    }

    // START: Getters & Setters.

    void YoutubeVideo::setId(nlohmann::json t_id, nlohmann::json t_json) {
        assignJsonStringValue(m_id, t_id, t_json);
    }

    const std::string &YoutubeVideo::getId() const {
        return m_id;
    }

    const std::string &YoutubeVideo::getPublishedAt() const {
        return m_publishedAt;
    }

    void YoutubeVideo::setPublishedAt(const std::string &mPublishedAt) {
        m_publishedAt = mPublishedAt;
    }

    const std::string &YoutubeVideo::getChannelId() const {
        return m_channelId;
    }

    void YoutubeVideo::setChannelId(const std::string &mChannelId) {
        m_channelId = mChannelId;
    }

    const std::string &YoutubeVideo::getTitle() const {
        return m_title;
    }

    void YoutubeVideo::setTitle(const std::string &mTitle) {
        m_title = mTitle;
    }

    const std::string &YoutubeVideo::getDescription() const {
        return m_description;
    }

    void YoutubeVideo::setDescription(const std::string &mDescription) {
        m_description = mDescription;
    }

    const std::map<std::string, thumbnail_t> &YoutubeVideo::getThumbnails() const {
        return m_thumbnails;
    }

    void YoutubeVideo::setThumbnails(const std::map<std::string, thumbnail_t> &mThumbnails) {
        m_thumbnails = mThumbnails;
    }

    const std::string &YoutubeVideo::getChannelTitle() const {
        return m_channelTitle;
    }

    void YoutubeVideo::setChannelTitle(const std::string &mChannelTitle) {
        m_channelTitle = mChannelTitle;
    }

    const std::list<std::string> &YoutubeVideo::getTags() const {
        return m_tags;
    }

    void YoutubeVideo::setTags(const std::list<std::string> &mTags) {
        m_tags = mTags;
    }

    const std::string &YoutubeVideo::getCategoryId() const {
        return m_categoryId;
    }

    void YoutubeVideo::setCategoryId(const std::string &mCategoryId) {
        m_categoryId = mCategoryId;
    }

    const std::string &YoutubeVideo::getLiveBroadcastContent() const {
        return m_liveBroadcastContent;
    }

    void YoutubeVideo::setLiveBroadcastContent(const std::string &mLiveBroadcastContent) {
        m_liveBroadcastContent = mLiveBroadcastContent;
    }

    const std::string &YoutubeVideo::getDefaultLanguage() const {
        return m_defaultLanguage;
    }

    void YoutubeVideo::setDefaultLanguage(const std::string &mDefaultLanguage) {
        m_defaultLanguage = mDefaultLanguage;
    }

    const std::string &YoutubeVideo::getLocalizedTitle() const {
        return m_localizedTitle;
    }

    void YoutubeVideo::setLocalizedTitle(const std::string &mLocalizedTitle) {
        m_localizedTitle = mLocalizedTitle;
    }

    const std::string &YoutubeVideo::getLocalizedDescription() const {
        return m_localizedDescription;
    }

    void YoutubeVideo::setLocalizedDescription(const std::string &mLocalizedDescription) {
        m_localizedDescription = mLocalizedDescription;
    }

    const std::string &YoutubeVideo::getDefaultAudioLanguage() const {
        return m_defaultAudioLanguage;
    }

    void YoutubeVideo::setDefaultAudioLanguage(const std::string &mDefaultAudioLanguage) {
        m_defaultAudioLanguage = mDefaultAudioLanguage;
    }

    const std::string &YoutubeVideo::getDuration() const {
        return m_duration;
    }

    void YoutubeVideo::setDuration(const std::string &mDuration) {
        m_duration = mDuration;
    }

    bool YoutubeVideo::isMIs3D() const {
        return m_is3D;
    }

    void YoutubeVideo::setIs3D(bool mIs3D) {
        m_is3D = mIs3D;
    }

    bool YoutubeVideo::isMIs2D() const {
        return m_is2D;
    }

    void YoutubeVideo::setIs2D(bool mIs2D) {
        m_is2D = mIs2D;
    }

    bool YoutubeVideo::isMIsHd() const {
        return m_isHD;
    }

    void YoutubeVideo::setIsHd(bool mIsHd) {
        m_isHD = mIsHd;
    }

    bool YoutubeVideo::isMHasCaptions() const {
        return m_hasCaptions;
    }

    void YoutubeVideo::setHasCaptions(bool mHasCaptions) {
        m_hasCaptions = mHasCaptions;
    }

    bool YoutubeVideo::isMIsLicensedContent() const {
        return m_isLicensedContent;
    }

    void YoutubeVideo::setIsLicensedContent(bool mIsLicensedContent) {
        m_isLicensedContent = mIsLicensedContent;
    }

    const std::list<std::string> &YoutubeVideo::getRegionRestrictionWhitelist() const {
        return m_regionRestrictionWhitelist;
    }

    void YoutubeVideo::setRegionRestrictionWhitelist(const std::list<std::string> &mRegionRestrictionWhitelist) {
        m_regionRestrictionWhitelist = mRegionRestrictionWhitelist;
    }

    const std::list<std::string> &YoutubeVideo::getRegionRestrictionBlacklist() const {
        return m_regionRestrictionBlacklist;
    }

    void YoutubeVideo::setRegionRestrictionBlacklist(const std::list<std::string> &mRegionRestrictionBlacklist) {
        m_regionRestrictionBlacklist = mRegionRestrictionBlacklist;
    }

    bool YoutubeVideo::isMIs360() const {
        return m_is360;
    }

    void YoutubeVideo::setIs360(bool mIs360) {
        m_is360 = mIs360;
    }

    bool YoutubeVideo::isMIsRectanguar() const {
        return m_isRectanguar;
    }

    void YoutubeVideo::setIsRectanguar(bool mIsRectanguar) {
        m_isRectanguar = mIsRectanguar;
    }

    bool YoutubeVideo::isMHasCustomThumbnail() const {
        return m_hasCustomThumbnail;
    }

    void YoutubeVideo::setHasCustomThumbnail(bool mHasCustomThumbnail) {
        m_hasCustomThumbnail = mHasCustomThumbnail;
    }

    const std::string &YoutubeVideo::getUploadStatus() const {
        return m_uploadStatus;
    }

    void YoutubeVideo::setUploadStatus(const std::string &mUploadStatus) {
        m_uploadStatus = mUploadStatus;
    }

    const std::string &YoutubeVideo::getFailureReason() const {
        return m_failureReason;
    }

    void YoutubeVideo::setFailureReason(const std::string &mFailureReason) {
        m_failureReason = mFailureReason;
    }

    const std::string &YoutubeVideo::getRejectionReason() const {
        return m_rejectionReason;
    }

    void YoutubeVideo::setRejectionReason(const std::string &mRejectionReason) {
        m_rejectionReason = mRejectionReason;
    }

    const std::string &YoutubeVideo::getPrivacyStatus() const {
        return m_privacyStatus;
    }

    void YoutubeVideo::setPrivacyStatus(const std::string &mPrivacyStatus) {
        m_privacyStatus = mPrivacyStatus;
    }

    const std::string &YoutubeVideo::getPublishAt() const {
        return m_publishAt;
    }

    void YoutubeVideo::setPublishAt(const std::string &mPublishAt) {
        m_publishAt = mPublishAt;
    }

    const std::string &YoutubeVideo::getLicense() const {
        return m_license;
    }

    void YoutubeVideo::setLicense(const std::string &mLicense) {
        m_license = mLicense;
    }

    bool YoutubeVideo::isMIsEmbeddable() const {
        return m_isEmbeddable;
    }

    void YoutubeVideo::setIsEmbeddable(bool mIsEmbeddable) {
        m_isEmbeddable = mIsEmbeddable;
    }

    bool YoutubeVideo::isMPublicStatsViewable() const {
        return m_publicStatsViewable;
    }

    void YoutubeVideo::setPublicStatsViewable(bool mPublicStatsViewable) {
        m_publicStatsViewable = mPublicStatsViewable;
    }

    unsigned long YoutubeVideo::getViewCount() const {
        return m_viewCount;
    }

    void YoutubeVideo::setViewCount(unsigned long mViewCount) {
        m_viewCount = mViewCount;
    }

    unsigned long YoutubeVideo::getLikeCount() const {
        return m_likeCount;
    }

    void YoutubeVideo::setLikeCount(unsigned long mLikeCount) {
        m_likeCount = mLikeCount;
    }

    unsigned long YoutubeVideo::getDislikeCount() const {
        return m_dislikeCount;
    }

    void YoutubeVideo::setDislikeCount(unsigned long mDislikeCount) {
        m_dislikeCount = mDislikeCount;
    }

    unsigned long YoutubeVideo::getCommentCount() const {
        return m_commentCount;
    }

    void YoutubeVideo::setCommentCount(unsigned long commentCount) {
        YoutubeVideo::m_commentCount = commentCount;
    }

    const player_t &YoutubeVideo::getPlayer() const {
        return m_player;
    }

    void YoutubeVideo::setPlayer(const player_t &mPlayer) {
        m_player = mPlayer;
    }

    const std::list<std::string> &YoutubeVideo::getTopicCategories() const {
        return m_topicCategories;
    }

    void YoutubeVideo::setTopicCategories(const std::list<std::string> &mTopicCategories) {
        m_topicCategories = mTopicCategories;
    }

    const std::string &YoutubeVideo::getLocation() const {
        return m_location;
    }

    void YoutubeVideo::setLocation(const std::string &mLocation) {
        m_location = mLocation;
    }

    const std::string &YoutubeVideo::getRecordingDate() const {
        return m_recordingDate;
    }

    void YoutubeVideo::setRecordingDate(const std::string &mRecordingDate) {
        m_recordingDate = mRecordingDate;
    }

    const std::string &YoutubeVideo::getFileName() const {
        return m_fileName;
    }

    void YoutubeVideo::setFileName(const std::string &mFileName) {
        m_fileName = mFileName;
    }

    unsigned long YoutubeVideo::getFileSize() const {
        return m_fileSize;
    }

    void YoutubeVideo::setFileSize(unsigned long mFileSize) {
        m_fileSize = mFileSize;
    }

    const std::string &YoutubeVideo::getFileType() const {
        return m_fileType;
    }

    void YoutubeVideo::setFileType(const std::string &mFileType) {
        m_fileType = mFileType;
    }

    const std::string &YoutubeVideo::getContainer() const {
        return m_container;
    }

    void YoutubeVideo::setContainer(const std::string &mContainer) {
        m_container = mContainer;
    }

    const std::list<videoStream_t> &YoutubeVideo::getVideoStreams() const {
        return m_videoStreams;
    }

    void YoutubeVideo::setVideoStreams(const std::list<videoStream_t> &mVideoStreams) {
        m_videoStreams = mVideoStreams;
    }

    const std::list<audioStream_t> &YoutubeVideo::getAudioStreams() const {
        return m_audioStreams;
    }

    void YoutubeVideo::setAudioStreams(const std::list<audioStream_t> &mAudioStreams) {
        m_audioStreams = mAudioStreams;
    }

    unsigned long YoutubeVideo::getDurationMs() const {
        return m_durationMs;
    }

    void YoutubeVideo::setDurationMs(unsigned long mDurationMs) {
        m_durationMs = mDurationMs;
    }

    unsigned long YoutubeVideo::getBitrateBps() const {
        return m_bitrateBps;
    }

    void YoutubeVideo::setBitrateBps(unsigned long mBitrateBps) {
        m_bitrateBps = mBitrateBps;
    }

    const std::string &YoutubeVideo::getCreationTime() const {
        return m_creationTime;
    }

    void YoutubeVideo::setCreationTime(const std::string &mCreationTime) {
        m_creationTime = mCreationTime;
    }

    const std::string &YoutubeVideo::getProcessingStatus() const {
        return m_processingStatus;
    }

    void YoutubeVideo::setProcessingStatus(const std::string &mProcessingStatus) {
        m_processingStatus = mProcessingStatus;
    }

    const processingProgress_t &YoutubeVideo::getProcessingProgress() const {
        return m_processingProgress;
    }

    void YoutubeVideo::setProcessingProgress(const processingProgress_t &mProcessingProgress) {
        m_processingProgress = mProcessingProgress;
    }

    const std::string &YoutubeVideo::getProcessingFailureReason() const {
        return m_processingFailureReason;
    }

    void YoutubeVideo::setProcessingFailureReason(const std::string &mProcessingFailureReason) {
        m_processingFailureReason = mProcessingFailureReason;
    }

    const std::string &YoutubeVideo::getProcessingIssuesAvailability() const {
        return m_processingIssuesAvailability;
    }

    void YoutubeVideo::setProcessingIssuesAvailability(const std::string &mProcessingIssuesAvailability) {
        m_processingIssuesAvailability = mProcessingIssuesAvailability;
    }

    const std::string &YoutubeVideo::getTagSuggestionsAvailability() const {
        return m_tagSuggestionsAvailability;
    }

    void YoutubeVideo::setTagSuggestionsAvailability(const std::string &mTagSuggestionsAvailability) {
        m_tagSuggestionsAvailability = mTagSuggestionsAvailability;
    }

    const std::string &YoutubeVideo::getEditorSuggestionsAvailability() const {
        return m_editorSuggestionsAvailability;
    }

    void YoutubeVideo::setEditorSuggestionsAvailability(const std::string &mEditorSuggestionsAvailability) {
        m_editorSuggestionsAvailability = mEditorSuggestionsAvailability;
    }

    const std::string &YoutubeVideo::getThumbnailsAvailability() const {
        return m_thumbnailsAvailability;
    }

    void YoutubeVideo::setThumbnailsAvailability(const std::string &mThumbnailsAvailability) {
        m_thumbnailsAvailability = mThumbnailsAvailability;
    }

    const std::list<std::string> &YoutubeVideo::getProcessingErrors() const {
        return m_processingErrors;
    }

    void YoutubeVideo::setProcessingErrors(const std::list<std::string> &mProcessingErrors) {
        m_processingErrors = mProcessingErrors;
    }

    const std::list<std::string> &YoutubeVideo::getProcessingWarnings() const {
        return m_processingWarnings;
    }

    void YoutubeVideo::setProcessingWarnings(const std::list<std::string> &mProcessingWarnings) {
        m_processingWarnings = mProcessingWarnings;
    }

    const std::list<std::string> &YoutubeVideo::getProcessingHints() const {
        return m_processingHints;
    }

    void YoutubeVideo::setProcessingHints(const std::list<std::string> &mProcessingHints) {
        m_processingHints = mProcessingHints;
    }

    const std::list<tagSuggestions_t> &YoutubeVideo::getTagSuggestions() const {
        return m_tagSuggestions;
    }

    void YoutubeVideo::setTagSuggestions(const std::list<tagSuggestions_t> &mTagSuggestions) {
        m_tagSuggestions = mTagSuggestions;
    }

    const std::list<std::string> &YoutubeVideo::getEditorSuggestions() const {
        return m_editorSuggestions;
    }

    void YoutubeVideo::setEditorSuggestions(const std::list<std::string> &mEditorSuggestions) {
        m_editorSuggestions = mEditorSuggestions;
    }

    const std::list<liveStreamingDetails_t> &YoutubeVideo::getLiveStreamingDetails() const {
        return m_liveStreamingDetails;
    }

    void YoutubeVideo::setLiveStreamingDetails(const std::list<liveStreamingDetails_t> &mLiveStreamingDetails) {
        m_liveStreamingDetails = mLiveStreamingDetails;
    }

    const std::list<localization_t> &YoutubeVideo::getLocalizations() const {
        return m_localizations;
    }

    void YoutubeVideo::setLocalizations(const std::list<localization_t> &mLocalizations) {
        m_localizations = mLocalizations;
    }

    // END: Getters & Setters.

    void YoutubeVideo::print(int indentationSpacing = 0) { // FIXME: IMPLEMENT
        std::string indentation(indentationSpacing, ' ');

//        std::cout << indentation << "Title: " << getTitle() << std::endl;
//        std::cout << indentation << "ID: " << getId() << std::endl;
    }

    void YoutubeVideo::addFromJson(nlohmann::json t_json) {
        try {
            // General.
            setId(t_json["id"]);

            // Add properties to various parts (if they exist).

            // Part: Snippet.
            if (t_json.find("snippet") != t_json.end()) {

            }

            // Part: Content details.
            if (t_json.find("contentDetails") != t_json.end()) {

            }

            // Part: Status.
            if (t_json.find("status") != t_json.end()) {

            }

            // Part: Statistics
            if (t_json.find("statistics") != t_json.end()) {

            }

            // Part: Player
            if (t_json.find("player") != t_json.end()) {

            }

            // Part: Topic details
            if (t_json.find("topicDetails") != t_json.end()) {

            }

            // Part: Recording details.
            if (t_json.find("recordingDetails") != t_json.end()) {

            }

            // Part: File details.
            if (t_json.find("fileDetails") != t_json.end()) {

            }

            // Part: Processing details.
            if (t_json.find("processingDetails") != t_json.end()) {

            }

            // Part: Suggestions
            if (t_json.find("suggestions") != t_json.end()) {

            }

            // Part: Live streaming details.
            if (t_json.find("liveStreamingDetails") != t_json.end()) {

            }

            // Part: Localizations.
            if (t_json.find("localizations") != t_json.end()) {

            }
        } catch (nlohmann::detail::type_error &exc) {
            addError("Skipping YoutubeVideo::addFromJson due to Exception: " + std::string(exc.what()), t_json);
        } catch (const std::exception &exc) {
            addError("Skipping YoutubeVideo::addFromJson due to Unexpected Exception: " +
                     std::string(exc.what()), t_json);
        }
    }
} // namespace sane
