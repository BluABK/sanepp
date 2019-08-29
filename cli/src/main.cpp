/*
 *  Sane CLI.
 */

// Standard Libraries
#include <iostream>
#include <string>

// 3rd Party
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// Sane++ Project specific
#include <config.hpp>
#include <api_handler/api_handler.hpp>
#include <db_handler/db_handler.hpp>
#include "cli.hpp"

#define LOG_START_INDICATOR 1
#define LOG_END_INDICATOR   2

bool createLogger(std::shared_ptr<spdlog::logger> &t_loggerObject, const std::string &t_facility,
                  const std::string &t_logFile) {
    try {
        t_loggerObject = spdlog::basic_logger_mt(t_facility, t_logFile);
        return true;
    } catch (const spdlog::spdlog_ex &floggerExc) {
        std::cerr << "File logger init failed, defaulting to stderr: " << floggerExc.what() << std::endl;
        try {
            t_loggerObject = spdlog::stderr_color_mt(t_facility);
        } catch (const spdlog::spdlog_ex &exc) {
            std::cerr << "stderr logger init failed, aborting due to fatal error " << exc.what() << std::endl;
        }
    }

    return false;
}

void logSeparator(int t_state) {
    try {
        std::shared_ptr<spdlog::logger> _;
        const std::string line = std::string(30, '-');

        if (t_state == LOG_START_INDICATOR) {
            createLogger(_, "LOG_START_INDICATOR", CLI_LOG_FILE);
            _->set_pattern("\n" + line + " Sane++ CLI Log START %Y-%m-%d %H:%M:%S %z " + line);
            _->info("");
            _->flush();
        } else if (t_state == LOG_END_INDICATOR) {
            createLogger(_, "LOG_END_INDICATOR", CLI_LOG_FILE);
            _->set_pattern(line + " Sane++ CLI Log END   %Y-%m-%d %H:%M:%S %z " + line);
            _->info("");
        }

    } catch (const spdlog::spdlog_ex &ex) {
        std::cerr << "Log init failed: " << ex.what() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    const std::string LOG_FACILITY = "core";
    std::shared_ptr<spdlog::logger> logger;

    logSeparator(LOG_START_INDICATOR);

    createLogger(logger, LOG_FACILITY, CLI_LOG_FILE);

    logger->set_level(spdlog::level::trace);

    logger->info("Sane++ CLI Main init!");

    // Instantiate the CLI class.
    std::shared_ptr<sane::CLI> cli = std::make_shared<sane::CLI>();
    logger->info("Instantiated CLI.");

    // If arguments were passed, execute them. If not, then run interactively.
    if (argc > 1 and argv != nullptr) {
        // Handle passed arguments.
        logger->info("Arguments were passed, executing them instead of running interactively.");
        std::vector<std::string> commandWithArgs;
        commandWithArgs.assign(argv + 1, argv + argc);

        // Log which arguments to debug log level.
        std::string argsAsString;
        for (const auto& arg: commandWithArgs) {
            argsAsString += arg;
        }
        logger->debug("Arguments: " + argsAsString);

        cli->executeCommand(commandWithArgs);
    } else {
        // Make the CLI interactive
        logger->info("Running CLI in interactive mode.");
        cli->interactive();
    }

    logSeparator(LOG_END_INDICATOR);

    return 0;
}