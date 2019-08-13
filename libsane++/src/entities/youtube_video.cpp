#include <iostream>

#include <entities/common.hpp>
#include <entities/youtube_video.hpp>

namespace sane {
    // An empty constructor if you want to populate it later.
    YoutubeVideo::YoutubeVideo() = default;

    void YoutubeVideo::addError(const std::string &t_errorMsg, nlohmann::json t_json) {
        std::map<std::string, nlohmann::json> _;
        _[t_errorMsg] = t_json;

        m_errors.push_back(_);
    }

    void YoutubeVideo::addWarning(const std::string &t_warningMsg, nlohmann::json t_json) {
        std::map<std::string, nlohmann::json> _;
        _[t_warningMsg] = t_json;

        m_warnings.push_back(_);
    }

    void YoutubeVideo::reportProblems(std::map<std::string, std::string> &t_problems) {
        if (!t_problems.empty()) {
            addWarning(t_problems["warning"]);
            addError(t_problems["error"]);
        }
    }

    std::list<std::map<std::string, nlohmann::json>> YoutubeVideo::getErrors() {
        return m_errors;
    }

    void YoutubeVideo::printErrors(int indent, bool withJson, int jsonIndent) {
        // For map in list
        for (auto const& item : getErrors()) {
            // For message, JSON in map
            for (auto const& map : item) {
                std::cout << std::string(indent, ' ') << map.first << std::endl;
                if (withJson and !map.second.empty()) {
                    std::cout << map.second.dump(jsonIndent) << std::endl;
                }
            }
        }
    }

    std::list<std::map<std::string, nlohmann::json>> YoutubeVideo::getWarnings() {
        return m_warnings;
    }

