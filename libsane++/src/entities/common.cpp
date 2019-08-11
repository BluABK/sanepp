#include <iostream>

#include <entities/common.hpp>

namespace sane {
/**
 * Attempts to assign a JSON value to a given string and handle any bogus data.
 * If the input JSON value isn't a string it sets MISSING_VALUE.
 *
 * This function passes stringToAssignValue by reference (directly modifying it).
 *
 * @param stringToAssignValue   String to assign a value (modified directly).
 * @param unknownJsonTypeValue  The undetermined JSON value to assign the string.
 * @param fullJson              OPTIONAL: The complete JSON object, for use in debugging/error reporting.
 *
 * @return integer code.
 */
    int assignJsonStringValue(std::string &stringToAssignValue, nlohmann::json &unknownJsonTypeValue) {
        if (unknownJsonTypeValue.is_null()) {
            stringToAssignValue = MISSING_VALUE;

            return JSON_VALUE_NULL;
        } else if (!unknownJsonTypeValue.is_string()) {
            stringToAssignValue = MISSING_VALUE;

            return JSON_VALUE_NOT_STRING;
        } else {
            // If it actually is a string, then explicitly cast it.
            stringToAssignValue = unknownJsonTypeValue.get<std::string>();

            return JSON_VALUE_OK;
        }
    }

    bool isBool(nlohmann::json &t_bool) {
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

    bool getJsonBoolValue(nlohmann::json &t_bool) {
        if (t_bool.is_boolean()) {
            return t_bool.get<bool>();
        } else if (t_bool.is_string()) {
            return t_bool.get<std::string>() == "true";
        } else {
            std::cerr << "Called getJsonBoolValue with invalid bool, forced to return false!" << t_bool << std::endl;
            return false;
        }

    }
} // namespace sane