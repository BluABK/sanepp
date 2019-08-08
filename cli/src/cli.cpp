#include <iostream>
#include <list>
#include <sstream>

#include "cli.hpp"

namespace sane {
    void CLI::addCommand(const std::string &t_name, const std::string &t_description, const int &t_category) {
        // Define a command type struct.
        command_t command;

        // Assign values and its function.
        command.name = t_name;
        command.description = t_description;
        command.category = t_category;

        // Add category to list of categories and clear non-unique elements.
        m_commandCategories.push_back(t_category);
        m_commandCategories.unique();

        // Determine indentation spacing between command name and description.
        if (t_name.length() > longestLine) {
            longestLine = t_name.length();
        }

        // Add the command to the commands map.
        m_commands[t_name] = command;
    }

    void CLI::addCommand(const std::string &t_name, const std::string &t_description, const std::string &t_usageSyntax,
                         const int &t_category) {
        if (!t_usageSyntax.empty()) {
            usageSyntaxEntryExists = true;
        }

        // Define a command type struct.
        command_t command;

        // Assign values and its function.
        command.name = t_name;
        command.description = t_description;
        command.usageSyntax = " " + t_usageSyntax;
        command.category = t_category;

        // Add category to list of categories and clear non-unique elements.
        m_commandCategories.push_back(t_category);
        m_commandCategories.unique();

        // Determine indentation spacing between command name and description.
        if ((t_name + t_usageSyntax).length() > longestLineWithUsageSyntax) {
            longestLineWithUsageSyntax = (t_name + t_usageSyntax).length();
        }

        // Add the command to the commands map.
        m_commands[t_name] = command;
    }

    void CLI::addCommand(const std::string &t_name, const std::string &t_description, const std::string &t_usageSyntax,
                         const std::string &t_usage, const int &t_category) {
        if (!t_usageSyntax.empty()) {
            usageSyntaxEntryExists = true;
        }

        // Define a command type struct.
        command_t command;

        // Assign values and its function.
        command.name = t_name;
        command.description = t_description;
        command.usageSyntax = " " + t_usageSyntax;
        command.usage = t_usage;
        command.category = t_category;

        // Add category to list of categories and clear non-unique elements.
        m_commandCategories.push_back(t_category);
        m_commandCategories.unique();

        // Determine indentation spacing between command name and description.
        if ((t_name + t_usageSyntax).length() > longestLineWithUsageSyntax) {
            longestLineWithUsageSyntax = (t_name + t_usageSyntax).length();
        }

        // Add the command to the commands map.
        m_commands[t_name] = command;
    }

