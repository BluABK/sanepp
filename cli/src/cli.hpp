#ifndef SANE_CLI_HPP
#define SANE_CLI_HPP

#include <map>
#include <list>
#include <string>
#include <iostream>

#include <entities/youtube_channel.hpp>
#include <api_handler/api_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>

#define DEFAULT_INDENT  4

// Command categories
#define UNCATEGORISED   -1
#define CORE_CATEGORY    0
#define ENTITY_CATEGORY  1
#define JSON_CATEGORY    2
#define DB_CATEGORY      3

namespace sane {
    struct command_t {
        std::string name;
        std::string description;
        std::string usageSyntax;
        std::string usage;
        int category;
    };

    class CLI {
    public:
        explicit CLI();

        static std::vector<std::string> tokenize(const std::string &t_input, char t_delim = ' ');

        static std::map<std::string, std::string> stringToMap(const std::string &t_string);

        void addCommand(const std::string &t_name, const std::string &t_description,
                const int &t_category = UNCATEGORISED);

        void addCommand(const std::string &t_name, const std::string &t_description,
                        const std::string &t_usageSyntax = std::string(), const int &t_category = UNCATEGORISED);

        void addCommand(const std::string &t_name, const std::string &t_description,
                        const std::string &t_usageSyntax = std::string(), const std::string &t_usage = std::string(),
                        const int &t_category = UNCATEGORISED);
        
        void interactive();

        static const std::string padStringValue(const std::string &string_t, std::size_t maxLength);

        void exit();

        void help(bool extended = false);

        void executeCommand(const std::vector<std::string> &t_tokenizedInput);

        void getSubscriptionsFromApi();

        void printSubscriptionsJsonFromApi(int jsonIndent = DEFAULT_INDENT);

        void printSubscriptionsFull();

        void printSubscriptionsBasic();

        void printChannelFromApiByName(const std::string &t_input);

        void printChannelFromApiByName(const std::vector<std::string> &t_input);

        void printChannelFromApiById(const std::string &t_input);

        void printChannelFromApiById(const std::vector<std::string> &t_input);

        void printChannelJsonFromApiByName(const std::string &t_input, int jsonIndent = DEFAULT_INDENT);

        void printChannelJsonFromApiByName(const std::vector<std::string> &t_input,
                                                  int jsonIndent = DEFAULT_INDENT);

        void printChannelJsonFromApiById(const std::string &t_input, int jsonIndent = DEFAULT_INDENT);

        void printChannelJsonFromApiById(const std::vector<std::string> &t_input,
                                                int jsonIndent = DEFAULT_INDENT);

        void listActivitiesJsonFromApi(const std::vector<std::string> &t_input, int jsonIndent = DEFAULT_INDENT);

    private:
        // Internal
        bool manuallyExit = false;
        bool isInteractive = false;

        // Define Command names.
        // Internal
        const std::string EXIT = "exit";
        const std::string HELP = "help";
        const std::string HELP_EXTENDED = "help-extended";
        // Subscriptions -- Entity
        const std::string GET_SUBSCRIPTIONS_FROM_API = "get-subscriptions";
        const std::string PRINT_SUBSCRIPTIONS_PREFIX = "list-subscriptions-";
        const std::string PRINT_SUBSCRIPTIONS_FULL = PRINT_SUBSCRIPTIONS_PREFIX + "full";
        const std::string PRINT_SUBSCRIPTIONS_BASIC = PRINT_SUBSCRIPTIONS_PREFIX + "basic";
        // Subscriptions -- JSON
        const std::string PRINT_SUBSCRIPTIONS_JSON_FROM_API = "print-subscriptions-json";
        // Channel -- Entity
        const std::string PRINT_CHANNEL_BY_USERNAME = "get-channel-by-name";
        const std::string PRINT_CHANNEL_BY_ID = "get-channel-by-id";
        // Channels -- JSON
        const std::string PRINT_CHANNEL_JSON_BY_USERNAME = "get-channel-json-by-name";
        const std::string PRINT_CHANNEL_JSON_BY_ID = "get-channel-json-by-id";
        // Activities -- JSON
        const std::string LIST_ACTIVITIES_JSON = "list-activities-json";

        // Map of commands (to be populated)
        std::map<std::string, command_t> m_commands;
        std::list<int> m_commandCategories;

        // Semantics
        const std::string COMMAND_PROMPT_STYLE = ">: ";
        size_t longestLine = 0;
        size_t longestLineWithUsageSyntax = 0;
        bool usageSyntaxEntryExists = false;
        int spacingLength = 4;

        std::shared_ptr<sane::APIHandler> api;
    };

} // namespace sane
#endif //SANE_CLI_HPP
