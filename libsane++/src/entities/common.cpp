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

    bool isDigits(nlohmann::json &t_json) {
        if (!t_json.empty()) {
            if (t_json.is_number()) {
                return true;
            } else if (t_json.is_string()) {
                // If string, check if string consists solely of digits.
                std::string str = t_json.get<std::string>();
                return std::all_of(str.begin(), str.end(), ::isdigit);
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

    long getJsonLongValue(nlohmann::json &t_long) {
        if (t_long.is_string()) {
            return std::stol(t_long.get<std::string>());
        } else if (t_long.is_number()) {
            return t_long.get<long>();
        } else {
            std::cerr << "getJsonLongValue called with invalid parameter! Type: " << t_long.type_name()
                      << ", Value: " + t_long.dump() << std::endl;
            return 0;
        }
    }

    unsigned long getJsonULongValue(nlohmann::json &t_ulong) {
        if (t_ulong.is_string()) {
            return std::stoul(t_ulong.get<std::string>());
        } else if (t_ulong.is_number()) {
            return t_ulong.get<unsigned long>();
        } else {
            std::cerr << "getJsonULongValue called with invalid parameter! Type: " << t_ulong.type_name()
                      << ", Value: " + t_ulong.dump() << std::endl;
            return 0;
        }
    }
} // namespace sane