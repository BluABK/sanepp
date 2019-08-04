#ifndef SANE_CLI_HPP
#define SANE_CLI_HPP

#include <map>
#include <list>
#include <string>
#include <iostream>

#include <entities/youtube_channel.hpp>
#include <api_handler/api_handler.hpp>
#include <db_handler/db_youtube_channels.hpp>

#define DEFAULT_INDENT 4

namespace sane {
    class CLI {
    public:
        explicit CLI();
        
        void interactive();

        static const std::string padStringValue(const std::string &string_t, std::size_t maxLength);

        void exit();

        void help();

        void executeCommand(const std::vector<std::string> &t_tokenizedInput);

        void getSubscriptionsFromApi();

        void printSubscriptionsJsonFromApi(int jsonIndent = DEFAULT_INDENT);

        static void printSubscriptionsFull();

        static void printSubscriptionsBasic();

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
    private:
        // Internal
        bool manuallyExit = false;

        // Define Command names.
        // Internal
        const std::string EXIT = "exit";
        const std::string HELP = "help";
        // Subscriptions -- Entity
        const std::string GET_SUBSCRIPTIONS_FROM_API = "get-subscriptions";
        const std::string PRINT_SUBSCRIPTIONS_PREFIX = "list-subscriptions-";
        const std::string PRINT_SUBSCRIPTIONS_FULL = PRINT_SUBSCRIPTIONS_PREFIX + "full";
        const std::string PRINT_SUBSCRIPTIONS_BASIC = PRINT_SUBSCRIPTIONS_PREFIX + "basic";
        // Subscriptions -- JSON
        const std::string PRINT_SUBSCRIPTIONS_JSON_FROM_API = "get-subscriptions-json";
        // Channel -- Entity
        const std::string PRINT_CHANNEL_BY_USERNAME = "get-channel-by-name";
        const std::string PRINT_CHANNEL_BY_ID = "get-channel-by-id";
        // Channels -- JSON
        const std::string PRINT_CHANNEL_JSON_BY_USERNAME = "get-channel-json-by-name";
        const std::string PRINT_CHANNEL_JSON_BY_ID = "get-channel-json-by-id";

        // list of commands (to be populated)
        std::map<std::string, std::string> commands;

        // Semantics
        const std::string COMMAND_PROMPT_STYLE = ">: ";
        size_t longestLine;
        int spacingLength = 4;

        std::shared_ptr<sane::APIHandler> api;
    };

} // namespace sane
#endif //SANE_CLI_HPP
