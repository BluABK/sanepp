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

    void YoutubeVideo::setId(nlohmann::json t_id, nlohmann::json t_json) {
        assignJsonStringValue(m_id, t_id, t_json);
    }

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