    CLI::CLI() {
        // Add commands to map of commands on the form of: name, description, category, function ptr.
        addCommand(EXIT, "Exit program", CORE_CATEGORY);
        addCommand(HELP, "Print help", CORE_CATEGORY);
        addCommand(HELP_EXTENDED, "Print extended help", CORE_CATEGORY);
        addCommand(GET_SUBSCRIPTIONS_FROM_API, "Retrieves (and stores) a fresh list of subscriptions from "
                                               "the YouTube API.", DB_CATEGORY);
        addCommand(PRINT_SUBSCRIPTIONS_FULL, "Lists all subscriptions separately as fully detailed blocks of text",
                   ENTITY_CATEGORY);
        addCommand(PRINT_SUBSCRIPTIONS_BASIC, "Lists all subscriptions in a compact line-by-line form.",
                   ENTITY_CATEGORY);
        addCommand(PRINT_SUBSCRIPTIONS_JSON_FROM_API, "Retrieves a fresh list of subscriptions from the YouTube API"
                                                      " and prints it.", JSON_CATEGORY);
        addCommand(PRINT_CHANNEL_BY_USERNAME, "Retrieve a channel by username.", "NAME", ENTITY_CATEGORY);
        addCommand(PRINT_CHANNEL_BY_ID, "Retrieve and print a channel entity by channel ID.", "CHAN_ID",
                ENTITY_CATEGORY);
        addCommand(PRINT_CHANNEL_JSON_BY_USERNAME, "Retrieve and print a channel JSON by username.", "NAME",
                JSON_CATEGORY);
        addCommand(PRINT_CHANNEL_JSON_BY_ID, "Retrieve and print a channel JSON by channel ID.", "CHAN_ID",
                JSON_CATEGORY);
        addCommand(LIST_ACTIVITIES_JSON, "Returns a list of channel activity events.",
                "PART... FILTER [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_CAPTIONS_JSON, "Returns a list of caption tracks that are associated with a specified video.",
                   "PART... VIDEO_ID [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_CHANNELS_JSON, "Returns a collection of zero or more channel resources.",
                   "PART... FILTER [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_CHANNEL_SECTIONS_JSON, "Returns a list of channelSection resources.",
                   "PART... FILTER [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_COMMENTS_JSON, "Returns a list of comments.",
                   "PART... FILTER [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_COMMENT_THREADS_JSON, "Returns a list of comment threads.", "PART... FILTER [PARAM...]",
                JSON_CATEGORY);
        addCommand(LIST_GUIDE_CATEGORIES_JSON, "Returns a list of categories that can be associated with "
                                               "YouTube channels.", "PART... FILTER [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_I18N_LANGUAGES_JSON, "Returns a list of application languages that the "
                                             "YouTube website supports.", "PART [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_I18N_REGIONS_JSON, "Returns a list of content regions that the "
                                           "YouTube website supports.", "PART [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_PLAYLIST_ITEMS_JSON, "Returns a collection of playlist items. You can retrieve all of "
                                             "the playlist items in a specified playlist or retrieve one or "
                                             "more playlist items by their unique IDs", "PART... FILTER [PARAM...]",
                   JSON_CATEGORY);
        addCommand(LIST_PLAYLISTS_JSON, "Returns a collection of playlists.", "PART... FILTER [PARAM...]",
                   JSON_CATEGORY);
        addCommand(SEARCH_JSON, "Returns a collection of search results.", "PARAM...",
                   JSON_CATEGORY);
        addCommand(FILTERED_SEARCH_JSON, "Returns a collection of filtered search results.", "FILTER [PARAM...]",
                   JSON_CATEGORY);
        addCommand(LIST_SUBSCRIPTIONS_JSON, "Returns subscription resources.", "PART... FILTER [PARAM...]",
                   JSON_CATEGORY);
        addCommand(LIST_VIDEO_ABUSE_REPORT_REASONS_JSON, "Retrieve a list of reasons that can be used to report "
                                                         "abusive videos.", "PART [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_VIDEO_CATEGORIES_JSON, "Returns a list of categories that can be associated with "
                                               "YouTube videos.", "PART... FILTER [PARAM...]", JSON_CATEGORY);
        addCommand(LIST_VIDEOS_JSON, "Returns a list of videos.", "PART... FILTER [PARAM...]", JSON_CATEGORY);

        // Instantiate the API Handler.
        api = std::make_shared<sane::APIHandler>();
    }

