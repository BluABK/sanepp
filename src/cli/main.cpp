/*
 *  Sane CLI.
 */

// Standard Libraries
#include <iostream>
#include <string>

// Sane++ Project specific
#include <api_handler.hpp>
#include <sapi_objs/sapi_test_static_json.hpp>
#include "interactive_cli.hpp"

int main() {
    std::shared_ptr<sane::InteractiveCLI> cli = std::make_shared<sane::InteractiveCLI>();

    return 0;
}