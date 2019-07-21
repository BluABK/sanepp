#ifndef SANE_BASIC_INTERACTIVE_COMMAND_PROMPT_HPP
#define SANE_BASIC_INTERACTIVE_COMMAND_PROMPT_HPP

#include <map>
#include <string>
#include <iostream>

namespace sane {
    class BasicInteractiveCommandPrompt {
    public:
        BasicInteractiveCommandPrompt();

    private:
        // Internal
        bool manuallyExit = false;

        // Define Command names.
        const std::string EXIT = "exit";
        const std::string HELP = "help";
        const std::string GET_SUBSCRIPTIONS_PREFIX = "get-subscriptions";
        const std::string GET_SUBSCRIPTIONS_FROM_API = GET_SUBSCRIPTIONS_PREFIX + "-from-remote";
        const std::string GET_SUBSCRIPTIONS_FROM_DB = GET_SUBSCRIPTIONS_PREFIX + "-from-database";

        // list of commands (to be populated)
        std::map<std::string, std::string> commands;

        // Semantics
        const std::string COMMAND_PROMPT_STYLE = ">: ";
        size_t longestLine;
        int spacingLength = 4;

        void exit();

        void help();

        void executeCommand(const std::string &command);

        void getSubscriptionsFromApi();
    };
} // namespace sane
#endif //SANE_BASIC_INTERACTIVE_COMMAND_PROMPT_HPP
