/*
 *  Sane CLI.
 */

// Standard Libraries
#include <iostream>
#include <string>

// Sane++ Project specific
#include <config.hpp>
#include <api_handler/api_handler.hpp>
#include <db_handler/db_handler.hpp>
#include <log_handler/log_handler.hpp>
#include <config_handler/config_handler.hpp>
#include "cli.hpp"

// Logging
#define LOG_FILE     "logs/sane++.log"

int main(int argc, char *argv[]) {
    // Instantiate LogHandler before ConfigHandler due to default log file.
    std::shared_ptr<sane::LogHandler> logHandler = std::make_shared<sane::LogHandler>(LOG_FILE);

    // Instantiate ConfigHandler before SPDLog due to log config settings.
    std::shared_ptr<sane::ConfigHandler> cfg = std::make_shared<sane::ConfigHandler>();

    std::shared_ptr<spdlog::logger> log = logHandler->createLogger("main");

    // Logger settings:
    // - Level.
    if (cfg->hasSection("logging/level")) {
        int logLevel = cfg->getInt("logging/level");
        if (logLevel >= (int)spdlog::level::trace and logLevel <= (int)spdlog::level::off) {
            logHandler->setLevel(static_cast<spdlog::level::level_enum>(logLevel));
            log->set_level(static_cast<spdlog::level::level_enum>(logLevel));
        } else {
            log->error("Invalid log level in config: " + std::to_string(logLevel));
            logHandler->setLevel(spdlog::level::trace);
            log->set_level(spdlog::level::trace);
        }
    }

    // Now that log level has been determined, the separator can be printed.
    logHandler->logSeparator();

    // - Severity based flush level override.
    if (cfg->hasSection("logging/flush_level")) {
        if (cfg->isNumber("logging/flush_level")) {
            int logFlushLevel = cfg->getInt("logging/flush_level");

            if (logFlushLevel >= (int)spdlog::level::trace and logFlushLevel <= (int)spdlog::level::critical) {
                // This will trigger flush whenever messages of given severity are logged.
                log->flush_on(static_cast<spdlog::level::level_enum>(logFlushLevel));
                log->info("Set severity based flush level: " + std::to_string(logFlushLevel));
            } else {
                log->error("Config has section \"logging/flush_level\", but numeric value is invalid: "
                           + std::to_string(spdlog::level::trace) + std::string(" <= ")
                           + std::to_string(logFlushLevel) + " <= " + std::to_string(spdlog::level::critical));
            }
        } else {
            log->error("Config has section \"logging/flush_level\", but value is NaN: "
                     + cfg->getString("logging/flush_level"));
        }
    }

    log->info("Sane++ CLI Main init!");

    // Instantiate the CLI class.
    std::shared_ptr<sane::CLI> cli = std::make_shared<sane::CLI>();
    log->info("Instantiated CLI.");

    // If arguments were passed, execute them. If not, then run interactively.
    if (argc > 1 and argv != nullptr) {
        // Handle passed arguments.
        log->info("Arguments were passed, executing them instead of running interactively.");
        std::vector<std::string> commandWithArgs;
        commandWithArgs.assign(argv + 1, argv + argc);

        // Log which arguments to debug log level.
        std::string argsAsString;
        for (const auto& arg: commandWithArgs) {
            argsAsString += arg;
        }
        log->debug("Arguments: " + argsAsString);

        cli->executeCommand(commandWithArgs);
    } else {
        // Make the CLI interactive
        log->info("Running CLI in interactive mode.");
        cli->interactive();
    }

    log->info("Exit.");
    return 0;
}