/*
 *  Sane CLI.
 */

// Standard Libraries
#include <iostream>
#include <string>

// Sane++ Project specific
#include <api_handler.hpp>
#include <sapi_objs/sapi_test_static_json.hpp>
#include "cli.hpp"
#include <db/rusty_database.h>

int main() {
    // Instantiate the CLI class.
    std::shared_ptr<sane::CLI> cli = std::make_shared<sane::CLI>();
    
    // Make the CLI interactive
//    cli->interactive();

    rust_function();

    return 0;
}