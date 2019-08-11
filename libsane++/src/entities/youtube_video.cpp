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
     * @param fullJson              OPTIONAL: The complete JSON object, for use in debugging/error reporting.
     */
    void YoutubeVideo::assignJsonStringValue(std::string &stringToAssignValue,
                                               nlohmann::json &unknownJsonTypeValue, nlohmann::json t_json) {
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

    bool YoutubeVideo::isBool(nlohmann::json &t_bool) {
        if (!t_bool.empty()) {
            if (t_bool.is_boolean()) {
                return true;
            } else if (t_bool.is_string()) {
                if (t_bool.get<std::string>() == "true" or t_bool.get<std::string>() == "false") {
                    return true;
                }
            }
        }
        return false;
    }

    bool YoutubeVideo::getJsonBoolValue(nlohmann::json &t_bool) {
        if (t_bool.is_boolean()) {
            return t_bool.get<bool>();
        } else if (t_bool.is_string()) {
            return t_bool.get<std::string>() == "true";
        } else {
            addError("Called getJsonBoolValue with invalid bool, forced to return false!", t_bool);
            return false;
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

    void YoutubeVideo::setId(nlohmann::json t_id) {
        assignJsonStringValue(m_id, t_id);
    }

    void YoutubeVideo::setId(const std::string &t_id) {
        m_id = t_id;
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

    void YoutubeVideo::setPublishedAt(nlohmann::json &t_publishedAt) {
        assignJsonStringValue(m_publishedAt, t_publishedAt);
    }

    const std::string &YoutubeVideo::getChannelId() const {
        return m_channelId;
    }

    void YoutubeVideo::setChannelId(const std::string &t_channelId) {
        m_channelId = t_channelId;
    }

    void YoutubeVideo::setChannelId(nlohmann::json &t_channelId) {
        assignJsonStringValue(m_channelId, t_channelId);
    }

    const std::string &YoutubeVideo::getTitle() const {
        return m_title;
    }

    void YoutubeVideo::setTitle(const std::string &t_title) {
        m_title = t_title;
    }

    void YoutubeVideo::setTitle(nlohmann::json &t_title) {
        assignJsonStringValue(m_title, t_title);
    }

    const std::string &YoutubeVideo::getDescription() const {
        return m_description;
    }

    void YoutubeVideo::setDescription(const std::string &t_description) {
        m_description = t_description;
    }

    void YoutubeVideo::setDescription(nlohmann::json &t_description) {
        assignJsonStringValue(m_description, t_description);
    }

    const std::map<std::string, thumbnail_t> &YoutubeVideo::getThumbnails() const {
        return m_thumbnails;
    }

    void YoutubeVideo::setThumbnails(const std::map<std::string, thumbnail_t> &t_thumbnails) {
        m_thumbnails = t_thumbnails;
    }

    void YoutubeVideo::setThumbnails(nlohmann::json &t_thumbnails) {
        // Define variables.
        std::map<std::string, thumbnail_t> thumbnails;

        // Create each thumbnail struct.
        thumbnail_t defaultThumbnail;
        assignJsonStringValue(defaultThumbnail.url, t_thumbnails["default"]);
        defaultThumbnail.height  = t_thumbnails["default"]["height"].get<unsigned int>();
        defaultThumbnail.width   = t_thumbnails["default"]["width"].get<unsigned int>();

        thumbnail_t highThumbnail;
        assignJsonStringValue(highThumbnail.url, t_thumbnails["high"]);
        highThumbnail.height     = t_thumbnails["high"]["height"].get<unsigned int>();
        highThumbnail.width      = t_thumbnails["high"]["width"].get<unsigned int>();

        thumbnail_t mediumThumbnail;
        assignJsonStringValue(mediumThumbnail.url, t_thumbnails["medium"]);
        mediumThumbnail.height   = t_thumbnails["medium"]["height"].get<unsigned int>();
        mediumThumbnail.width    = t_thumbnails["medium"]["width"].get<unsigned int>();

        thumbnail_t standardThumbnail;
        assignJsonStringValue(standardThumbnail.url, t_thumbnails["standard"]);
        standardThumbnail.height = t_thumbnails["standard"]["height"].get<unsigned int>();
        standardThumbnail.width  = t_thumbnails["standard"]["width"].get<unsigned int>();

        thumbnail_t maxresThumbnail;
        assignJsonStringValue(maxresThumbnail.url, t_thumbnails["maxres"]);
        maxresThumbnail.height  = t_thumbnails["maxres"]["height"].get<unsigned int>();
        maxresThumbnail.width   = t_thumbnails["maxres"]["width"].get<unsigned int>();

        // Add thumbnail structs to map.
        thumbnails["default"] = defaultThumbnail;
        thumbnails["high"] = highThumbnail;
        thumbnails["medium"] = mediumThumbnail;
        thumbnails["standard"] = standardThumbnail;
        thumbnails["maxres"] = maxresThumbnail;

        // Finally assign the thumbnails map to the object property.
        m_thumbnails = thumbnails;
    }

    const std::string &YoutubeVideo::getChannelTitle() const {
        return m_channelTitle;
    }

    void YoutubeVideo::setChannelTitle(const std::string &t_channelTitle) {
        m_channelTitle = t_channelTitle;
    }

    void YoutubeVideo::setChannelTitle(nlohmann::json &t_channelTitle) {
        assignJsonStringValue(m_channelTitle, t_channelTitle);
    }

    const std::list<std::string> &YoutubeVideo::getTags() const {
        return m_tags;
    }

    void YoutubeVideo::setTags(const std::list<std::string> &t_tags) {
        m_tags = t_tags;
    }

    void YoutubeVideo::setTags(nlohmann::json &t_tags) {
        std::list<std::string> tags;

        if (t_tags.empty()) {
            return;
        }

        for (nlohmann::json tag : t_tags.items()) {
            tags.push_back(tag.get<std::string>());
        }

        m_tags = tags;
    }

    const std::string &YoutubeVideo::getCategoryId() const {
        return m_categoryId;
    }

    void YoutubeVideo::setCategoryId(const std::string &t_categoryId) {
        m_categoryId = t_categoryId;
    }

    void YoutubeVideo::setCategoryId(nlohmann::json &t_categoryId) {
        assignJsonStringValue(m_categoryId, t_categoryId);
    }

    const std::string &YoutubeVideo::getLiveBroadcastContent() const {
        return m_liveBroadcastContent;
    }

    void YoutubeVideo::setLiveBroadcastContent(const std::string &t_liveBroadcastContent) {
        m_liveBroadcastContent = t_liveBroadcastContent;
    }

    void YoutubeVideo::setLiveBroadcastContent(nlohmann::json &t_liveBroadcastContent) {
        assignJsonStringValue(m_liveBroadcastContent, t_liveBroadcastContent);
    }

    const std::string &YoutubeVideo::getDefaultLanguage() const {
        return m_defaultLanguage;
    }

    void YoutubeVideo::setDefaultLanguage(const std::string &t_defaultLanguage) {
        m_defaultLanguage = t_defaultLanguage;
    }

    void YoutubeVideo::setDefaultLanguage(nlohmann::json &t_defaultLanguage) {
        assignJsonStringValue(m_defaultLanguage, t_defaultLanguage);
    }

    const std::string &YoutubeVideo::getLocalizedTitle() const {
        return m_localizedTitle;
    }

    void YoutubeVideo::setLocalizedTitle(const std::string &t_localizedTitle) {
        m_localizedTitle = t_localizedTitle;
    }

    void YoutubeVideo::setLocalizedTitle(nlohmann::json &t_localizedTitle) {
        assignJsonStringValue(m_localizedTitle, t_localizedTitle);
    }

    const std::string &YoutubeVideo::getLocalizedDescription() const {
        return m_localizedDescription;
    }

    void YoutubeVideo::setLocalizedDescription(const std::string &t_localizedDescription) {
        m_localizedDescription = t_localizedDescription;
    }

    void YoutubeVideo::setLocalizedDescription(nlohmann::json &t_localizedDescription) {
        assignJsonStringValue(m_localizedDescription, t_localizedDescription);
    }

    const std::string &YoutubeVideo::getDefaultAudioLanguage() const {
        return m_defaultAudioLanguage;
    }

    void YoutubeVideo::setDefaultAudioLanguage(const std::string &t_defaultAudioLanguage) {
        m_defaultAudioLanguage = t_defaultAudioLanguage;
    }

    void YoutubeVideo::setDefaultAudioLanguage(nlohmann::json &t_defaultAudioLanguage) {
        assignJsonStringValue(m_defaultAudioLanguage, t_defaultAudioLanguage);
    }

    const std::string &YoutubeVideo::getDuration() const {
        return m_duration;
    }

    void YoutubeVideo::setDuration(const std::string &t_duration) {
        m_duration = t_duration;
    }

    void YoutubeVideo::setDuration(nlohmann::json &t_duration) {
        assignJsonStringValue(m_duration, t_duration);
    }

    void YoutubeVideo::setDimension(nlohmann::json &t_dimension) {
        // Determine is video is 2D or 3D;
        if (!t_dimension.empty() and t_dimension.is_string()) {
            std::string dimension = t_dimension.get<std::string>();

            if (dimension == "2d") {
                setIs2D(true);
                setIs3D(false);
            } else if (dimension == "3d") {
                setIs2D(false);
                setIs3D(true);
            } else {
                // If somehow a third dimension appears:
                addError("setDimension: Unexpected dimension '" + dimension + "'!", t_dimension);
            }
        }
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

    void YoutubeVideo::setDefinition(nlohmann::json &t_definition) {
        // Determine is video is 2D or 3D;
        if (!t_definition.empty() and t_definition.is_string()) {
            std::string definition = t_definition.get<std::string>();

            if (definition == "hd") {
                setIsHD(true);
            } else if (definition == "sd") {
                setIsHD(false);
            } else {
                // If somehow a third definition appears:
                addError("setDefinition: Unexpected definition '" + definition + "'!", t_definition);
            }
        }
    }

    bool YoutubeVideo::isHD() const {
        return m_isHD;
    }

    void YoutubeVideo::setIsHD(bool t_isHD) {
        m_isHD = t_isHD;
    }

    bool YoutubeVideo::hasCaptions() const {
        return m_hasCaptions;
    }

    void YoutubeVideo::setHasCaptions(bool t_hasCaptions) {
        m_hasCaptions = t_hasCaptions;
    }

    void YoutubeVideo::setHasCaptions(nlohmann::json &t_hasCaptions) {
        if (isBool(t_hasCaptions)) {
            setHasCaptions(getJsonBoolValue(t_hasCaptions));
        }
    }

    bool YoutubeVideo::isLicensedContent() const {
        return m_isLicensedContent;
    }

    void YoutubeVideo::setIsLicensedContent(bool t_isLicensedContent) {
        m_isLicensedContent = t_isLicensedContent;
    }

    void YoutubeVideo::setIsLicensedContent(nlohmann::json &t_isLicensedContent) {
        if (isBool(t_isLicensedContent)) {
            setIsLicensedContent(getJsonBoolValue(t_isLicensedContent));
        }
    }

    const std::list<std::string> &YoutubeVideo::getRegionRestrictionWhitelist() const {
        return m_regionRestrictionWhitelist;
    }

    void YoutubeVideo::setRegionRestrictionWhitelist(const std::list<std::string> &t_regionRestrictionWhitelist) {
        m_regionRestrictionWhitelist = t_regionRestrictionWhitelist;
    }

    void YoutubeVideo::setRegionRestrictionWhitelist(nlohmann::json &t_regionRestrictionWhitelist) {
        std::list<std::string> whitelist;

        for (const nlohmann::json& countryCode : t_regionRestrictionWhitelist) {
            whitelist.push_back(countryCode.get<std::string>());
        }

        m_regionRestrictionWhitelist = whitelist;
    }

    const std::list<std::string> &YoutubeVideo::getRegionRestrictionBlacklist() const {
        return m_regionRestrictionBlacklist;
    }

    void YoutubeVideo::setRegionRestrictionBlacklist(const std::list<std::string> &t_regionRestrictionBlacklist) {
        m_regionRestrictionBlacklist = t_regionRestrictionBlacklist;
    }

    void YoutubeVideo::setRegionRestrictionBlacklist(nlohmann::json &t_regionRestrictionBlacklist) {
        std::list<std::string> blacklist;

        for (const nlohmann::json& countryCode : t_regionRestrictionBlacklist) {
            blacklist.push_back(countryCode.get<std::string>());
        }

        m_regionRestrictionWhitelist = blacklist;
    }

    void YoutubeVideo::setProjection(nlohmann::json &t_projection) {
        // Determine is video is rectangular or 360 degrees.
        if (!t_projection.empty() and t_projection.is_string()) {
            std::string definition = t_projection.get<std::string>();

            if (definition == "rectangular") {
                setIsRectanguar(true);
                setIs360(false);
            } else if (definition == "360") {
                setIsRectanguar(false);
                setIs360(true);
            } else {
                // If somehow a third definition appears:
                addError("setProjection: Unexpected projection '" + definition + "'!", t_projection);
            }
        }
    }

    bool YoutubeVideo::is360() const {
        return m_is360;
    }

    void YoutubeVideo::setIs360(bool t_is360) {
        m_is360 = t_is360;
    }

    bool YoutubeVideo::isRectangular() const {
        return m_isRectangular;
    }

    void YoutubeVideo::setIsRectanguar(bool t_isRectanguar) {
        m_isRectangular = t_isRectanguar;
    }

    bool YoutubeVideo::hasCustomThumbnail() const {
        return m_hasCustomThumbnail;
    }

    void YoutubeVideo::setHasCustomThumbnail(bool t_hasCustomThumbnail) {
        m_hasCustomThumbnail = t_hasCustomThumbnail;
    }

    void YoutubeVideo::setHasCustomThumbnail(nlohmann::json &t_hasCustomThumbnail) {
        if (isBool(t_hasCustomThumbnail)) {
            setHasCustomThumbnail(getJsonBoolValue(t_hasCustomThumbnail));
        }
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
        std::string indentation2x = indentation + indentation;
        std::string indentation3x = indentation + indentation + indentation;

        std::cout << indentation << "ID: " << getId() << std::endl;
        std::cout << indentation << "PublishedAt: " << getPublishedAt() << std::endl;
        std::cout << indentation << "Channel ID: " << getChannelId() << std::endl;
        std::cout << indentation << "Channel Title: " << getChannelTitle() << std::endl;
        std::cout << indentation << "Description: " << getDescription() << std::endl;
        std::cout << indentation << "Thumbnails: " << std::endl;
        // For each category/key
        for (auto const& thumbnail : getThumbnails()) {
            // Category:
            std::cout << indentation2x << thumbnail.first << ":" << std::endl;
            //      Contents...
            std::cout << indentation3x << "Dimensions: " << thumbnail.second.width << "x" << thumbnail.second.height
                      << std::endl;
            std::cout << indentation3x << "URL:        " << thumbnail.second.url << std::endl;
        }
        std::cout << indentation << "Tags: " << std::endl;
        for (auto const& tag : getTags()) {
            std::cout << indentation2x << tag;
        }
        std::cout << indentation << "Category ID: " << getCategoryId() << std::endl;
        std::cout << indentation << "Live Broadcast Content: " << getLiveBroadcastContent() << std::endl;
        std::cout << indentation << "Default Language: " << getDefaultLanguage() << std::endl;
        std::cout << indentation << "Localized Title: " << getLocalizedTitle() << std::endl;
        std::cout << indentation << "Localized Description: " << getLocalizedDescription() << std::endl;
        std::cout << indentation << "Default Audio Language: " << getDefaultAudioLanguage() << std::endl;
        std::cout << indentation << "Duration: " << getDuration() << std::endl;
        std::cout << indentation << "Is 2-Dimensional: " << is2D() << std::endl;
        std::cout << indentation << "Is 3-Dimensional: " << is3D() << std::endl;
        std::cout << indentation << "Is HD: " << isHD() << std::endl;
        std::cout << indentation << "Is Rectangular: " << isRectangular() << std::endl;
        std::cout << indentation << "Is 360 Degrees: " << is360() << std::endl;
        std::cout << indentation << "Has Captions: " << hasCaptions() << std::endl;
        std::cout << indentation << "Is Licensed Content: " << isLicensedContent() << std::endl;
        std::cout << indentation << "Region Restriction: " << std::endl;
        std::cout << indentation2x << "Whitelist: " << std::endl;
        for (auto const& country : getRegionRestrictionWhitelist()) {
            std::cout << indentation3x << country << std::endl;
        }
        std::cout << indentation2x << "Blacklist: " << std::endl;
        for (auto const& country : getRegionRestrictionBlacklist()) {
            std::cout << indentation3x << country << std::endl;
        }
        std::cout << indentation << "Has Custom Thumbnail: " << hasCustomThumbnail() << std::endl;
        std::cout << indentation << "Upload Status: " << getUploadStatus() << std::endl;
        std::cout << indentation << "Failure Reason: " << getFailureReason() << std::endl;
        std::cout << indentation << "Rejection Reason: " << getRejectionReason() << std::endl;
        std::cout << indentation << "Privacy Status: " << getPrivacyStatus() << std::endl;
        std::cout << indentation << "Publish At: " << getPublishAt() << std::endl;
        std::cout << indentation << "License: " << getLicense() << std::endl;
        std::cout << indentation << "Is Embeddable: " << isEmbeddable() << std::endl;
        std::cout << indentation << "Public Stats Viewable: " << isPublicStatsViewable() << std::endl;
        std::cout << indentation << "Views: " << getViewCount() << std::endl;
        std::cout << indentation << "Likes: " << getLikeCount() << std::endl;
        std::cout << indentation << "Dislikes: " << getDislikeCount() << std::endl;
        std::cout << indentation << "Comments: " << getCommentCount() << std::endl;
        std::cout << indentation << "Player: " << std::endl;
        player_t player = getPlayer();
        std::cout << indentation2x << "Embed Dimensions: " << player.embedWidth << "x" << player.embedHeight
                  << std::endl;
        std::cout << indentation << "Embed HTML: " << player.embedHtml << std::endl;
        std::cout << indentation << "Topic Categories: " << std::endl;
        for (auto const& topicCat : getTopicCategories()) {
            std::cout << indentation2x << topicCat << std::endl;
        }
        std::cout << indentation << "Location: " << getLocation() << std::endl;
        std::cout << indentation << "Recording Date: " << getRecordingDate() << std::endl;
        std::cout << indentation << "Filename: " << getFileName() << std::endl;
        std::cout << indentation << "Filesize: " << getFileSize() << std::endl;
        std::cout << indentation << "Filetype: " << getFileType() << std::endl;
        std::cout << indentation << "Container: " << getContainer() << std::endl;
        std::cout << indentation << "Video Streams: " << std::endl;
        for (auto const& stream : getVideoStreams()) {
            std::cout << indentation2x << stream.codec << " " << stream.vendor << " " << stream.widthPixels <<  "x" << stream.heightPixels
                      << "p" << stream.frameRateFps << " (AR: " << stream.aspectRatio << ", Rotation; "
                      << stream.rotation << ") @ " << stream.bitrateBps << "Bps" << "." << std::endl;
        }
        std::cout << indentation << "Audio Streams: " << std::endl;
        for (auto const& stream : getAudioStreams()) {
            std::cout << indentation2x << stream.codec << " " << stream.vendor << " channels: " << stream.channelCount
                      << " @ " << stream.bitrateBps << "Bps" << "." << std::endl;
        }
        std::cout << indentation << "Duration (ms): " << getDurationMs() << std::endl;
        std::cout << indentation << "Bitrate (Bps): " << getBitrateBps() << std::endl;
        std::cout << indentation << "Creation Time: " << getCreationTime() << std::endl;
        std::cout << indentation << "Processing Status: " << getProcessingStatus() << std::endl;
        std::cout << indentation << "Processing Failure Reason: " << getProcessingFailureReason() << std::endl;
        std::cout << indentation << "Processing Issues Availability: " << getProcessingIssuesAvailability()
                  << std::endl;
        std::cout << indentation << "Tag Suggestions Availability: " << getTagSuggestionsAvailability() << std::endl;
        std::cout << indentation << "Editor Suggestions Availability: " << getEditorSuggestionsAvailability()
                  << std::endl;
        std::cout << indentation << "Thumbnails Availability: " << getThumbnailsAvailability() << std::endl;
        std::cout << indentation << "Processing Warnings: " << std::endl;
        for (auto const& warning : getProcessingWarnings()) {
            std::cout << indentation2x << warning << std::endl;
        }
        std::cout << indentation << "Processing Errors: " << std::endl;
        for (auto const& error : getProcessingErrors()) {
            std::cout << indentation2x << error << std::endl;
        }
        std::cout << indentation << "Processing Hints:: " << std::endl;
        for (auto const& hint : getProcessingHints()) {
            std::cout << indentation2x << hint << std::endl;
        }
        std::cout << indentation << "Tag Suggestions: " << std::endl;
        for (auto const& tagSuggestion : getTagSuggestions()) {
            std::cout << indentation2x << "Tag : " << tagSuggestion.tag << std::endl;
            std::cout << indentation2x << "Relevant video categories: " << std::endl;
            for (auto const& catRestrict : tagSuggestion.categoryRestricts) {
                std::cout << indentation3x << catRestrict << std::endl;
            }
        }
        std::cout << indentation << "Editor Suggestions: " << std::endl;
        for (auto const& suggestion : getEditorSuggestions()) {
            std::cout << indentation2x << suggestion << std::endl;
        }
        std::cout << indentation << "Live Streaming Details: " << std::endl;
        for (auto const& detail : getLiveStreamingDetails()) {
            std::cout << indentation2x << "Active LiveChat ID:   " << detail.activeLiveChatId << std::endl;
            std::cout << indentation2x << "Concurrent Viewers:   " << detail.concurrentViewers << std::endl;
            std::cout << indentation2x << "Scheduled Start Time: " << detail.scheduledStartTime << std::endl;
            std::cout << indentation2x << "Actual Start Time:    " << detail.actualStartTime << std::endl;
            std::cout << indentation2x << "Scheduled End Time:   " << detail.scheduledEndTime << std::endl;
            std::cout << indentation2x << "Actual End Time:      " << detail.actualEndTime << std::endl;
        }
        std::cout << indentation << "Localizations: " << std::endl;
        for (auto const& localization : getLocalizations()) {
            std::cout << indentation2x << "Title:       " << localization.title << std::endl;
            std::cout << indentation2x << "Description: " << localization.description << std::endl;
        }
    }

    void YoutubeVideo::addFromJson(nlohmann::json t_json) {
        try {
            // General.
            setId(t_json["id"]);

            // Add properties to various parts (if they exist).

            // Part: Snippet.
            if (t_json.find("snippet") != t_json.end()) {
                nlohmann::json snippet = t_json["snippet"];

                setTitle(snippet["title"]);
                setChannelId(snippet["channelId"]);
                setChannelTitle(snippet["channelTitle"]);
                setTags(snippet["tags"]);
                setDefaultLanguage(snippet["defaultLanguage"]);
                setDefaultAudioLanguage(snippet["defaultAudioLanguage"]);
                setDescription(snippet["description"]);
                setCategoryId(snippet["categoryId"]);
                setLiveBroadcastContent(snippet["liveBroadcastContent"]);
                setPublishedAt(snippet["publishedAt"]);

                if (snippet.find("localized") != snippet.end()) {
                    setLocalizedTitle(snippet["localized"]["title"]);
                    setLocalizedDescription(snippet["localized"]["description"]);
                }

                if (snippet.find("thumbnails") != snippet.end()) {
                    setThumbnails(snippet["thumbnails"]);
                }
            }

            // Part: Content details.
            if (t_json.find("contentDetails") != t_json.end()) {
                nlohmann::json contentDetails = t_json["contentDetails"];

                setDuration(contentDetails["duration"]);
                setDimension(contentDetails["dimension"]);
                setDefinition(contentDetails["definition"]);
                setHasCaptions(contentDetails["caption"]);
                setIsLicensedContent(contentDetails["licensedContent"]);
                setRegionRestrictionWhitelist(contentDetails["regionRestriction"]["allowed"]);
                setRegionRestrictionBlacklist(contentDetails["regionRestriction"]["blocked"]);
                // TODO: Content rating is SKIPPED for now.
                setProjection(contentDetails["projection"]);
                setHasCustomThumbnail(contentDetails["hasCustomThumbnail"]);
            }

            // Part: Status.
            if (t_json.find("status") != t_json.end()) {
                nlohmann::json status = t_json["status"];
            }

            // Part: Statistics
            if (t_json.find("statistics") != t_json.end()) {
                nlohmann::json statistics = t_json["statistics"];
            }

            // Part: Player
            if (t_json.find("player") != t_json.end()) {
                nlohmann::json player = t_json["player"];
            }

            // Part: Topic details
            if (t_json.find("topicDetails") != t_json.end()) {
                nlohmann::json topicDetails = t_json["topicDetails"];
            }

            // Part: Recording details.
            if (t_json.find("recordingDetails") != t_json.end()) {
                nlohmann::json recordingDetails = t_json["recordingDetails"];
            }

            // Part: File details.
            if (t_json.find("fileDetails") != t_json.end()) {
                nlohmann::json fileDetails = t_json["fileDetails"];
            }

            // Part: Processing details.
            if (t_json.find("processingDetails") != t_json.end()) {
                nlohmann::json processingDetails = t_json["processingDetails"];
            }

            // Part: Suggestions
            if (t_json.find("suggestions") != t_json.end()) {
                nlohmann::json suggestions = t_json["suggestions"];
            }

            // Part: Live streaming details.
            if (t_json.find("liveStreamingDetails") != t_json.end()) {
                nlohmann::json liveStreamingDetails = t_json["liveStreamingDetails"];
            }

            // Part: Localizations.
            if (t_json.find("localizations") != t_json.end()) {
                nlohmann::json localizations = t_json["localizations"];
            }
        } catch (nlohmann::detail::type_error &exc) {
            addError("Skipping YoutubeVideo::addFromJson due to Exception: " + std::string(exc.what()), t_json);
        } catch (const std::exception &exc) {
            addError("Skipping YoutubeVideo::addFromJson due to Unexpected Exception: " +
                     std::string(exc.what()), t_json);
        }
    }
} // namespace sane
