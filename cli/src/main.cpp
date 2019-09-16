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

    // Send in LOG_FILE in order to set the further default value printSeparator True.
    std::shared_ptr<spdlog::logger> log = logHandler->createLogger("main", LOG_FILE, true);

    log->info("Sane++ CLI Main init!");

    // Instantiate the CLI class.
    std::shared_ptr<sane::CLI> cli = std::make_shared<sane::CLI>();
    log->info("Instantiated CLI.");

    // If arguments were passed, execute them. If not, then run interactively.
    if (argc > 1 and argv != nullptr) {
        // Handle passed arguments.
        log->info("Arguments were passed, executing them instead of running CLI interactively.");
        std::vector<std::string> commandWithArgs;
        commandWithArgs.assign(argv + 1, argv + argc);

        // Log which arguments to debug log level.
        std::string argsAsString;
        for (const auto& arg: commandWithArgs) {
            argsAsString += arg;
        }
        log->debug("Arguments: " + argsAsString);

        log->info("Running CLI.");
        cli->executeCommand(commandWithArgs);
    } else {
        // Make the CLI interactive
        log->info("Running CLI.");
        cli->interactive();
    }

    log->info("Exit.");
    return 0;
}