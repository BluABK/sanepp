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
#include "cli.hpp"

int main(int argc, char *argv[]) {
    sane::LogHandler logHandler;
    const std::string LOG_FACILITY = "core";
    std::shared_ptr<spdlog::logger> log = logHandler.createLogger(LOG_FACILITY, CLI_LOG_FILE);

    logHandler.logSeparator(CLI_LOG_FILE);

    log->set_level(spdlog::level::trace);

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