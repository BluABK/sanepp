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

    void YoutubeVideo::setPublishedAt(const std::string &t_publishedAt) {
        m_publishedAt = t_publishedAt;
    }

    const std::string &YoutubeVideo::getChannelId() const {
        return m_channelId;
    }

    void YoutubeVideo::setChannelId(const std::string &t_channelId) {
        m_channelId = t_channelId;
    }

    const std::string &YoutubeVideo::getTitle() const {
        return m_title;
    }

    void YoutubeVideo::setTitle(const std::string &t_title) {
        m_title = t_title;
    }

    const std::string &YoutubeVideo::getDescription() const {
        return m_description;
    }

    void YoutubeVideo::setDescription(const std::string &t_description) {
        m_description = t_description;
    }

    const std::map<std::string, thumbnail_t> &YoutubeVideo::getThumbnails() const {
        return m_thumbnails;
    }

    void YoutubeVideo::setThumbnails(const std::map<std::string, thumbnail_t> &t_thumbnails) {
        m_thumbnails = t_thumbnails;
    }

    const std::string &YoutubeVideo::getChannelTitle() const {
        return m_channelTitle;
    }

    void YoutubeVideo::setChannelTitle(const std::string &t_channelTitle) {
        m_channelTitle = t_channelTitle;
    }

    const std::list<std::string> &YoutubeVideo::getTags() const {
        return m_tags;
    }

    void YoutubeVideo::setTags(const std::list<std::string> &t_tags) {
        m_tags = t_tags;
    }

    const std::string &YoutubeVideo::getCategoryId() const {
        return m_categoryId;
    }

    void YoutubeVideo::setCategoryId(const std::string &t_categoryId) {
        m_categoryId = t_categoryId;
    }

    const std::string &YoutubeVideo::getLiveBroadcastContent() const {
        return m_liveBroadcastContent;
    }

    void YoutubeVideo::setLiveBroadcastContent(const std::string &t_liveBroadcastContent) {
        m_liveBroadcastContent = t_liveBroadcastContent;
    }

    const std::string &YoutubeVideo::getDefaultLanguage() const {
        return m_defaultLanguage;
    }

    void YoutubeVideo::setDefaultLanguage(const std::string &t_defaultLanguage) {
        m_defaultLanguage = t_defaultLanguage;
    }

    const std::string &YoutubeVideo::getLocalizedTitle() const {
        return m_localizedTitle;
    }

    void YoutubeVideo::setLocalizedTitle(const std::string &t_localizedTitle) {
        m_localizedTitle = t_localizedTitle;
    }

    const std::string &YoutubeVideo::getLocalizedDescription() const {
        return m_localizedDescription;
    }

    void YoutubeVideo::setLocalizedDescription(const std::string &t_localizedDescription) {
        m_localizedDescription = t_localizedDescription;
    }

    const std::string &YoutubeVideo::getDefaultAudioLanguage() const {
        return m_defaultAudioLanguage;
    }

    void YoutubeVideo::setDefaultAudioLanguage(const std::string &t_defaultAudioLanguage) {
        m_defaultAudioLanguage = t_defaultAudioLanguage;
    }

    const std::string &YoutubeVideo::getDuration() const {
        return m_duration;
    }

    void YoutubeVideo::setDuration(const std::string &t_duration) {
        m_duration = t_duration;
    }

    bool YoutubeVideo::is3D() const {
        return m_is3D;
    }

    void YoutubeVideo::setIs3D(bool t_is3D) {
        m_is3D = t_is3D;
    }

    bool YoutubeVideo::is2D() const {
        return m_is2D;
    }

    void YoutubeVideo::setIs2D(bool t_is2D) {
        m_is2D = t_is2D;
    }

    bool YoutubeVideo::isHD() const {
        return m_isHD;
    }

    void YoutubeVideo::setIsHd(bool t_isHD) {
        m_isHD = t_isHD;
    }

    bool YoutubeVideo::hasCaptions() const {
        return m_hasCaptions;
    }

    void YoutubeVideo::setHasCaptions(bool t_hasCaptions) {
        m_hasCaptions = t_hasCaptions;
    }

    bool YoutubeVideo::isLicensedContent() const {
        return m_isLicensedContent;
    }

    void YoutubeVideo::setIsLicensedContent(bool t_isLicensedContent) {
        m_isLicensedContent = t_isLicensedContent;
    }

    const std::list<std::string> &YoutubeVideo::getRegionRestrictionWhitelist() const {
        return m_regionRestrictionWhitelist;
    }

    void YoutubeVideo::setRegionRestrictionWhitelist(const std::list<std::string> &t_regionRestrictionWhitelist) {
        m_regionRestrictionWhitelist = t_regionRestrictionWhitelist;
    }

    const std::list<std::string> &YoutubeVideo::getRegionRestrictionBlacklist() const {
        return m_regionRestrictionBlacklist;
    }

    void YoutubeVideo::setRegionRestrictionBlacklist(const std::list<std::string> &t_regionRestrictionBlacklist) {
        m_regionRestrictionBlacklist = t_regionRestrictionBlacklist;
    }

    bool YoutubeVideo::is360() const {
        return m_is360;
    }

    void YoutubeVideo::setIs360(bool t_is360) {
        m_is360 = t_is360;
    }

    bool YoutubeVideo::isRectanguar() const {
        return m_isRectanguar;
    }

    void YoutubeVideo::setIsRectanguar(bool t_isRectanguar) {
        m_isRectanguar = t_isRectanguar;
    }

    bool YoutubeVideo::hasCustomThumbnail() const {
        return m_hasCustomThumbnail;
    }

    void YoutubeVideo::setHasCustomThumbnail(bool t_hasCustomThumbnail) {
        m_hasCustomThumbnail = t_hasCustomThumbnail;
    }

    const std::string &YoutubeVideo::getUploadStatus() const {
        return m_uploadStatus;
    }

    void YoutubeVideo::setUploadStatus(const std::string &t_uploadStatus) {
        m_uploadStatus = t_uploadStatus;
    }

    const std::string &YoutubeVideo::getFailureReason() const {
        return m_failureReason;
    }

    void YoutubeVideo::setFailureReason(const std::string &t_failureReason) {
        m_failureReason = t_failureReason;
    }

    const std::string &YoutubeVideo::getRejectionReason() const {
        return m_rejectionReason;
    }

    void YoutubeVideo::setRejectionReason(const std::string &t_rejectionReason) {
        m_rejectionReason = t_rejectionReason;
    }

    const std::string &YoutubeVideo::getPrivacyStatus() const {
        return m_privacyStatus;
    }

    void YoutubeVideo::setPrivacyStatus(const std::string &t_privacyStatus) {
        m_privacyStatus = t_privacyStatus;
    }

    const std::string &YoutubeVideo::getPublishAt() const {
        return m_publishAt;
    }

    void YoutubeVideo::setPublishAt(const std::string &t_publishAt) {
        m_publishAt = t_publishAt;
    }

    const std::string &YoutubeVideo::getLicense() const {
        return m_license;
    }

    void YoutubeVideo::setLicense(const std::string &t_license) {
        m_license = t_license;
    }

    bool YoutubeVideo::isEmbeddable() const {
        return m_isEmbeddable;
    }

    void YoutubeVideo::setIsEmbeddable(bool t_isEmbeddable) {
        m_isEmbeddable = t_isEmbeddable;
    }

    bool YoutubeVideo::isPublicStatsViewable() const {
        return m_publicStatsViewable;
    }

    void YoutubeVideo::setPublicStatsViewable(bool t_isPublicStatsViewable) {
        m_publicStatsViewable = t_isPublicStatsViewable;
    }

    unsigned long YoutubeVideo::getViewCount() const {
        return m_viewCount;
    }

    void YoutubeVideo::setViewCount(unsigned long t_viewCount) {
        m_viewCount = t_viewCount;
    }

    unsigned long YoutubeVideo::getLikeCount() const {
        return m_likeCount;
    }

    void YoutubeVideo::setLikeCount(unsigned long t_likeCount) {
        m_likeCount = t_likeCount;
    }

    unsigned long YoutubeVideo::getDislikeCount() const {
        return m_dislikeCount;
    }

    void YoutubeVideo::setDislikeCount(unsigned long t_dislikeCount) {
        m_dislikeCount = t_dislikeCount;
    }

    unsigned long YoutubeVideo::getCommentCount() const {
        return m_commentCount;
    }

    void YoutubeVideo::setCommentCount(unsigned long t_commentCount) {
        m_commentCount = t_commentCount;
    }

    const player_t &YoutubeVideo::getPlayer() const {
        return m_player;
    }

    void YoutubeVideo::setPlayer(const player_t &t_player) {
        m_player = t_player;
    }

    const std::list<std::string> &YoutubeVideo::getTopicCategories() const {
        return m_topicCategories;
    }

    void YoutubeVideo::setTopicCategories(const std::list<std::string> &t_topicCategories) {
        m_topicCategories = t_topicCategories;
    }

    const std::string &YoutubeVideo::getLocation() const {
        return m_location;
    }

    void YoutubeVideo::setLocation(const std::string &t_location) {
        m_location = t_location;
    }

    const std::string &YoutubeVideo::getRecordingDate() const {
        return m_recordingDate;
    }

    void YoutubeVideo::setRecordingDate(const std::string &t_recordingDate) {
        m_recordingDate = t_recordingDate;
    }

    const std::string &YoutubeVideo::getFileName() const {
        return m_fileName;
    }

    void YoutubeVideo::setFileName(const std::string &t_fileName) {
        m_fileName = t_fileName;
    }

    unsigned long YoutubeVideo::getFileSize() const {
        return m_fileSize;
    }

    void YoutubeVideo::setFileSize(unsigned long t_fileSize) {
        m_fileSize = t_fileSize;
    }

    const std::string &YoutubeVideo::getFileType() const {
        return m_fileType;
    }

    void YoutubeVideo::setFileType(const std::string &t_fileType) {
        m_fileType = t_fileType;
    }

    const std::string &YoutubeVideo::getContainer() const {
        return m_container;
    }

    void YoutubeVideo::setContainer(const std::string &t_container) {
        m_container = t_container;
    }

    const std::list<videoStream_t> &YoutubeVideo::getVideoStreams() const {
        return m_videoStreams;
    }

    void YoutubeVideo::setVideoStreams(const std::list<videoStream_t> &t_videoStreams) {
        m_videoStreams = t_videoStreams;
    }

    const std::list<audioStream_t> &YoutubeVideo::getAudioStreams() const {
        return m_audioStreams;
    }

    void YoutubeVideo::setAudioStreams(const std::list<audioStream_t> &t_audioStreams) {
        m_audioStreams = t_audioStreams;
    }

    unsigned long YoutubeVideo::getDurationMs() const {
        return m_durationMs;
    }

    void YoutubeVideo::setDurationMs(unsigned long t_durationMs) {
        m_durationMs = t_durationMs;
    }

    unsigned long YoutubeVideo::getBitrateBps() const {
        return m_bitrateBps;
    }

    void YoutubeVideo::setBitrateBps(unsigned long t_bitrateBps) {
        m_bitrateBps = t_bitrateBps;
    }

    const std::string &YoutubeVideo::getCreationTime() const {
        return m_creationTime;
    }

    void YoutubeVideo::setCreationTime(const std::string &t_creationTime) {
        m_creationTime = t_creationTime;
    }

    const std::string &YoutubeVideo::getProcessingStatus() const {
        return m_processingStatus;
    }

    void YoutubeVideo::setProcessingStatus(const std::string &t_processingStatus) {
        m_processingStatus = t_processingStatus;
    }

    const processingProgress_t &YoutubeVideo::getProcessingProgress() const {
        return m_processingProgress;
    }

    void YoutubeVideo::setProcessingProgress(const processingProgress_t &t_processingProgress) {
        m_processingProgress = t_processingProgress;
    }

    const std::string &YoutubeVideo::getProcessingFailureReason() const {
        return m_processingFailureReason;
    }

    void YoutubeVideo::setProcessingFailureReason(const std::string &t_processingFailureReason) {
        m_processingFailureReason = t_processingFailureReason;
    }

    const std::string &YoutubeVideo::getProcessingIssuesAvailability() const {
        return m_processingIssuesAvailability;
    }

    void YoutubeVideo::setProcessingIssuesAvailability(const std::string &t_processingIssuesAvailability) {
        m_processingIssuesAvailability = t_processingIssuesAvailability;
    }

    const std::string &YoutubeVideo::getTagSuggestionsAvailability() const {
        return m_tagSuggestionsAvailability;
    }

    void YoutubeVideo::setTagSuggestionsAvailability(const std::string &t_tagSuggestionsAvailability) {
        m_tagSuggestionsAvailability = t_tagSuggestionsAvailability;
    }

    const std::string &YoutubeVideo::getEditorSuggestionsAvailability() const {
        return m_editorSuggestionsAvailability;
    }

    void YoutubeVideo::setEditorSuggestionsAvailability(const std::string &t_editorSuggestionsAvailability) {
        m_editorSuggestionsAvailability = t_editorSuggestionsAvailability;
    }

    const std::string &YoutubeVideo::getThumbnailsAvailability() const {
        return m_thumbnailsAvailability;
    }

    void YoutubeVideo::setThumbnailsAvailability(const std::string &t_thumbnailsAvailability) {
        m_thumbnailsAvailability = t_thumbnailsAvailability;
    }

    const std::list<std::string> &YoutubeVideo::getProcessingErrors() const {
        return m_processingErrors;
    }

    void YoutubeVideo::setProcessingErrors(const std::list<std::string> &t_processingErrors) {
        m_processingErrors = t_processingErrors;
    }

    const std::list<std::string> &YoutubeVideo::getProcessingWarnings() const {
        return m_processingWarnings;
    }

    void YoutubeVideo::setProcessingWarnings(const std::list<std::string> &t_processingWarnings) {
        m_processingWarnings = t_processingWarnings;
    }

    const std::list<std::string> &YoutubeVideo::getProcessingHints() const {
        return m_processingHints;
    }

    void YoutubeVideo::setProcessingHints(const std::list<std::string> &t_processingHints) {
        m_processingHints = t_processingHints;
    }

    const std::list<tagSuggestions_t> &YoutubeVideo::getTagSuggestions() const {
        return m_tagSuggestions;
    }

    void YoutubeVideo::setTagSuggestions(const std::list<tagSuggestions_t> &t_tagSuggestions) {
        m_tagSuggestions = t_tagSuggestions;
    }

    const std::list<std::string> &YoutubeVideo::getEditorSuggestions() const {
        return m_editorSuggestions;
    }

    void YoutubeVideo::setEditorSuggestions(const std::list<std::string> &t_editorSuggestions) {
        m_editorSuggestions = t_editorSuggestions;
    }

    const std::list<liveStreamingDetails_t> &YoutubeVideo::getLiveStreamingDetails() const {
        return m_liveStreamingDetails;
    }

    void YoutubeVideo::setLiveStreamingDetails(const std::list<liveStreamingDetails_t> &t_liveStreamingDetails) {
        m_liveStreamingDetails = t_liveStreamingDetails;
    }

    const std::list<localization_t> &YoutubeVideo::getLocalizations() const {
        return m_localizations;
    }

    void YoutubeVideo::setLocalizations(const std::list<localization_t> &t_localizations) {
        m_localizations = t_localizations;
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
