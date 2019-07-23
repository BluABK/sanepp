/*
 *  Sane CLI.
 */

// Standard Libraries
#include <iostream>
#include <string>

// Sane++ Project specific
#include <api_handler.hpp>
#include <sapi_objs/sapi_test_static_json.hpp>
#include <sanepp_rusty_db.h>
#include "cli.hpp"

int main() {
    // Instantiate the CLI class.
    std::shared_ptr<sane::CLI> cli = std::make_shared<sane::CLI>();
    
    // Make the CLI interactive
//    cli->interactive();

    rust_function();

    return 0;
}