    /**
     * Prints a "help" table listing Command names and descriptions.
     *
     * @param extended  If true, print extended help information.
     */
    void CLI::help(bool extended) {
        std::string cmdHeader;
        size_t longestLine_ = 0;

        if (!isInteractive) {
            std::cout << "Sane++ (rudimentary) Command Line Interface." << std::endl;
            std::cout << std::endl;
            std::cout << "Run without arguments to enter interactive mode." << std::endl;
            std::cout << std::endl;
        }

        if (usageSyntaxEntryExists and extended) {
            longestLine_ = longestLineWithUsageSyntax;
            cmdHeader = "Command (w/ usage syntax)";
        } else {
            longestLine_ = longestLine;
            cmdHeader = "Command";
        }

        std::cout << cmdHeader << std::string(longestLine_ - cmdHeader.length() + spacingLength, ' ')
                  << "Description" << std::endl;

        // Iterate the map of commands.
        for (auto const& commandEntry : m_commands) {
            std::string commandName = commandEntry.second.name;

            // If usage syntax is defined and we're in extended mode, append it to the command name.
            if (!commandEntry.second.usageSyntax.empty() and extended) {
                commandName += commandEntry.second.usageSyntax;
            }

            // Set the spacing variable (varies for each item).
            std::string commandSpacing(longestLine_ - commandName.length() + spacingLength, ' ');

            // Print list of commands on the form of <name, description>.
            std::cout << commandName << commandSpacing << commandEntry.second.description << std::endl;

            // If usage field is defined, add it on the following line, in the description column.
            if (!commandEntry.second.usage.empty()) {
                // Iterate over newlines in order to preserve indent in help "table".
                for (auto const& line : tokenize(commandEntry.second.usage, '\n')) {
                    std::cout << std::string(commandName.length(), ' ') << commandSpacing
                              << line << std::endl;
                }
            }

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
        } else if (command == HELP_EXTENDED) {
            help(true);
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
        } else if (command == LIST_ACTIVITIES_JSON) {
            listActivitiesJsonFromApi(args);
        } else if (command == LIST_CAPTIONS_JSON) {
            listCaptionsJsonFromApi(args);
        } else if (command == LIST_CHANNELS_JSON) {
            listChannelsJsonFromApi(args);
        } else if (command == LIST_CHANNEL_SECTIONS_JSON) {
            listChannelSectionsJsonFromApi(args);
        } else if (command == LIST_COMMENTS_JSON) {
            listCommentsJsonFromApi(args);
        } else if (command == LIST_COMMENT_THREADS_JSON) {
            listCommentThreadsJsonFromApi(args);
        } else if (command == LIST_GUIDE_CATEGORIES_JSON) {
            listGuideCategoriesJsonFromApi(args);
        } else if (command == LIST_I18N_LANGUAGES_JSON) {
            listi18nLanguagesJsonFromApi(args);
        } else if (command == LIST_I18N_REGIONS_JSON) {
            listi18nRegionsJsonFromApi(args);
        } else if (command == LIST_PLAYLIST_ITEMS_JSON) {
            listPlaylistItemsJsonFromApi(args);
        } else if (command == LIST_PLAYLISTS_JSON) {
            listPlaylistsJsonFromApi(args);
        } else if (command == SEARCH_JSON) {
            listSearchJsonFromApi(args);
        } else if (command == FILTERED_SEARCH_JSON) {
            listFilteredSearchJsonFromApi(args);
        } else if (command == LIST_SUBSCRIPTIONS_JSON) {
            listSubscriptionsJsonFromApi(args);
        } else if (command == LIST_VIDEO_ABUSE_REPORT_REASONS_JSON) {
            listVideoAbuseReportReasonsJsonFromApi(args);
        } else if (command == LIST_VIDEO_CATEGORIES_JSON) {
            listVideoCategoriesJsonFromApi(args);
        } else if (command == LIST_VIDEOS_JSON) {
            listVideosJsonFromApi(args);
        }

    }

    const std::string CLI::padStringValue(const std::string &string_t,
            const std::size_t maxLength) {
        std::string paddedString;

        if (string_t.length() <= maxLength) {
            paddedString = string_t + std::string(maxLength - string_t.length(), ' ');
        } else {
            // If the string is longer than pad length, return the original string.
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
    std::vector<std::string> CLI::tokenize(const std::string &t_input, char t_delim)  {
        std::vector<std::string> tokens;
        std::stringstream   mySstream(t_input);
        std::string         temp;

        while( getline( mySstream, temp, t_delim ) ) {
            tokens.push_back( temp );
        }

        return tokens;
    }

    void CLI::interactive() {
        isInteractive = true;
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
            if ( m_commands.find(tokenizedInput.front()) == m_commands.end() ) {
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