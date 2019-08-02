#ifndef SANE_CLI_HPP
#define SANE_CLI_HPP

#include <map>
#include <list>
#include <string>
#include <iostream>

#include <entities/youtube_channel.hpp>
#include <api_handler/api_handler.hpp>

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

        static void getSubscriptionsFromApi();

        static void printSubscriptionsFull();

        static void printSubscriptionsBasic();

        static void printChannelFromApiByName(const std::string &t_input);

        static void printChannelFromApiByName(const std::vector<std::string> &t_input);

        static void printChannelFromApiById(const std::string &t_input);

        static void printChannelFromApiById(const std::vector<std::string> &t_input);
    private:
        // Internal
        bool manuallyExit = false;

        // Define Command names.
        // Internal
        const std::string EXIT = "exit";
        const std::string HELP = "help";
        // Subscriptions
        const std::string GET_SUBSCRIPTIONS_FROM_API = "get-subscriptions";
        const std::string PRINT_SUBSCRIPTIONS_PREFIX = "list-subscriptions-";
        const std::string PRINT_SUBSCRIPTIONS_FULL = PRINT_SUBSCRIPTIONS_PREFIX + "full";
        const std::string PRINT_SUBSCRIPTIONS_BASIC = PRINT_SUBSCRIPTIONS_PREFIX + "basic";
        // Channel(s)
        const std::string GET_CHANNEL_BY_USERNAME = "get-channel-by-name";
        const std::string GET_CHANNEL_BY_ID = "get-channel-by-id";

        // list of commands (to be populated)
        std::map<std::string, std::string> commands;

        // Semantics
        const std::string COMMAND_PROMPT_STYLE = ">: ";
        size_t longestLine;
        int spacingLength = 4;

    };

} // namespace sane
#endif //SANE_CLI_HPP
