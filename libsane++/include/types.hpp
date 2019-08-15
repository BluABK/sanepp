#ifndef SANE_TYPES_HPP
#define SANE_TYPES_HPP

#include <string>
#include <ctime>

#include <time.h>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <iostream>
#include <vector>

#include <lexical_analysis.hpp>

namespace sane {
    extern "C" char* strptime(const char* s, const char* f, struct tm* tm);

    struct datetime_t {
        bool isEmpty = true;

        // YYYY-MM-DDThh:mm:ss.sssZ
        std::string iso8601;

        // YYYY-MM-DD HH:MM:SS
        std::string isoDateAndTime;

        // UNIX Timestamp
        long int timestamp;

        // Hybrid UNIX Timestamp with milliseconds for extra precision.
        long double timestampWithMsec;

        // As individual Segments (Using std::tm struct as template)
        int year;               // Years since 1900
        int month;              // Months since January     [0,  11]
        int day;                // Day of the month         [1,  31]
        int weekDay;            // Days since Sunday        [0,   6]
        int yearDay;            // Days since January 1st   [0, 365]

        int hour;               // Hours since midnight     [0,  23]
        int minute;             // Minutes after the hour   [0,  59]
        int second;             // Seconds after the minute [0,  60]
        int millisecond;        // Milliseconds [000-999]

        // Timezones! D:
        // Pesky, pesky DST ~
        int isDST;              // Daylight Saving Time flag.
                                // The value is positive if DST is in effect,
                                // zero if not and negative if no information is available.
        long int gmtOffset;	    // Seconds east of UTC.
        std::string timezone;   // Timezone abbreviation.

        // Format strings
        const char* ISO8601_FORMAT = "%Y-%m-%dT%H:%M:%S."; // millisecond not in tm struct spec

        void fromISO8601(const std::string &t_iso8601) {
            std::string iso8601WithoutMilliseconds;
            iso8601 = t_iso8601;

            // Only check for milliseconds if there's a '.' in the ISO 8601 string.
            if (t_iso8601.find('.') != std::string::npos) {
                iso8601WithoutMilliseconds = tokenize(t_iso8601, '.').at(0);
            } else {
                iso8601WithoutMilliseconds = t_iso8601;
            }

            // Create an std::tm struct and strptime the ISO8601 string.
            tm timeInfo = tm();
            strptime(t_iso8601.c_str(), ISO8601_FORMAT, &timeInfo);

            // Calculate UNIX timestamp (returned value is time_t, which needs to be recast).
            timestamp = static_cast<long int>(timegm(&timeInfo));

            // Set humanized date & time.
            isoDateAndTime = iso8601WithoutMilliseconds;
            std::replace(isoDateAndTime.begin(), isoDateAndTime.end(), 'T', ' ');

            // Assign individual values
            year    = timeInfo.tm_year;
            month   = timeInfo.tm_mon;
            day     = timeInfo.tm_mday;
            weekDay = timeInfo.tm_wday;
            yearDay = timeInfo.tm_yday;
            hour    = timeInfo.tm_hour;
            minute  = timeInfo.tm_min;
            second  = timeInfo.tm_sec;
            try {
                // Only check for milliseconds if there's a '.' in the ISO 8601 string.
                if (t_iso8601.find('.') != std::string::npos) {
                    std::vector<std::string> tokensWithUTCDesignator = tokenize(t_iso8601, '.');
                    std::string hopefullyNumericString = tokenize(tokensWithUTCDesignator.at(1), 'Z').at(0);
                    // Only continue if the string isn't empty.
                    if (!hopefullyNumericString.empty()) {
                        millisecond = std::stoi(hopefullyNumericString);
                    }
                }
            } catch(std::invalid_argument& e){
                // if no conversion could be performed
                std::cerr << "datetime_t.fromISO8601(" << t_iso8601 << "): " << "Invalid argument exception!"
                          << std::endl;
            } catch(std::out_of_range& e){
                // if the converted value would fall out of the range of the result type
                // or if the underlying function (std::strtol or std::strtoull) sets errno
                // to ERANGE.
                std::cerr << "datetime_t.fromISO8601(" << t_iso8601 << "): " <<"Out of range exception" << std::endl;
            } catch (const std::exception &exc) {
                std::cerr << "datetime_t.fromISO8601(" << t_iso8601 << "): " << "Unexpected exception: "
                          << std::string(exc.what()) << "!" << std::endl;
            }

            timestampWithMsec = timestamp + (millisecond / 1000.0);

            isDST = timeInfo.tm_isdst;
            gmtOffset = timeInfo.tm_gmtoff;
            timezone  = timeInfo.tm_zone;

            isEmpty = false;
        }

        bool empty() {
            return isEmpty;
        }
    };
} // namespace sane
#endif //SANE_TYPES_HPP

