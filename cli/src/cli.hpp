#ifndef SANE_CLI_HPP
#define SANE_CLI_HPP

#include <map>
#include <list>
#include <string>
#include <iostream>

#include <entities/youtube_channel.hpp>
#include <api_handler/api_handler.hpp>

namespace sane {
    class CLI {
    public:
        explicit CLI();
        
        void interactive();

        static const std::string padStringValue(const std::string &string_t, std::size_t maxLength);
    private:
        // Internal
        bool manuallyExit = false;

        // Define Command names.
        const std::string EXIT = "exit";
        const std::string HELP = "help";
        const std::string GET_SUBSCRIPTIONS_FROM_API = "get-subscriptions";
        const std::string PRINT_SUBSCRIPTIONS_PREFIX = "list-subscriptions-";
        const std::string PRINT_SUBSCRIPTIONS_FULL = PRINT_SUBSCRIPTIONS_PREFIX + "full";
        const std::string PRINT_SUBSCRIPTIONS_BASIC = PRINT_SUBSCRIPTIONS_PREFIX + "basic";

        // list of commands (to be populated)
        std::map<std::string, std::string> commands;

        // Semantics
        const std::string COMMAND_PROMPT_STYLE = ">: ";
        size_t longestLine;
        int spacingLength = 4;

        void exit();

        void help();

        void executeCommand(const std::string &command);

        static void getSubscriptionsFromApi();

        static void printSubscriptionsFull();

        static void printSubscriptionsBasic();
    };
} // namespace sane
#endif //SANE_CLI_HPP
