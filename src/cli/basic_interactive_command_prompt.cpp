#include <iostream>
#include <list>
#include <exception>

#include <youtube_subscription.hpp>
#include <api_handler.hpp>
#include "basic_interactive_command_prompt.hpp"

namespace sane {
    BasicInteractiveCommandPrompt::BasicInteractiveCommandPrompt() {
        // Add commands to map of commands on the form of <name, description>.
        commands[EXIT] = "Exit program";
        commands[HELP] = "Print help";
        commands[GET_SUBSCRIPTIONS_FROM_API] = "Retrieves a fresh list of subscriptions from the YouTube API.";
        commands[GET_SUBSCRIPTIONS_FROM_DB] = "Loads a list of subscriptions from the database.";

        // Determine indentation spacing between command name and description.
        // Create a map iterator and point it to the beginning of the map.
        auto it = commands.begin();

        // Iterate the map.
        longestLine = 0;
        while(it != commands.end())
        {
            if (it->first.length() > longestLine) {
                longestLine = it->first.length();
            }

            // Increment iterator to point at next command entry.
            it++;
        }

        // Do the loop
        std::string input;
        std::cout << COMMAND_PROMPT_STYLE;
        while (std::getline(std::cin, input)) { // quit the program with ctrl-d
            // Check if input is junk
            if ( input.empty()) {
                continue;
            }
            // Check if input is a valid command.
            if ( commands.find(input) == commands.end() ) {
                std::cout << "Error: Invalid command! (see 'help' for available commands)" << std::endl;
            } else{
                executeCommand(input);
            }
            // Check for manual exit here instead of at start due to the boolean being set from the above code,
            if (manuallyExit) {
                return;
            }
            std::cout << COMMAND_PROMPT_STYLE;
        }
    }

    void BasicInteractiveCommandPrompt::help() {
        // Create a map iterator and point it to the beginning of the map.
        auto it = commands.begin();

        // Subtract 7 for the length of the string "Command", and then also remember to add the spacing length.
        std::cout << "Command" << std::string(longestLine - 7 + spacingLength, ' ') << "Description" << std::endl;

        // Iterate the map.
        while(it != commands.end())
        {
            // Set the spacing variable (varies for each item).
            std::string commandSpacing(longestLine - it->first.length() + spacingLength, ' ');

            // Print list of commands on the form of <name, description>.
            std::cout << it->first << commandSpacing << it->second << std::endl;

            // Increment iterator to point at next command entry.
            it++;
        }
        std::cout << std::endl;
    }

    void BasicInteractiveCommandPrompt::exit() {
        manuallyExit = true;
    }

    void BasicInteractiveCommandPrompt::executeCommand(const std::string &command) {
        if (command == HELP) {
            help();
        } else if (command == EXIT) {
            exit();
        } else if (command == GET_SUBSCRIPTIONS_FROM_API) {
            getSubscriptionsFromApi();
        } else if (command == GET_SUBSCRIPTIONS_FROM_DB) {
            std::cerr << "Error: NOT IMPLEMENTED" << std::endl;
        }

    }
    /**
     * Retrieves a list of YouTube subscription objects from YouTube API via SaneAPI
     */
    void BasicInteractiveCommandPrompt::getSubscriptionsFromApi() {
        std::list<std::shared_ptr<YoutubeSubscription>> subscriptions;
        subscriptions = sapiGetSubscriptions();
    }
} // namespace sane