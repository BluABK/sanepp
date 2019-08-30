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

        explicit LogHandler(const std::string &t_defaultLogFile);

        std::shared_ptr<spdlog::logger> createLogger(const std::string &t_facility,
                const std::string &t_logFile = m_defaultLogfile);

        void logSeparator(const std::string &t_logFile = m_defaultLogfile);

        void resetPattern(std::shared_ptr<spdlog::logger> &t_logger);

        void resetPattern(std::shared_ptr<spdlog::sinks::basic_file_sink_mt> &t_sink);

        void setLevel(spdlog::level::level_enum t_logLevel);

        spdlog::level::level_enum getLevel();

        static std::map<std::string, std::shared_ptr<spdlog::sinks::basic_file_sink_mt>> sinks;
    private:
        static std::string m_defaultLogfile;
        std::string m_defaultLogPattern = "%+";
        spdlog::level::level_enum m_logLevel = spdlog::level::trace;
    };
} // namespace sane



#endif //SANE_LOG_HANDLER_HPP
