#include <iostream>
#include <list>
#include <sstream>

#include "cli.hpp"

namespace sane {
    CLI::CLI() {
        // Add commands to map of commands on the form of <name, description>.
        commands[EXIT] = "Exit program";
        commands[HELP] = "Print help";
        commands[GET_SUBSCRIPTIONS_FROM_API] = "Retrieves (and stores) a fresh list of subscriptions from the YouTube API.";
        commands[PRINT_SUBSCRIPTIONS_FULL] = "Lists all subscriptions separately as fully detailed blocks of text";
        commands[PRINT_SUBSCRIPTIONS_BASIC] = "Lists all subscriptions in a compact line-by-line form.";
        commands[PRINT_SUBSCRIPTIONS_JSON_FROM_API] = "Retrieves a fresh list of subscriptions from the YouTube API"
                                                    "and prints it as JSON.";
        commands[PRINT_CHANNEL_BY_USERNAME] = "Retrieve a channel by username.";
        commands[PRINT_CHANNEL_BY_ID] = "Retrieve and print a channel entity by channel ID.";
        commands[PRINT_CHANNEL_JSON_BY_USERNAME] = "Retrieve and print a channel JSON by username.";
        commands[PRINT_CHANNEL_JSON_BY_ID] = "Retrieve and print a channel JSON by channel ID.";

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

        // Instantiate the API Handler.
        api = std::make_shared<sane::APIHandler>();
    }

    void CLI::help() {
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

    void CLI::exit() {
        manuallyExit = true;
    }

    void CLI::executeCommand(const std::vector<std::string> &t_tokenizedInput) {
        // First item is the command itself
        const std::string command = t_tokenizedInput.front();

        // Put any remaining args into its own vector.
        const std::vector<std::string> args(t_tokenizedInput.begin() + 1, t_tokenizedInput.end());

        if (command == HELP) {
            help();
        } else if (command == EXIT) {
            exit();
        } else if (command == GET_SUBSCRIPTIONS_FROM_API) {
            getSubscriptionsFromApi();
        } else if (command == PRINT_SUBSCRIPTIONS_JSON_FROM_API){
            printSubscriptionsJsonFromApi();
        } else if (command == PRINT_SUBSCRIPTIONS_FULL) {
            printSubscriptionsFull();
        } else if (command == PRINT_SUBSCRIPTIONS_BASIC) {
            printSubscriptionsBasic();
        } else if (command == PRINT_CHANNEL_BY_USERNAME) {
            printChannelFromApiByName(args);
        } else if (command == PRINT_CHANNEL_JSON_BY_USERNAME) {
            printChannelJsonFromApiByName(args);
        } else if (command == PRINT_CHANNEL_BY_ID) {
            printChannelFromApiById(args);
        } else if (command == PRINT_CHANNEL_JSON_BY_ID) {
            printChannelJsonFromApiById(args);
        }

    }

    const std::string CLI::padStringValue(const std::string &string_t,
            const std::size_t maxLength) {
        std::string paddedString;

        if (string_t.length() <= maxLength) {
            paddedString = string_t + std::string(maxLength - string_t.length(), ' ');
        } else {
            // If the string is longer than pad length, give a warning and return the original string.
//            std::cerr << "padStringValue WARNING: Given string '" << string_t <<
//            "' is longer than specified max length: " << maxLength << "!" << std::endl;
            return string_t;
        }
        return paddedString;
    }

    /**
     * Tokenize a std::string with a given delimiter char.
     *
     * @param t_input   String to tokenize.
     * @param t_delim   Delimiter char.
     * @return          std::vector<std::string> tokens.
     */
    std::vector<std::string> tokenize(const std::string &t_input, char t_delim)  {
        std::vector<std::string> tokens;
        std::stringstream   mySstream(t_input);
        std::string         temp;

        while( getline( mySstream, temp, t_delim ) ) {
            tokens.push_back( temp );
        }

        return tokens;
    }

    void CLI::interactive() {
        std::string input;
        std::cout << COMMAND_PROMPT_STYLE;
        while (std::getline(std::cin, input)) { // quit the program with ctrl-d
            // Check if input is junk
            if ( input.empty()) {
                continue;
            }

            // Tokenize input (split on whitespace).
            std::vector<std::string> tokenizedInput = tokenize(input, ' ');

            // Check if input is a valid command.
            if ( commands.find(tokenizedInput.front()) == commands.end() ) {
                std::cout << "Error: Invalid command! (see 'help' for available commands)" << std::endl;
            } else{
                executeCommand(tokenizedInput);
            }
            // Check for manual exit here instead of at start due to the boolean being set from the above code,
            if (manuallyExit) {
                return;
            }
            std::cout << COMMAND_PROMPT_STYLE;
        }
    }
} // namespace sane