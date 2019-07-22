/*
 *  Sane CLI.
 */

// Standard Libraries
#include <iostream>
#include <string>

// Sane++ Project specific
#include <api_handler.hpp>
#include <sapi_objs/sapi_test_static_json.hpp>
#include "basic_interactive_command_prompt.hpp"

int main() {
    std::shared_ptr<sane::BasicInteractiveCommandPrompt> cli = std::make_shared<sane::BasicInteractiveCommandPrompt>();

    return 0;
}