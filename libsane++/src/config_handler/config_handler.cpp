#include <vector>

#include <lexical_analysis.hpp>

#include <config_handler/config_handler.hpp>

namespace sane {
    ConfigHandler::ConfigHandler() = default;

    /**
     * Reads config file into a nlohmann::json object and returns it.
     *
     * @return
     */
    nlohmann::json ConfigHandler::getConfig() {
        // Open config file.
        std::ifstream ifs{ "config.json" };

        // Read config ifstream into a JSON object.
        nlohmann::json config = nlohmann::json::parse(ifs);

        // Close config file.
        ifs.close();

        return config;
    }

    /**
     * Checks if a given section exists or not.
     *
     * @param t_section String of path to JSON/config section.
     * @return
     */
    bool ConfigHandler::hasSection(const std::string &t_section) {
        nlohmann::json config = getConfig();

        if (config.empty()) {
            std::cerr << "ConfigHandler Error: Empty config file!" << std::endl;
            return false;
        }

        std::vector<std::string> sections = tokenize(t_section, SECTION_SEPARATOR);
        nlohmann::json valueJson = config;
        for (const auto& section : sections) {
            if (valueJson.find(section) != valueJson.end()) {
                valueJson = valueJson[section];
            } else {
                std::cerr << "ConfigHandler Error: No such section: \"" << section << "\" (\"" << t_section << "\")"
                          << std::endl;
                return false;
            }
        }

        return true;
    }

    /**
     * Returns an config/JSON section (as JSON).
     *
     * @param t_section String of path to JSON/config section.
     * @param debug     Print debug info.
     * @return          nlohmann::json or empty nlohmann::json::object().
     */
    nlohmann::json ConfigHandler::getSection(const std::string &t_section, bool debug) {
        nlohmann::json config = getConfig();

        if (hasSection(t_section)) {
            std::vector<std::string> keys = tokenize(t_section, SECTION_SEPARATOR);

            nlohmann::json valueJson = config;

            for (const auto& key : keys) {
                valueJson = valueJson[key];
                if (debug) {
                    std::cout << "DEBUG: currentDepth: " << valueJson.dump() << std::endl;
                }
            }

            return valueJson;
        }

        return nlohmann::json::object();
    }

    /**
     * Read a config section and return its value as a std::string.
     *
     * @param t_section String of path to JSON/config section.
     * @return          Result or empty string.
     */
    const std::string ConfigHandler::getString(const std::string &t_section)  {
        // Open config file.
        std::ifstream ifs{ "config.json" };

        // Read config ifstream into a JSON object.
        nlohmann::json config = nlohmann::json::parse(ifs);

        // Close config file.
        ifs.close();

        if (hasSection(t_section)) {
            return getSection(t_section).get<std::string>();
        }

        return {};
    }
} // namespace sane