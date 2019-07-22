#include <iostream>
#include <list>

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
        commands[PRINT_SUBSCRIPTIONS_FULL] = "Lists all subscriptions separately as fully detailed blocks of text";
        commands[PRINT_SUBSCRIPTIONS_BASIC] = "Lists all subscriptions in a compact line-by-line form.";

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
        } else if (command == PRINT_SUBSCRIPTIONS_FULL) {
            printSubscriptionsFull();
        } else if (command == PRINT_SUBSCRIPTIONS_BASIC) {
            printSubscriptionsBasic();
        }

    }
    /**
     * Retrieves a list of YouTube subscription objects from YouTube API via SaneAPI
     */
    std::list<std::shared_ptr<YoutubeSubscription>> BasicInteractiveCommandPrompt::getSubscriptionsFromApi() {
        subscriptions = sapiGetSubscriptions();

        return subscriptions;
    }

    void BasicInteractiveCommandPrompt::printSubscriptionsFull() {
        int counter = 1;  // Humanized counting.
        for (auto & subscription : subscriptions) {
            std::cout << "Sub#" << counter << ":" << std::endl;
            subscription->print(4);
            counter++;
        }
    }

    const std::string padStringValue(const std::string &string_t, const int maxLength) {
        std::string paddedString = string_t + std::string(string_t.length() - maxLength, ' ');

        return paddedString;
    }

    void BasicInteractiveCommandPrompt::printSubscriptionsBasic() {
        // Spacing between each column item.
        const std::string columnSpacing(4, ' ');
        // Spacing based on the number of subscriptions.
        const std::string subscriptionNumberingSpacing(subscriptions.size(), ' ');
        // Max length of IDs, used to calculate padding.
        const int idItemMaxLength = 24;

        int counter = 1;  // Humanized counting.
        // ID: Len max ca. 24
        std::cout << "Sub#" << subscriptionNumberingSpacing <<
        "Channel ID" << columnSpacing << "Uploads playlist ID" << columnSpacing << "Channel title" <<  std::endl;

        for (auto & subscription : subscriptions) {
            // Pad ID ID item columns with spaces to ensure a uniform indentation.
            const std::string paddedChannelId = padStringValue(subscription->getChannelId(), idItemMaxLength);
            const std::string paddedUploadsPlaylistId = padStringValue(
                    subscription->getUploadsPlaylist(), idItemMaxLength);

            std::cout << counter << paddedChannelId << columnSpacing << paddedUploadsPlaylistId << columnSpacing <<
            subscription->getTitle() << std::endl;

            counter++;
        }
    }
} // namespace sane