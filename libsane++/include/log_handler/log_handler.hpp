#ifndef SANE_LOG_HANDLER_HPP
#define SANE_LOG_HANDLER_HPP

#include <string>
#include <map>
#include <iostream>

#include <spdlog/spdlog.h>

namespace sane {
    class LogHandler {
    public:
        LogHandler();

        std::shared_ptr<spdlog::logger> createLogger(const std::string &t_facility, const std::string &t_logFile);

        void logSeparator(const std::string &t_filename);
    private:
    };
} // namespace sane



#endif //SANE_LOG_HANDLER_HPP
