#ifndef SANE_LOG_HANDLER_HPP
#define SANE_LOG_HANDLER_HPP

#include <string>
#include <map>
#include <iostream>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace sane {
    class LogHandler {
    public:
        LogHandler();

        std::shared_ptr<spdlog::logger> createLogger(const std::string &t_facility, const std::string &t_logFile);

        void logSeparator(const std::string &t_logFile);

        void resetPattern(std::shared_ptr<spdlog::logger> &t_logger);

        void resetPattern(std::shared_ptr<spdlog::sinks::basic_file_sink_mt> &t_sink);

        void setLevel(spdlog::level::level_enum t_logLevel);

        spdlog::level::level_enum getLevel();

        static std::map<std::string, std::shared_ptr<spdlog::sinks::basic_file_sink_mt>> sinks;
    private:
        std::string defaultLogPattern = "%+";
        spdlog::level::level_enum logLevel = spdlog::level::trace;
    };
} // namespace sane



#endif //SANE_LOG_HANDLER_HPP
