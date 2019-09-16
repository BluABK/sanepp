#include <iostream>
#include <list>
#include <sstream>

#include <config.hpp>
#include <log_handler/log_handler.hpp>
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

        // Log the added command
        log->debug("Added command '" + t_name + "': cat: " + std::to_string(t_category) + ", desc: " + t_description);
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
        if (t_name.length() > longestLine) {
            longestLine = t_name.length();
        }

        // Determine indentation spacing between command name and description, including usage syntax.
        if (t_name.length() + t_usageSyntax.length() > longestLineWithUsageSyntax) {
            longestLineWithUsageSyntax = (t_name + t_usageSyntax).length();
        }

        // Add the command to the commands map.
        m_commands[t_name] = command;

        // Log the added command
        log->debug("Added command '" + t_name + "': cat: " + std::to_string(t_category) + ", desc: " + t_description
        + ", Usage syntax: " + t_usageSyntax);
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

        // Log the added command
        log->debug("Added command '" + t_name + "': cat: " + std::to_string(t_category) + ", desc: " + t_description
                   + ", Usage syntax: " + t_usageSyntax + ", Usage: " + t_usage);
    }

    CLI::CLI() {
        // Create a logger instance.
        std::shared_ptr<sane::LogHandler> logHandler = std::make_shared<sane::LogHandler>();
        log = logHandler->createLogger("cli");

        // Add commands to map of commands on the form of: name, description, category, function ptr.
        addCommand(EXIT, "Exit program", CORE_CATEGORY);
        addCommand(HELP, "Print help", CORE_CATEGORY);
        addCommand(HELP_EXTENDED, "Print extended help", CORE_CATEGORY);
        addCommand(AUTHENTICATE_OAUTH2, "Authenticates OAuth 2.0 for YouTube (required for certain API calls)", UNCATEGORISED);

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
        addCommand(PRINT_VIDEOS_JSON, "Returns a list of videos.", "PART... FILTER [PARAM...]", JSON_CATEGORY);
        addCommand(PRINT_VIDEOS, "Prints a list of videos and their valid info.", "PART... FILTER [PARAM...]",
                ENTITY_CATEGORY);
        addCommand(PRINT_FULL_VIDEOS, "Prints a list of videos and all their info.", "PART... FILTER [PARAM...]",
                ENTITY_CATEGORY);
        addCommand(PRINT_PLAYLIST_ITEMS, "Prints a table of playlist videos.", "PLAYLIST_ID [PARAM...]", UNCATEGORISED);
        addCommand(PRINT_SUBSCRIPTIONS_FEED, "Prints a table of your subscriptions feed.", "LIMIT PART [PARAM...]",
                UNCATEGORISED);

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
            std::cout << PROJECT_NAME << " v" << PROJECT_VER << " [" << PROJECT_GIT_REV << "]" << std::flush;
            std::cout << " Command Line Interface." << std::endl;
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
        } else if (command == AUTHENTICATE_OAUTH2) {
            authenticateOAuth2();
        }  else if (command == GET_SUBSCRIPTIONS_FROM_API) {
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
        } else if (command == PRINT_VIDEOS_JSON) {
            printVideosJsonFromApi(args);
        } else if (command == PRINT_VIDEOS) {
            printVideosFromApi(args);
        } else if (command == PRINT_FULL_VIDEOS) {
            printVideosFromApi(args, PRINT_FULL_INFO);
        } else if (command == PRINT_PLAYLIST_ITEMS) {
            if (args.size() == 1) {
                // Has playlist ID
                printPlaylistVideos(args.at(0));
            } else if (args.size() == 2) {
                // Has playlist ID and optional params.
                printPlaylistVideos( args.at(0), stringToMap(args.at(1)) );
            } else {
                std::cerr << "Error in PRINT_PLAYLIST_ITEMS: invalid argument count: " << args.size() << std::endl;
            }
        } else if (command == PRINT_SUBSCRIPTIONS_FEED) {
            if (args.size() == 3) {
                int limit = std::stoi(args.at(0));
                const std::string part = args.at(1);
                const std::map<std::string, std::string> filter = std::map<std::string, std::string>();
                const std::map<std::string, std::string> optParams = stringToMap(args.at(2));

                printSubscriptionsFeed(limit, part, filter, optParams);
            } else if (args.empty()) {
                // Assume default values if no argument is given.
                const std::map<std::string, std::string> emptyFilter = std::map<std::string, std::string>();
                std::map<std::string, std::string> optParams = std::map<std::string, std::string>();

                optParams["maxResults"] = "50";

                printSubscriptionsFeed(50, "snippet,contentDetails", emptyFilter, optParams);
            } else {
                std::cerr << "Error in PRINT_PLAYLIST_ITEMS: invalid argument count: " << args.size() << std::endl;
            }
        }

    }

    void CLI::authenticateOAuth2() {
        log->info("Initiating OAuth2 authentication procedure.");

        // Step 1: Send a request to Google's OAuth 2.0 server
        log->info("Generating OAuth2 authentication URI...");
        std::string oauth2Uri = api->generateOAuth2URI();

        std::cout << "Open this link in a browser: " << oauth2Uri << std::endl;
        log->info("Generated OAuth2 authentication URI.");
        log->debug("OAuth2 authentication URI: " + oauth2Uri);

        // Step 2: Wait for Google user consent prompts, then handle the OAuth 2.0 server response.
        log->info("Waiting for Google user consent prompt (user action).");
        std::thread t1(sane::APIHandler::runOAuth2Server, OAUTH2_DEFAULT_REDIRECT_URI);
        t1.join();
        log->info("Got Google user consent prompt reply.");

        // Step 3: Exchange authorization code for refresh and access tokens.
        log->info("Authorizing OAuth2: Exchange authorization code for refresh and access tokens...");
        nlohmann::json response = api->authorizeOAuth2();

        if (!response.empty()) {
            log->info("Authorized OAuth2.");
            std::cout << "Successfully authorized OAuth2." << std::endl;
        } else {
            log->info("OAuth2 authentication aborted!");
            std::cout << "OAuth2 authentication aborted!" << std::endl;
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

    void CLI::interactive() {
        log->info("CLI is now in interactive mode.");
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