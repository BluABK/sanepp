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
#include "cli.hpp"

int main(int argc, char *argv[]) {
    // Instantiate the CLI class.
    std::shared_ptr<sane::CLI> cli = std::make_shared<sane::CLI>();

    // If arguments were passed, execute them. If not, then run interactively.
    if (argc > 1 and argv != nullptr) {
        // Handle passed arguments.
        std::vector<std::string> commandWithArgs;
        commandWithArgs.assign(argv + 1, argv + argc);
        
        cli->executeCommand(commandWithArgs);
    } else {
        // Make the CLI interactive
        cli->interactive();
    }
    return 0;
}