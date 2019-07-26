/*
 *  Sane CLI.
 */

// Standard Libraries
#include <iostream>
#include <string>

// Sane++ Project specific
#include <api_handler/api_handler.hpp>
#include <db_handler.hpp>
#include "cli.hpp"

int main() {
    // Instantiate the CLI class.
    std::shared_ptr<sane::CLI> cli = std::make_shared<sane::CLI>();
    
    // Make the CLI interactive
    cli->interactive();

    return 0;
}