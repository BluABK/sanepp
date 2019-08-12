#include <iostream>

#include <entities/common.hpp>

namespace sane {
    /**
     * Extended check for if JSON value is boolean.
     *
     * @param t_bool
     * @return
     */
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

    /**
     * Extended check for if JSON value is a number/digits.
     *
     * @param t_json
     * @return
     */
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

    /**
     * Returns a boolean value from the given JSON value.
     *
     * If unsuccessful return default value and warn/error.
     *
     * @param t_bool    JSON value to be retrieved.
     * @param t_funcName  Name of the function that called me.
     * @param t_problems  Map to populate with possible warnings and errors..
     * @return
     */
    bool getJsonBoolValue(nlohmann::json &t_bool, const std::string &t_funcName,
                          std::map<std::string, std::string> &t_problems) {
        if (t_bool.is_boolean()) {
            // JSON is of expected type.
            return t_bool.get<bool>();
        } else if (t_bool.is_string()) {
            // JSON is a string, but it might still contain a boolean value.
            return t_bool.get<std::string>() == "true";
        } else {
            // JSON is of unhandled type.
            t_problems["error"] = t_funcName + ": getJsonBoolValue called with invalid bool, "
                                               "forced to return false! Type: "
                                + t_bool.type_name() + std::string(", JSON: ") + t_bool.dump();
        }

        // If you got here an error has occurred, returning false since something *has* to be returned.
        return false;
    }

    /**
     * Returns a string value from the given JSON value.
     *
     * If unsuccessful return default value and warn/error.
     *
     * @param t_string  JSON value to be retrieved.
     * @param t_funcName  Name of the function that called me.
     * @param t_problems  Map to populate with possible warnings and errors..
     * @return
     */
    std::string getJsonStringValue(nlohmann::json &t_string, const std::string &t_funcName,
                                   std::map<std::string, std::string> &t_problems) {
        if (t_string.is_string()) {
            if (t_string.get<std::string>() == "null") {
                // Called with the "null" string.
                t_problems["warning"] = t_funcName + ": getJsonStringValue called with \"null\" string, "
                                                     "returning empty string!";
            } else {
                // If it actually is a string, then explicitly cast it.
                return t_string.get<std::string>();
            }
        } else if (t_string.is_null()) {
            // Called with no value.
            t_problems["error"] = t_funcName + ": getJsonStringValue called with no value, returning empty string!";
        } else if (!t_string.is_string()) {
            // JSON is string with no digits.
            t_problems["error"] = t_funcName + ": getJsonStringValue called with non-string! Type: "
                    + t_string.type_name() + std::string(", JSON: ") + t_string.dump();
        } else {
            // JSON is of unhandled type.
            t_problems["error"] = t_funcName + ": getJsonStringValue called with invalid parameter! Type: "
                    + t_string.type_name() + std::string(", JSON: ") + t_string.dump();
        }

        // If you got here an error has occurred, returning empty string since something *has* to be returned.
        return {};
    }

    /**
     * Returns a long value from the given JSON value.
     *
     * If unsuccessful return default value and warn/error.
     *
     * @param t_long    JSON value to be retrieved.
     * @param t_funcName  Name of the function that called me.
     * @param t_problems  Map to populate with possible warnings and errors..
     * @return
     */
    long getJsonLongValue(nlohmann::json &t_long, const std::string &t_funcName,
                          std::map<std::string, std::string> &t_problems) {
        if (t_long.is_number()) {
            // JSON has expected value type.
            return t_long.get<long>();
        } else if (t_long.is_null()) {
            // Called with no value.
            t_problems["error"] = t_funcName + ": getJsonLongValue called with no value, returning 0 (likely wrong)!";
        } else if (t_long.is_string()) {
            // JSON is of string type, but might still hold digits.
            if ( isDigits(t_long) ) {
                // JSON is digits as a string.
                return std::stol(t_long.get<std::string>());
            } else if (t_long.get<std::string>() == "null") {
                // Called with the "null" string.
                t_problems["error"] = t_funcName + ": getJsonULongValue called with \"null\" string, "
                                                   "returning 0 (likely wrong)!";
            } else {
                    // JSON is string with no digits.
                t_problems["error"] = t_funcName + ": getJsonLongValue called with non-digit string, "
                                                   "returning 0 (likely wrong)! Type: " + t_long.type_name()
                                                   + std::string(", JSON: ") + t_long.dump();
                }
        } else {
            // Unhandled file type
            t_problems["error"] = t_funcName + ": getJsonLongValue called  with invalid parameter, "
                                               "returning 0 (likely wrong)! Type: " + t_long.type_name()
                                               + std::string(", JSON: ") + t_long.dump();
        }

        // If you got here an error has occurred, returning 0 since something *has* to be returned.
        return 0;
    }

    /**
     * Returns an unsigned long value from the given JSON value.
     *
     * If unsuccessful return default value and warn/error.
     *
     * @param t_ulong     JSON value to be retrieved.
     * @param t_funcName  Name of the function that called me.
     * @param t_problems  Map to populate with possible warnings and errors..
     * @return
     */
    unsigned long getJsonULongValue(nlohmann::json &t_ulong, const std::string &t_funcName,
                                    std::map<std::string, std::string> &t_problems) {
        if (t_ulong.is_number()) {
            // JSON has expected value type.
            return t_ulong.get<unsigned long>();
        } else if (t_ulong.is_null()) {
            // Called with no value.
            t_problems["error"] = t_funcName + ": getJsonULongValue called with no value, "
                                               "returning 0 (likely wrong)! Type: ";
        } else if (t_ulong.is_string()) {
            // JSON is of string type, but might still hold digits.
            if (isDigits(t_ulong)) {
                // JSON is digits as a string.
                return std::stoul(t_ulong.get<std::string>());
            } else if (t_ulong.get<std::string>() == "null") {
                // Called with the "null" string.
                t_problems["error"] = t_funcName + ": getJsonULongValue called with \"null\" string, "
                                                   "returning 0 (likely wrong)!";
            } else {
                // JSON is string with no digits.
                t_problems["error"] = t_funcName + ": getJsonULongValue called with non-digit string, "
                                                   "returning 0 (likely wrong)! Type: " + t_ulong.type_name()
                                                   + std::string(", JSON: ") + t_ulong.dump();
            }
        } else {
            // Unhandled file type
            t_problems["error"] = t_funcName + ": getJsonULongValue called "
                                  "with invalid parameter, returning 0 (likely wrong)! Type: " + t_ulong.type_name()
                                  + std::string(", JSON: ") + t_ulong.dump();
        }

        // If you got here an error has occurred, returning 0 since something *has* to be returned.
        return 0;
        }
} // namespace sane