    void YoutubeVideo::printWarnings(int indent, bool withJson, int jsonIndent) {
        // For map in list
        for (auto const& item : getWarnings()) {
            // For message, JSON in map
            for (auto const& map : item) {
                std::cout << std::string(indent, ' ') << map.first << std::endl;
                if (withJson and !map.second.empty()) {
                    std::cout << map.second.dump(jsonIndent) << std::endl;
                }
            }
        }
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
    const std::string &YoutubeVideo::getId() const {
        return m_id;
    }

    void YoutubeVideo::setId(const std::string &t_id) {
        m_id = t_id;
    }

    void YoutubeVideo::setId(nlohmann::json t_id) {
        if (t_id.is_string()) {
            std::map<std::string, std::string> problems;
            
            setId(getJsonStringValue(t_id, "setId", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getPublishedAt() const {
        return m_publishedAt;
    }

    void YoutubeVideo::setPublishedAt(const std::string &t_publishedAt) {
        m_publishedAt = t_publishedAt;
    }

    void YoutubeVideo::setPublishedAt(nlohmann::json &t_publishedAt) {
        if (t_publishedAt.is_string()) {
            std::map<std::string, std::string> problems;

            setPublishedAt(getJsonStringValue(t_publishedAt, "setPublishedAt", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getChannelId() const {
        return m_channelId;
    }

    void YoutubeVideo::setChannelId(const std::string &t_channelId) {
        m_channelId = t_channelId;
    }

    void YoutubeVideo::setChannelId(nlohmann::json &t_channelId) {
        if (t_channelId.is_string()) {
            std::map<std::string, std::string> problems;

            setChannelId(getJsonStringValue(t_channelId, "setChannelId", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getTitle() const {
        return m_title;
    }

    void YoutubeVideo::setTitle(const std::string &t_title) {
        m_title = t_title;
    }

    void YoutubeVideo::setTitle(nlohmann::json &t_title) {
        if (t_title.is_string()) {
            std::map<std::string, std::string> problems;

            setTitle(getJsonStringValue(t_title, "setTitle", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getDescription() const {
        return m_description;
    }

    void YoutubeVideo::setDescription(const std::string &t_description) {
        m_description = t_description;
    }

    void YoutubeVideo::setDescription(nlohmann::json &t_description) {
        if (t_description.is_string()) {
            std::map<std::string, std::string> problems;

            setDescription(getJsonStringValue(t_description, "setDescription", problems));
            reportProblems(problems);
        }
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
        if (t_thumbnails["default"]["url"].is_string()) {
            std::map<std::string, std::string> problems;

            defaultThumbnail.url = getJsonStringValue(t_thumbnails["default"]["url"],
                                                                     "setThumbnails [default]", problems);
            reportProblems(problems);
        }
        defaultThumbnail.height  = t_thumbnails["default"]["height"].get<unsigned int>();
        defaultThumbnail.width   = t_thumbnails["default"]["width"].get<unsigned int>();

        thumbnail_t highThumbnail;
        if (t_thumbnails["high"]["url"].is_string()) {
            std::map<std::string, std::string> problems;

            highThumbnail.url = getJsonStringValue(t_thumbnails["high"]["url"],
                                                                    "setThumbnails [high]", problems);
            reportProblems(problems);
        }
        highThumbnail.height     = t_thumbnails["high"]["height"].get<unsigned int>();
        highThumbnail.width      = t_thumbnails["high"]["width"].get<unsigned int>();

        thumbnail_t mediumThumbnail;
        if (t_thumbnails["medium"]["url"].is_string()) {
            std::map<std::string, std::string> problems;

            mediumThumbnail.url = getJsonStringValue(t_thumbnails["medium"]["url"],
                                                                 "setThumbnails [medium]", problems);
            reportProblems(problems);
        }
        mediumThumbnail.height   = t_thumbnails["medium"]["height"].get<unsigned int>();
        mediumThumbnail.width    = t_thumbnails["medium"]["width"].get<unsigned int>();

        thumbnail_t standardThumbnail;
        if (t_thumbnails["standard"]["url"].is_string()) {
            std::map<std::string, std::string> problems;

            standardThumbnail.url = getJsonStringValue(t_thumbnails["standard"]["url"],
                                                                 "setThumbnails [standard]", problems);
            reportProblems(problems);
        }
        standardThumbnail.height = t_thumbnails["standard"]["height"].get<unsigned int>();
        standardThumbnail.width  = t_thumbnails["standard"]["width"].get<unsigned int>();

        thumbnail_t maxresThumbnail;
        if (t_thumbnails["maxres"]["url"].is_string()) {
            std::map<std::string, std::string> problems;

            maxresThumbnail.url = getJsonStringValue(t_thumbnails["maxres"]["url"],
                                                                 "setThumbnails [maxres]", problems);
            reportProblems(problems);
        }
        maxresThumbnail.height  = t_thumbnails["maxres"]["height"].get<unsigned int>();
        maxresThumbnail.width   = t_thumbnails["maxres"]["width"].get<unsigned int>();

        // Add thumbnail structs to map.
        thumbnails["default"] = defaultThumbnail;
        thumbnails["high"] = highThumbnail;
        thumbnails["medium"] = mediumThumbnail;
        thumbnails["standard"] = standardThumbnail;
        thumbnails["maxres"] = maxresThumbnail;

        // Finally assign the thumbnails map to the object property.
        setThumbnails(thumbnails);
    }

    const std::string &YoutubeVideo::getChannelTitle() const {
        return m_channelTitle;
    }

    void YoutubeVideo::setChannelTitle(const std::string &t_channelTitle) {
        m_channelTitle = t_channelTitle;
    }

    void YoutubeVideo::setChannelTitle(nlohmann::json &t_channelTitle) {
        if (t_channelTitle.is_string()) {
            std::map<std::string, std::string> problems;
            
            setChannelTitle(getJsonStringValue(t_channelTitle, "setChannelTitle", problems));
            reportProblems(problems);
        }
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

        for (const nlohmann::json &tag : t_tags) {
            if (tag.is_string()) {
                tags.push_back(tag.get<std::string>());
            } else {
                addError("setTags: non-string (" + std::string(tag.type_name()) + "): " + tag.dump());
            }
        }

        setTags(tags);
    }

    const std::string &YoutubeVideo::getCategoryId() const {
        return m_categoryId;
    }

    void YoutubeVideo::setCategoryId(const std::string &t_categoryId) {
        m_categoryId = t_categoryId;
    }

    void YoutubeVideo::setCategoryId(nlohmann::json &t_categoryId) {
        if (t_categoryId.is_string()) {
            std::map<std::string, std::string> problems;
            
            setCategoryId(getJsonStringValue(t_categoryId, "setCategoryId", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getLiveBroadcastContent() const {
        return m_liveBroadcastContent;
    }

    void YoutubeVideo::setLiveBroadcastContent(const std::string &t_liveBroadcastContent) {
        m_liveBroadcastContent = t_liveBroadcastContent;
    }

    void YoutubeVideo::setLiveBroadcastContent(nlohmann::json &t_liveBroadcastContent) {
        if (t_liveBroadcastContent.is_string()) {
            std::map<std::string, std::string> problems;
            
            setLiveBroadcastContent(getJsonStringValue(t_liveBroadcastContent, "setLiveBroadcastContent",
                                                                     problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getDefaultLanguage() const {
        return m_defaultLanguage;
    }

    void YoutubeVideo::setDefaultLanguage(const std::string &t_defaultLanguage) {
        m_defaultLanguage = t_defaultLanguage;
    }

    void YoutubeVideo::setDefaultLanguage(nlohmann::json &t_defaultLanguage) {
        if (t_defaultLanguage.is_string()) {
            std::map<std::string, std::string> problems;
            
            setDefaultLanguage(getJsonStringValue(t_defaultLanguage, "setDefaultLanguage", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getLocalizedTitle() const {
        return m_localizedTitle;
    }

    void YoutubeVideo::setLocalizedTitle(const std::string &t_localizedTitle) {
        m_localizedTitle = t_localizedTitle;
    }

    void YoutubeVideo::setLocalizedTitle(nlohmann::json &t_localizedTitle) {
        if (t_localizedTitle.is_string()) {
            std::map<std::string, std::string> problems;
            
            setLocalizedTitle(getJsonStringValue(t_localizedTitle, "setLocalizedTitle", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getLocalizedDescription() const {
        return m_localizedDescription;
    }

    void YoutubeVideo::setLocalizedDescription(const std::string &t_localizedDescription) {
        m_localizedDescription = t_localizedDescription;
    }

    void YoutubeVideo::setLocalizedDescription(nlohmann::json &t_localizedDescription) {
        if (t_localizedDescription.is_string()) {
            std::map<std::string, std::string> problems;
            
            setLocalizedDescription(getJsonStringValue(t_localizedDescription, "setLocalizedDescription",
                                                                     problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getDefaultAudioLanguage() const {
        return m_defaultAudioLanguage;
    }

    void YoutubeVideo::setDefaultAudioLanguage(const std::string &t_defaultAudioLanguage) {
        m_defaultAudioLanguage = t_defaultAudioLanguage;
    }

    void YoutubeVideo::setDefaultAudioLanguage(nlohmann::json &t_defaultAudioLanguage) {
        if (t_defaultAudioLanguage.is_string()) {
            std::map<std::string, std::string> problems;
            
            setDefaultAudioLanguage(getJsonStringValue(t_defaultAudioLanguage, "setDefaultAudioLanguage",
                                                                     problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getDuration() const {
        return m_duration;
    }

    void YoutubeVideo::setDuration(const std::string &t_duration) {
        m_duration = t_duration;
    }

    void YoutubeVideo::setDuration(nlohmann::json &t_duration) {
        if (t_duration.is_string()) {
            std::map<std::string, std::string> problems;
            
            setDuration(getJsonStringValue(t_duration, "setDuration", problems));
            reportProblems(problems);
        }
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
            std::map<std::string, std::string> problems;
            
            setHasCaptions(getJsonBoolValue(t_hasCaptions, "setHasCaptions", problems));
            reportProblems(problems);
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
            std::map<std::string, std::string> problems;
            
            setIsLicensedContent(getJsonBoolValue(t_isLicensedContent, "setIsLicensedContent", problems));
            reportProblems(problems);
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

        setRegionRestrictionWhitelist(whitelist);
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

        setRegionRestrictionBlacklist(blacklist);
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
            std::map<std::string, std::string> problems;
            
            setHasCustomThumbnail(getJsonBoolValue(t_hasCustomThumbnail, "setHasCustomThumbnail", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getUploadStatus() const {
        return m_uploadStatus;
    }

    void YoutubeVideo::setUploadStatus(const std::string &t_uploadStatus) {
        m_uploadStatus = t_uploadStatus;
    }

    void YoutubeVideo::setUploadStatus(nlohmann::json &t_uploadStatus) {
        if (t_uploadStatus.is_string()) {
            std::map<std::string, std::string> problems;
            
            setUploadStatus(getJsonStringValue(t_uploadStatus, "setUploadStatus", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getFailureReason() const {
        return m_failureReason;
    }

    void YoutubeVideo::setFailureReason(const std::string &t_failureReason) {
        m_failureReason = t_failureReason;
    }

    void YoutubeVideo::setFailureReason(nlohmann::json &t_failureReason) {
        if (t_failureReason.is_string()) {
            std::map<std::string, std::string> problems;
            
            setFailureReason(getJsonStringValue(t_failureReason, "setFailureReason", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getRejectionReason() const {
        return m_rejectionReason;
    }

    void YoutubeVideo::setRejectionReason(const std::string &t_rejectionReason) {
        m_rejectionReason = t_rejectionReason;
    }

    void YoutubeVideo::setRejectionReason(nlohmann::json &t_rejectionReason) {
        if (t_rejectionReason.is_string()) {
            std::map<std::string, std::string> problems;
            
            setRejectionReason(getJsonStringValue(t_rejectionReason, "setRejectionReason", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getPrivacyStatus() const {
        return m_privacyStatus;
    }

    void YoutubeVideo::setPrivacyStatus(const std::string &t_privacyStatus) {
        m_privacyStatus = t_privacyStatus;
    }

    void YoutubeVideo::setPrivacyStatus(nlohmann::json &t_privacyStatus) {
        if (t_privacyStatus.is_string()) {
            std::map<std::string, std::string> problems;
            
            setPrivacyStatus(getJsonStringValue(t_privacyStatus, "setPrivacyStatus", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getPublishAt() const {
        return m_publishAt;
    }

    void YoutubeVideo::setPublishAt(const std::string &t_publishAt) {
        m_publishAt = t_publishAt;
    }

    void YoutubeVideo::setPublishAt(nlohmann::json &t_publishAt) {
        if (t_publishAt.is_string()) {
            std::map<std::string, std::string> problems;
            
            setPublishAt(getJsonStringValue(t_publishAt, "setPublishAt", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getLicense() const {
        return m_license;
    }

    void YoutubeVideo::setLicense(const std::string &t_license) {
        m_license = t_license;
    }

    void YoutubeVideo::setLicense(nlohmann::json &t_license) {
        if (t_license.is_string()) {
            std::map<std::string, std::string> problems;
            
            setLicense(getJsonStringValue(t_license, "setLicense", problems));
            reportProblems(problems);
        }
    }

    bool YoutubeVideo::isEmbeddable() const {
        return m_isEmbeddable;
    }

    void YoutubeVideo::setIsEmbeddable(bool t_isEmbeddable) {
        m_isEmbeddable = t_isEmbeddable;
    }

    void YoutubeVideo::setIsEmbeddable(nlohmann::json &t_isEmbeddable) {
        if (isBool(t_isEmbeddable)) {
            std::map<std::string, std::string> problems;
            setIsEmbeddable(getJsonBoolValue(t_isEmbeddable, "setIsEmbeddable", problems));
            reportProblems(problems);
        }
    }

    bool YoutubeVideo::isPublicStatsViewable() const {
        return m_publicStatsViewable;
    }

    void YoutubeVideo::setPublicStatsViewable(bool t_isPublicStatsViewable) {
        m_publicStatsViewable = t_isPublicStatsViewable;
    }

    void YoutubeVideo::setPublicStatsViewable(nlohmann::json &t_isPublicStatsViewable) {
        if (isBool(t_isPublicStatsViewable)) {
            std::map<std::string, std::string> problems;
            setPublicStatsViewable(getJsonBoolValue(t_isPublicStatsViewable, "setPublicStatsViewable",
                                                                  problems));
            reportProblems(problems);
        }
    }

    unsigned long YoutubeVideo::getViewCount() const {
        return m_viewCount;
    }

    void YoutubeVideo::setViewCount(unsigned long t_viewCount) {
        m_viewCount = t_viewCount;
    }

    void YoutubeVideo::setViewCount(nlohmann::json &t_viewCount) {
        if (isDigits(t_viewCount)) {
            std::map<std::string, std::string> problems;
            
            setViewCount(getJsonULongValue(t_viewCount, "setViewCount", problems));
            reportProblems(problems);
        }
    }

    unsigned long YoutubeVideo::getLikeCount() const {
        return m_likeCount;
    }

    void YoutubeVideo::setLikeCount(unsigned long t_likeCount) {
        m_likeCount = t_likeCount;
    }

    void YoutubeVideo::setLikeCount(nlohmann::json &t_likeCount) {
        if (isDigits(t_likeCount)) {
            std::map<std::string, std::string> problems;
            
            setLikeCount(getJsonULongValue(t_likeCount, "setLikeCount", problems));
            reportProblems(problems);
        }
    }

    unsigned long YoutubeVideo::getDislikeCount() const {
        return m_dislikeCount;
    }

    void YoutubeVideo::setDislikeCount(unsigned long t_dislikeCount) {
        m_dislikeCount = t_dislikeCount;
    }

    void YoutubeVideo::setDislikeCount(nlohmann::json &t_dislikeCount) {
        if (isDigits(t_dislikeCount)) {
            std::map<std::string, std::string> problems;
            
            setDislikeCount(getJsonULongValue(t_dislikeCount, "setDislikeCount", problems));
            reportProblems(problems);
        }
    }

    unsigned long YoutubeVideo::getCommentCount() const {
        return m_commentCount;
    }

    void YoutubeVideo::setCommentCount(unsigned long t_commentCount) {
        m_commentCount = t_commentCount;
    }

    void YoutubeVideo::setCommentCount(nlohmann::json &t_commentCount) {
        if (isDigits(t_commentCount)) {
            std::map<std::string, std::string> problems;
            
            setCommentCount(getJsonULongValue(t_commentCount, "setCommentCount", problems));
            reportProblems(problems);
        }
    }

    const player_t &YoutubeVideo::getPlayer() const {
        return m_player;
    }

    void YoutubeVideo::setPlayer(const player_t &t_player) {
        m_player = t_player;
    }

    void YoutubeVideo::setPlayer(nlohmann::json &t_player) {
        player_t player = player_t();

        if (t_player["embedHtml"].is_string()) {
            std::map<std::string, std::string> problems;
            
            player.embedHtml = getJsonStringValue(t_player["embedHtml"], "setPlayer [embedHtml]", problems);
            reportProblems(problems);
        }

        if (t_player.find("embedWidth") != t_player.end()) {
            nlohmann::json embedWidth = t_player["embedWidth"];
            if (isDigits(embedWidth)) {
                std::map<std::string, std::string> problems;

                player.embedWidth = getJsonLongValue(embedWidth, "setPlayer [embedWidth]", problems);
                reportProblems(problems);
            } else {
                addError("Player: Attempted to set non-long embedWidth! Type: " +
                         std::string(embedWidth.type_name()) + ", Value: " + embedWidth.dump(), embedWidth);
            }
        }

        if (t_player.find("embedHeight") != t_player.end()) {
            nlohmann::json embedHeight = t_player["embedHeight"];
            if (isDigits(embedHeight)) {
                std::map<std::string, std::string> problems;

                player.embedHeight = getJsonLongValue(embedHeight, "setPlayer [embedHeight]", problems);
                reportProblems(problems);
            } else {
                addError("Player: Attempted to set non-long embedHeight! Type: " +
                         std::string(embedHeight.type_name()) + ", Value: " + embedHeight.dump(), embedHeight);
            }
        }

        setPlayer(player);
    }

    const std::list<std::string> &YoutubeVideo::getTopicCategories() const {
        return m_topicCategories;
    }

    void YoutubeVideo::setTopicCategories(const std::list<std::string> &t_topicCategories) {
        m_topicCategories = t_topicCategories;
    }

    void YoutubeVideo::setTopicCategories(nlohmann::json &t_topicCategories) {
        std::list<std::string> categories;

        for (auto const& category : t_topicCategories) {
            categories.push_back(category.get<std::string>());
        }

        setTopicCategories(categories);
    }

    const std::string &YoutubeVideo::getRecordingDate() const {
        return m_recordingDate;
    }

    void YoutubeVideo::setRecordingDate(const std::string &t_recordingDate) {
        m_recordingDate = t_recordingDate;
    }

    void YoutubeVideo::setRecordingDate(nlohmann::json &t_recordingDate) {
        if (t_recordingDate.is_string()) {
            std::map<std::string, std::string> problems;

            setRecordingDate(getJsonStringValue(t_recordingDate, "setRecordingDate", problems));
            reportProblems(problems);
        }

    }

    const std::string &YoutubeVideo::getFileName() const {
        return m_fileName;
    }

    void YoutubeVideo::setFileName(const std::string &t_fileName) {
        m_fileName = t_fileName;
    }

    void YoutubeVideo::setFileName(nlohmann::json &t_fileName) {
        if (t_fileName.is_string()) {
            std::map<std::string, std::string> problems;

            setFileName(getJsonStringValue(t_fileName, "setFileName", problems));
            reportProblems(problems);
        }
    }

    unsigned long YoutubeVideo::getFileSize() const {
        return m_fileSize;
    }

    void YoutubeVideo::setFileSize(unsigned long t_fileSize) {
        m_fileSize = t_fileSize;
    }

    void YoutubeVideo::setFileSize(nlohmann::json &t_fileSize) {
        if (isDigits(t_fileSize)) {
            std::map<std::string, std::string> problems;

            setFileSize(getJsonULongValue(t_fileSize, "setFileSize", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getFileType() const {
        return m_fileType;
    }

    void YoutubeVideo::setFileType(const std::string &t_fileType) {
        m_fileType = t_fileType;
    }

    void YoutubeVideo::setFileType(nlohmann::json &t_fileType) {
        if (t_fileType.is_string()) {
            std::map<std::string, std::string> problems;

            setFileType(getJsonStringValue(t_fileType, "setFileType", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getContainer() const {
        return m_container;
    }

    void YoutubeVideo::setContainer(const std::string &t_container) {
        m_container = t_container;
    }

    void YoutubeVideo::setContainer(nlohmann::json &t_container) {
        if (t_container.is_string()) {
            std::map<std::string, std::string> problems;

            setContainer(getJsonStringValue(t_container, "setContainer", problems));
            reportProblems(problems);
        }
    }

    const std::list<videoStream_t> &YoutubeVideo::getVideoStreams() const {
        return m_videoStreams;
    }

    void YoutubeVideo::setVideoStreams(const std::list<videoStream_t> &t_videoStreams) {
        m_videoStreams = t_videoStreams;
    }

    void YoutubeVideo::setVideoStreams(nlohmann::json &t_videoStreams) {
        std::list<videoStream_t> streams;

        if (t_videoStreams.empty()) {
            return;
        }

        for (nlohmann::json item : t_videoStreams) {
            videoStream_t stream = videoStream_t();

            if (isDigits(item["widthPixels"])) {
                stream.widthPixels = item["widthPixels"].get<unsigned int>();
            }

            if (isDigits(item["heightPixels"])) {
                stream.heightPixels = item["heightPixels"].get<unsigned int>();
            }

            stream.frameRateFps = item["frameRateFps"].get<double>();
            stream.aspectRatio = item["aspectRatio"].get<double>();

            if (item["codec"].is_string()) {
                std::map<std::string, std::string> problems;

                stream.codec = getJsonStringValue(item["codec"], "setVideoStreams [codec]", problems);
                reportProblems(problems);
            }

            if (isDigits(item["bitrateBps"])) {
                std::map<std::string, std::string> problems;

                stream.bitrateBps = getJsonULongValue(item["bitrateBps"], "setVideoStreams [bitrateBps]",
                                                                    problems);
                reportProblems(problems);
            }

            if (item["rotation"].is_string()) {
                std::map<std::string, std::string> problems;

                stream.rotation = getJsonStringValue(item["rotation"], "setVideoStreams [rotation]",
                                                                   problems);
                reportProblems(problems);
            }

            if (item["vendor"].is_string()) {
                std::map<std::string, std::string> problems;

                stream.vendor = getJsonStringValue(item["vendor"], "setVideoStreams [vendor]", problems);
                reportProblems(problems);
            }

            streams.push_back(stream);
        }

        setVideoStreams(streams);
    }

    const std::list<audioStream_t> &YoutubeVideo::getAudioStreams() const {
        return m_audioStreams;
    }

    void YoutubeVideo::setAudioStreams(const std::list<audioStream_t> &t_audioStreams) {
        m_audioStreams = t_audioStreams;
    }

    void YoutubeVideo::setAudioStreams(nlohmann::json &t_audioStreams) {
        std::list<audioStream_t> streams;

        if (t_audioStreams.empty()) {
            return;
        }

        for (nlohmann::json item : t_audioStreams) {
            audioStream_t stream = audioStream_t();

            if (isDigits(item["channelCount"])) {
                stream.channelCount = item["channelCount"].get<unsigned int>();
            }

            if (item["codec"].is_string()) {
                std::map<std::string, std::string> problems;

                stream.codec = getJsonStringValue(item["codec"], "setAudioStreams [codec]", problems);
                reportProblems(problems);
            }

            if (isDigits(item["bitrateBps"])) {
                std::map<std::string, std::string> problems;

                stream.bitrateBps = getJsonULongValue(item["bitrateBps"], "setAudioStreams [bitrateBps]",
                                                                    problems);
                reportProblems(problems);
            }

            if (item["vendor"].is_string()) {
                std::map<std::string, std::string> problems;

                stream.vendor = getJsonStringValue(item["vendor"], "setAudioStreams [vendor]", problems);
                reportProblems(problems);
            }

            streams.push_back(stream);
        }

        setAudioStreams(streams);
    }

    unsigned long YoutubeVideo::getDurationMs() const {
        return m_durationMs;
    }

    void YoutubeVideo::setDurationMs(unsigned long t_durationMs) {
        m_durationMs = t_durationMs;
    }

    void YoutubeVideo::setDurationMs(nlohmann::json &t_durationMs) {
        if (isDigits(t_durationMs)) {
            std::map<std::string, std::string> problems;

            setDurationMs(getJsonULongValue(t_durationMs, "", problems));
            reportProblems(problems);
        }
    }

    unsigned long YoutubeVideo::getBitrateBps() const {
        return m_bitrateBps;
    }

    void YoutubeVideo::setBitrateBps(unsigned long t_bitrateBps) {
        m_bitrateBps = t_bitrateBps;
    }

    void YoutubeVideo::setBitrateBps(nlohmann::json &t_bitrateBps) {
        if (isDigits(t_bitrateBps)) {
            std::map<std::string, std::string> problems;

            setBitrateBps(getJsonULongValue(t_bitrateBps, "setBitrateBps", problems));
            reportProblems(problems);
        }
    }

    const std::string &YoutubeVideo::getCreationTime() const {
        return m_creationTime;
    }

    void YoutubeVideo::setCreationTime(const std::string &t_creationTime) {
        m_creationTime = t_creationTime;
    }

    void YoutubeVideo::setCreationTime(nlohmann::json & t_creationTime) {
        if (t_creationTime.is_string()) {
            std::map<std::string, std::string> problems;

            setCreationTime(getJsonStringValue(t_creationTime, "setCreationTime", problems));
            reportProblems(problems);
        }
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

    void YoutubeVideo::print(int t_indentationSpacing = 0, bool t_printFullInfo) { // FIXME: IMPLEMENT
        std::string indentation(t_indentationSpacing, ' ');
        std::string indentation2x(2 * t_indentationSpacing, ' ');
        std::string indentation3x(3 * t_indentationSpacing, ' ');

        // Print boolean values instead of ints.
        std::cout << std::boolalpha;

        if (!getId().empty() or t_printFullInfo) {
            std::cout << indentation << "ID: " << getId() << std::endl;
        }
        if (!getPublishedAt().empty() or t_printFullInfo) {
            std::cout << indentation << "PublishedAt: " << getPublishedAt() << std::endl;
        }
        if (!getChannelId().empty() or t_printFullInfo) {
            std::cout << indentation << "Channel ID: " << getChannelId() << std::endl;
        }
        if (!getChannelTitle().empty() or t_printFullInfo) {
            std::cout << indentation << "Channel Title: " << getChannelTitle() << std::endl;
        }
        if (!getDescription().empty() or t_printFullInfo) {
            std::cout << indentation << "Description: " << getDescription() << std::endl;
        }
        if (!getThumbnails().empty() or t_printFullInfo) {
            std::cout << indentation << "Thumbnails: " << std::endl;
            // For each category/key
            for (auto const& thumbnail : getThumbnails()) {
                if (!thumbnail.first.empty() or t_printFullInfo) {
                    std::string dimensions = std::to_string(thumbnail.second.width) + "x"
                                           + std::to_string(thumbnail.second.height);
                    size_t longestKeyLine = std::string("standard").length();
                    size_t longestDimensionLine = 10;

                    std::cout << indentation2x << thumbnail.first << ":"
                              << std::string(longestKeyLine - thumbnail.first.length() + 1, ' ')
                              << dimensions << std::string(longestDimensionLine - dimensions.length(), ' ')
                              << thumbnail.second.url
                              << std::endl;
                }
            }
        }
        if (!getTags().empty() or t_printFullInfo) {
            std::cout << indentation << "Tags: " << std::endl;
            size_t counter = 0;
            for (auto const& tag : getTags()) {
                std::cout << indentation2x << tag;

                if (counter < getTags().size()-1) {
                    std::cout << ",";
                }

                std::cout << std::endl;

                counter++;
            }
        }
        if (!getCategoryId().empty() or t_printFullInfo) {
            std::cout << indentation << "Category ID: " << getCategoryId() << std::endl;
        }
        if (!getLiveBroadcastContent().empty() or t_printFullInfo) {
            std::cout << indentation << "Live Broadcast Content: " << getLiveBroadcastContent() << std::endl;
        }
        if (!getDefaultLanguage().empty() or t_printFullInfo) {
            std::cout << indentation << "Default Language: " << getDefaultLanguage() << std::endl;
        }
        if (!getLocalizedTitle().empty() or t_printFullInfo) {
            std::cout << indentation << "Localized Title: " << getLocalizedTitle() << std::endl;
        }
        if (!getLocalizedDescription().empty() or t_printFullInfo) {
            std::cout << indentation << "Localized Description: " << getLocalizedDescription() << std::endl;
        }
        if (!getDefaultAudioLanguage().empty() or t_printFullInfo) {
            std::cout << indentation << "Default Audio Language: " << getDefaultAudioLanguage() << std::endl;
        }
        if (!getDuration().empty() or t_printFullInfo) {
            std::cout << indentation << "Duration: " << getDuration() << std::endl;
        }
        if (t_printFullInfo) {
            std::cout << indentation << "2-Dimensional: " << is2D() << std::endl;
            std::cout << indentation << "3-Dimensional: " << is3D() << std::endl;
        } else if (hasPartContentDetails) {
            std::cout << indentation << "Dimension: " << (is2D() ? "2D" : "3D") << std::endl;
        }
        if (hasPartContentDetails or t_printFullInfo) {
            std::cout << indentation << "HD: " << isHD() << std::endl;
        }
        if (t_printFullInfo) {
            std::cout << indentation << "Rectangular: " << isRectangular() << std::endl;
            std::cout << indentation << "360 Degrees: " << is360() << std::endl;
        } else if (hasPartContentDetails) {
            std::cout << indentation << "Projection: " << (is360() ? "360 Degrees" : "Rectangular") << std::endl;
        }
        if (hasPartContentDetails or t_printFullInfo) {
            std::cout << indentation << "Has Captions: " << hasCaptions() << std::endl;
        }
        if (hasPartContentDetails or t_printFullInfo) {
            std::cout << indentation << "Licensed Content: " << isLicensedContent() << std::endl;
        }
        if (!getRegionRestrictionWhitelist().empty() or !getRegionRestrictionBlacklist().empty() or t_printFullInfo) {
            std::cout << indentation << "Region Restriction: " << std::endl;

            if (!getRegionRestrictionWhitelist().empty() or t_printFullInfo) {
                std::cout << indentation2x << "Whitelist: " << std::endl;
                for (auto const& country : getRegionRestrictionWhitelist()) {
                    std::cout << indentation3x << country << std::endl;
                }
            }

            if (!getRegionRestrictionBlacklist().empty() or t_printFullInfo) {
                std::cout << indentation2x << "Blacklist: " << std::endl;
                for (auto const &country : getRegionRestrictionBlacklist()) {
                    std::cout << indentation3x << country << std::endl;
                }
            }
        }
        if (hasPartContentDetails or t_printFullInfo) {
            std::cout << indentation << "Has Custom Thumbnail: " << hasCustomThumbnail() << std::endl;
        }
        if (!getUploadStatus().empty() or t_printFullInfo) {
            std::cout << indentation << "Upload Status: " << getUploadStatus() << std::endl;
        }
        if (!getFailureReason().empty() or t_printFullInfo) {
            std::cout << indentation << "Failure Reason: " << getFailureReason() << std::endl;
        }
        if (!getRejectionReason().empty() or t_printFullInfo) {
            std::cout << indentation << "Rejection Reason: " << getRejectionReason() << std::endl;
        }
        if (!getPrivacyStatus().empty() or t_printFullInfo) {
            std::cout << indentation << "Privacy Status: " << getPrivacyStatus() << std::endl;
        }
        if (!getPublishAt().empty() or t_printFullInfo) {
            std::cout << indentation << "Publish At: " << getPublishAt() << std::endl;
        }
        if (!getLicense().empty() or t_printFullInfo) {
            std::cout << indentation << "License: " << getLicense() << std::endl;
        }
        if (hasPartStatus or t_printFullInfo) {
            std::cout << indentation << "Embeddable: " << isEmbeddable() << std::endl;
        }
        if (hasPartStatus or t_printFullInfo) {
            std::cout << indentation << "Public Stats Viewable: " << isPublicStatsViewable() << std::endl;
        }
        if (hasPartStatistics or t_printFullInfo) {
            std::cout << indentation << "Views: " << getViewCount() << std::endl;
        }
        if (hasPartStatistics or t_printFullInfo) {
            std::cout << indentation << "Likes: " << getLikeCount() << std::endl;
        }
        if (hasPartStatistics or t_printFullInfo) {
            std::cout << indentation << "Dislikes: " << getDislikeCount() << std::endl;
        }
        if (hasPartStatistics or t_printFullInfo) {
            std::cout << indentation << "Comments: " << getCommentCount() << std::endl;
        }
        if (hasPartPlayer or t_printFullInfo) {
            std::cout << indentation << "Player: " << std::endl;
            player_t player = getPlayer();
            if (player.embedWidth != 0 and player.embedHeight != 0) {
                std::cout << indentation2x << "Embed Dimensions: " << player.embedWidth << "x" << player.embedHeight
                          << std::endl;
            }
            std::cout << indentation2x << "Embed HTML: " << player.embedHtml << std::endl;
        }
        if (!getTopicCategories().empty() or t_printFullInfo) {
            std::cout << indentation << "Topic Categories: " << std::endl;

            for (auto const& topicCat : getTopicCategories()) {
                std::cout << indentation2x << topicCat << std::endl;
            }
        }
        if (!getRecordingDate().empty() or t_printFullInfo) {
            std::cout << indentation << "Recording Date: " << getRecordingDate() << std::endl;
        }
        if (!getFileName().empty() or t_printFullInfo) {
            std::cout << indentation << "Filename: " << getFileName() << std::endl;
        }
        if ( (hasPartFileDetails or t_printFullInfo) and getFileSize() != 0) {
            std::cout << indentation << "Filesize: " << getFileSize() << std::endl;
        }
        if (!getFileType().empty() or t_printFullInfo) {
            std::cout << indentation << "Filetype: " << getFileType() << std::endl;
        }
        if (!getContainer().empty() or t_printFullInfo) {
            std::cout << indentation << "Container: " << getContainer() << std::endl;
        }
        if (!getVideoStreams().empty() or t_printFullInfo) {
            std::cout << indentation << "Video Streams: " << std::endl;

            for (auto const& stream : getVideoStreams()) {
                std::cout << indentation2x << stream.codec << " " << stream.vendor << " " << stream.widthPixels <<  "x" << stream.heightPixels
                          << "p" << stream.frameRateFps << " (AR: " << stream.aspectRatio << ", Rotation; "
                          << stream.rotation << ") @ " << stream.bitrateBps << "Bps" << "." << std::endl;
            }
        }
        if (!getAudioStreams().empty() or t_printFullInfo) {
            std::cout << indentation << "Audio Streams: " << std::endl;

            for (auto const& stream : getAudioStreams()) {
                std::cout << indentation2x << stream.codec << " " << stream.vendor << " channels: " << stream.channelCount
                          << " @ " << stream.bitrateBps << "Bps" << "." << std::endl;
            }
        }
        if ( (hasPartFileDetails or t_printFullInfo) and getDurationMs() != 0 ) {
            std::cout << indentation << "Duration (ms): " << getDurationMs() << std::endl;
        }
        if ( (hasPartFileDetails or t_printFullInfo) and getBitrateBps() != 0 ) {
            std::cout << indentation << "Bitrate (Bps): " << getBitrateBps() << std::endl;
        }
        if (!getCreationTime().empty() or t_printFullInfo) {
            std::cout << indentation << "Creation Time: " << getCreationTime() << std::endl;
        }
        if (!getProcessingStatus().empty() or t_printFullInfo) {
            std::cout << indentation << "Processing Status: " << getProcessingStatus() << std::endl;
        }
        if (!getProcessingFailureReason().empty() or t_printFullInfo) {
            std::cout << indentation << "Processing Failure Reason: " << getProcessingFailureReason() << std::endl;
        }
        if (!getProcessingIssuesAvailability().empty() or t_printFullInfo) {
            std::cout << indentation << "Processing Issues Availability: " << getProcessingIssuesAvailability()
                  << std::endl;
        }
        if (!getTagSuggestionsAvailability().empty() or t_printFullInfo) {
            std::cout << indentation << "Tag Suggestions Availability: " << getTagSuggestionsAvailability() << std::endl;
        }
        if (!getEditorSuggestionsAvailability().empty() or t_printFullInfo) {
            std::cout << indentation << "Editor Suggestions Availability: " << getEditorSuggestionsAvailability()
                  << std::endl;
        }
        if (!getThumbnailsAvailability().empty() or t_printFullInfo) {
            std::cout << indentation << "Thumbnails Availability: " << getThumbnailsAvailability() << std::endl;
        }
        if (!getProcessingWarnings().empty() or t_printFullInfo) {
            std::cout << indentation << "Processing Warnings: " << std::endl;

            for (auto const& warning : getProcessingWarnings()) {
                std::cout << indentation2x << warning << std::endl;
            }
        }
        if (!getProcessingErrors().empty() or t_printFullInfo) {
            std::cout << indentation << "Processing Errors: " << std::endl;

            for (auto const& error : getProcessingErrors()) {
                std::cout << indentation2x << error << std::endl;
            }
        }
        if (!getProcessingHints().empty() or t_printFullInfo) {
            std::cout << indentation << "Processing Hints:: " << std::endl;

            for (auto const& hint : getProcessingHints()) {
                std::cout << indentation2x << hint << std::endl;
            }
        }
        if (!getTagSuggestions().empty() or t_printFullInfo) {
            std::cout << indentation << "Tag Suggestions: " << std::endl;

            for (auto const& tagSuggestion : getTagSuggestions()) {
                std::cout << indentation2x << "Tag : " << tagSuggestion.tag << std::endl;
                std::cout << indentation2x << "Relevant video categories: " << std::endl;
                for (auto const& catRestrict : tagSuggestion.categoryRestricts) {
                    std::cout << indentation3x << catRestrict << std::endl;
                }
            }
        }
        if (!getEditorSuggestions().empty() or t_printFullInfo) {
            std::cout << indentation << "Editor Suggestions: " << std::endl;

            for (auto const& suggestion : getEditorSuggestions()) {
                std::cout << indentation2x << suggestion << std::endl;
            }
        }
        if (!getLiveStreamingDetails().empty() or t_printFullInfo) {
            std::cout << indentation << "Live Streaming Details: " << std::endl;

            for (auto const& detail : getLiveStreamingDetails()) {
                std::cout << indentation2x << "Active LiveChat ID:   " << detail.activeLiveChatId << std::endl;
                std::cout << indentation2x << "Concurrent Viewers:   " << detail.concurrentViewers << std::endl;
                std::cout << indentation2x << "Scheduled Start Time: " << detail.scheduledStartTime << std::endl;
                std::cout << indentation2x << "Actual Start Time:    " << detail.actualStartTime << std::endl;
                std::cout << indentation2x << "Scheduled End Time:   " << detail.scheduledEndTime << std::endl;
                std::cout << indentation2x << "Actual End Time:      " << detail.actualEndTime << std::endl;
            }
        }
        if (!getLocalizations().empty() or t_printFullInfo) {
            std::cout << indentation << "Localizations: " << std::endl;

            for (auto const& localization : getLocalizations()) {
                std::cout << indentation2x << "Title:       " << localization.title << std::endl;
                std::cout << indentation2x << "Description: " << localization.description << std::endl;
            }
        }
        if (t_printFullInfo) {
            std::vector<std::string> parts;

            if (hasPartSnippet) parts.emplace_back("snippet");
            if (hasPartContentDetails) parts.emplace_back("contentDetails");
            if (hasPartStatus) parts.emplace_back("status");
            if (hasPartStatistics) parts.emplace_back("statistics");
            if (hasPartPlayer) parts.emplace_back("player");
            if (hasPartTopicDetails) parts.emplace_back("topicDetails");
            if (hasPartRecordingDetails) parts.emplace_back("recordingDetails");
            if (hasPartFileDetails) parts.emplace_back("fileDetails");
            if (hasPartProcessingDetails) parts.emplace_back("processingDetails");
            if (hasPartSuggestions) parts.emplace_back("suggestions");
            if (hasPartLiveStreamingDetails) parts.emplace_back("liveStreamingDetails");
            if (hasPartLocalizations) parts.emplace_back("localizations");

            std::cout << indentation << "Parts: ";
            for (size_t i = 0; i < parts.size(); ++i) {
                std::cout << parts.at(i);

                if (i < parts.size() - 1) {
                    // Add a comma delimiter after each item except the last one.
                    std::cout << ", ";
                } else {
                    // Print a sentence terminator.
                    std::cout << ".";
                }
            }
            std::cout << std::endl;
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
                hasPartSnippet = true;

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
                hasPartContentDetails = true;

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
                hasPartStatus = true;

                setUploadStatus(status["uploadStatus"]);
                if (status.find("failureReason") != status.end()) {
                    setFailureReason(status["failureReason"]);
                }
                if (status.find("rejectionReason") != status.end()) {
                    setRejectionReason(status["rejectionReason"]);
                }
                setPrivacyStatus(status["privacyStatus"]);
                setPublishAt(status["publishAt"]);
                setLicense(status["license"]);
                setIsEmbeddable(status["embeddable"]);
                setPublicStatsViewable(status["publicStatsViewable"]);
            }

            // Part: Statistics
            if (t_json.find("statistics") != t_json.end()) {
                nlohmann::json statistics = t_json["statistics"];
                hasPartStatistics = true;

                setViewCount(statistics["viewCount"]);
                setLikeCount(statistics["likeCount"]);
                setDislikeCount(statistics["dislikeCount"]);
                setCommentCount(statistics["commentCount"]);
            }

            // Part: Player
            if (t_json.find("player") != t_json.end()) {
                nlohmann::json player = t_json["player"];
                hasPartPlayer = true;

                setPlayer(player);
            }

            // Part: Topic details
            if (t_json.find("topicDetails") != t_json.end()) {
                nlohmann::json topicDetails = t_json["topicDetails"];
                hasPartTopicDetails = true;

                setTopicCategories(topicDetails["topicCategories"]);
            }

            // Part: Recording details.
            if (t_json.find("recordingDetails") != t_json.end()) {
                nlohmann::json recordingDetails = t_json["recordingDetails"];
                hasPartRecordingDetails = true;

                setRecordingDate(recordingDetails["recordingDate"]);
            }

            // Part: File details.
            if (t_json.find("fileDetails") != t_json.end()) {
                nlohmann::json fileDetails = t_json["fileDetails"];
                hasPartFileDetails = true;

                setFileName(fileDetails["fileName"]);
                setFileSize(fileDetails["fileSize"]);
                setFileType(fileDetails["fileType"]);
                setContainer(fileDetails["container"]);
                setVideoStreams(fileDetails["videoStreams"]);
                setAudioStreams(fileDetails["audioStreams"]);
                setDurationMs(fileDetails["durationMs"]);
                setBitrateBps(fileDetails["bitrateBps"]);
                setCreationTime(fileDetails["creationTime"]);
            }

            // Part: Processing details.
            if (t_json.find("processingDetails") != t_json.end()) {
                nlohmann::json processingDetails = t_json["processingDetails"];
                hasPartProcessingDetails = true;
            }

            // Part: Suggestions
            if (t_json.find("suggestions") != t_json.end()) {
                nlohmann::json suggestions = t_json["suggestions"];
                hasPartSuggestions = true;
            }

            // Part: Live streaming details.
            if (t_json.find("liveStreamingDetails") != t_json.end()) {
                nlohmann::json liveStreamingDetails = t_json["liveStreamingDetails"];
                hasPartLiveStreamingDetails = true;
            }

            // Part: Localizations.
            if (t_json.find("localizations") != t_json.end()) {
                nlohmann::json localizations = t_json["localizations"];
                hasPartLocalizations = true;
            }
        } catch (nlohmann::detail::type_error &exc) {
            addError("Skipping YoutubeVideo::addFromJson due to Exception: " + std::string(exc.what()), t_json);
        } catch (const std::exception &exc) {
            addError("Skipping YoutubeVideo::addFromJson due to Unexpected Exception: " +
                     std::string(exc.what()), t_json);
        }
    }
} // namespace sane
