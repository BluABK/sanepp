#ifndef SANE_CLI_HPP
#define SANE_CLI_HPP

#include <map>
#include <list>
#include <string>
#include <iostream>

#include <src/youtube_channel.hpp>
#include <src/api_handler.hpp>

namespace sane {
    class CLI {
    public:
        explicit CLI();
        
        void interactive();

        const std::string padStringValue(const std::string &string_t, std::size_t maxLength);
    private:
        // Internal
        bool manuallyExit = false;

        // YouTube
        std::list<std::shared_ptr<YoutubeChannel>> subscriptions;

        // Define Command names.
        const std::string EXIT = "exit";
        const std::string HELP = "help";
        const std::string GET_SUBSCRIPTIONS_PREFIX = "get-subscriptions-";
        const std::string GET_SUBSCRIPTIONS_FROM_API = GET_SUBSCRIPTIONS_PREFIX + "from-remote";
        const std::string GET_SUBSCRIPTIONS_FROM_DB = GET_SUBSCRIPTIONS_PREFIX + "from-database";
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

        std::list<std::shared_ptr<YoutubeChannel>> getSubscriptionsFromApi();


        void printSubscriptionsFull();

        void printSubscriptionsBasic();

    };
} // namespace sane
#endif //SANE_CLI_HPP
