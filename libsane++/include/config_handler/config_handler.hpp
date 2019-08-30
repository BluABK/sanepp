#ifndef SANE_CONFIG_HANDLER_HPP
#define SANE_CONFIG_HANDLER_HPP

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>

// 3rd party libraries.
#include <nlohmann/json.hpp>
#include <spdlog/logger.h>

namespace sane {
    class ConfigHandler {
    public:
        ConfigHandler();

        nlohmann::json getConfig();

        void setConfig(nlohmann::json &t_json);

        bool hasSection(const std::string &t_section);

        nlohmann::json getSection(const std::string &t_section, bool debug = false);

        const std::string getString(const std::string &t_section);

        bool isString(const std::string &t_section);

        bool isNumber(const std::string &t_section);

        int getInt(const std::string &t_section);

        long int getLongInt(const std::string &t_section);

        const std::list<std::string> getStringList(const std::string &t_section);
    private:
        const std::string CONFIG_FILE = "config.json";
        const char SECTION_SEPARATOR = '/';
        std::shared_ptr<spdlog::logger> log;

    };
} // namespace sane

#endif //SANE_CONFIG_HANDLER_HPP
