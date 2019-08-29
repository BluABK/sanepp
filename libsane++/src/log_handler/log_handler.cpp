#include <log_handler/log_handler.hpp>

// 3rd Party
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace sane {
    LogHandler::LogHandler() = default;

    std::shared_ptr<spdlog::logger> LogHandler::createLogger(const std::string &t_facility,
            const std::string &t_logFile) {
        std::shared_ptr<spdlog::logger> logger;

        try {
            logger = spdlog::basic_logger_mt(t_facility, t_logFile);
        } catch (const spdlog::spdlog_ex &floggerExc) {
            std::cerr << "File logger init failed, defaulting to stderr: " << floggerExc.what() << std::endl;
            try {
                logger = spdlog::stderr_color_mt(t_facility);
            } catch (const spdlog::spdlog_ex &exc) {
                std::cerr << "stderr logger init failed, aborting due to fatal error " << exc.what() << std::endl;
            }
        }

        return logger;
    }

    void LogHandler::logSeparator(const std::string &t_filename) {
        // Create temporary logger instance.
        std::shared_ptr<spdlog::logger> _ = createLogger("LOG_SEPARATOR", t_filename);

        // Set the pattern to be a static dotted line.
        _->set_pattern("\n" + std::string(80, '-'));

        // Print the static pattern.
        _->info("");

        // Flush the output to file
        _->flush();

        // Remove the temporary logger instance from the registry.
        spdlog::drop("LOG_SEPARATOR");
    }
} // namespace sane