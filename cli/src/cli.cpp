#include <iostream>
#include <list>

#include <entities/youtube_channel.hpp>
#include <api_handler/api_handler.hpp>
#include <api_handler/entity_reponse.hpp>
#include <db_handler/db_youtube_channels.hpp>
#include <sstream>

#include "cli.hpp"

namespace sane {
    CLI::CLI() {
        // Add commands to map of commands on the form of <name, description>.
        commands[EXIT] = "Exit program";
        commands[HELP] = "Print help";
        commands[GET_SUBSCRIPTIONS_FROM_API] = "Retrieves a fresh list of subscriptions from the YouTube API.";
        commands[PRINT_SUBSCRIPTIONS_FULL] = "Lists all subscriptions separately as fully detailed blocks of text";
        commands[PRINT_SUBSCRIPTIONS_BASIC] = "Lists all subscriptions in a compact line-by-line form.";
        commands[GET_CHANNEL_BY_USERNAME] = "Retrieve a channel by username.";
        commands[GET_CHANNEL_BY_ID] = "Retrieve a channel by its ID.";

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
        } else if (command == PRINT_SUBSCRIPTIONS_FULL) {
            printSubscriptionsFull();
        } else if (command == PRINT_SUBSCRIPTIONS_BASIC) {
            printSubscriptionsBasic();
        } else if (command == GET_CHANNEL_BY_USERNAME) {
            printChannelFromApiByName(args);
        } else if (command == GET_CHANNEL_BY_ID) {
            printChannelFromApiById(args);
        }

    }
    /**
     * Retrieves a list of YouTube subscription objects from YouTube API via SaneAPI
     */
    void CLI::getSubscriptionsFromApi() {
        sapiGetSubscriptions();
    }

    void CLI::printSubscriptionsFull() {
        // Fetch list of channels
        std::list <std::shared_ptr<YoutubeChannel>> channels;
        channels = sane::getChannelsFromDB(NO_ERROR_LOG);
        int counter = 1;  // Humanized counting.
        for (auto & channel : channels) {
            std::cout << "Sub#" << counter << ":" << std::endl;
            channel->print(DEFAULT_INDENT);
            counter++;
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

    void CLI::printSubscriptionsBasic() {
        // Fetch list of channels
        std::list <std::shared_ptr<YoutubeChannel>> channels;
        channels = sane::getChannelsFromDB(NO_ERROR_LOG);
        // Spacing between each column item.
        const std::string columnSpacing(4, ' ');
        // Pad subscription counter based on the amount of digits in the sum total.
        const std::size_t maxCounterDigitAmount = std::to_string(channels.size()).length();
        // Max length of IDs, used to calculate padding.
        const std::size_t idItemMaxLength = 24;
        // Humanized counting.
        int counter = 1;

        // Print a table heading/legend.
        std::cout << padStringValue("#", maxCounterDigitAmount) << columnSpacing <<
        padStringValue("Channel ID", idItemMaxLength) << columnSpacing <<
        padStringValue("Uploads playlist ID", idItemMaxLength) <<
        columnSpacing << "Channel title" <<  std::endl;

        for (auto & channel : channels) {
            // Pad item columns with spaces to ensure a uniform indentation.
            const std::string paddedChannelId = padStringValue(channel->getId(), idItemMaxLength);
            const std::string paddedUploadsPlaylistId = padStringValue(
                    channel->getUploadsPlaylist(), idItemMaxLength);
            const std::string paddedNumbering = padStringValue(std::to_string(counter), maxCounterDigitAmount);

            std::cout << paddedNumbering << columnSpacing <<
            paddedChannelId << columnSpacing << paddedUploadsPlaylistId << columnSpacing << channel->getTitle()
            << std::endl;

            counter++;
        }
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

    void CLI::printChannelFromApiByName(const std::string &t_input) {
        std::shared_ptr<YoutubeChannel> channel = sapiGetChannelByUsername(t_input);
        channel->print(DEFAULT_INDENT);
    }

    void CLI::printChannelFromApiByName(const std::vector<std::string> &t_input) {
        if (t_input.empty()) {
            std::cout << "Error: no arguments given, required: 1." << std::endl;
        } else {
            printChannelFromApiByName(t_input.front());
        }
    }

    void CLI::printChannelFromApiById(const std::string &t_input) {
        std::shared_ptr<YoutubeChannel> channel = sapiGetChannelById(t_input);
        channel->print(DEFAULT_INDENT);
    }

    void CLI::printChannelFromApiById(const std::vector<std::string> &t_input) {
        if (t_input.empty()) {
            std::cout << "Error: no arguments given, required: 1." << std::endl;
        } else {
            printChannelFromApiById(t_input.front());
        }
    }
} // namespace sane