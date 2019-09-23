#include <vector>
#include <iomanip>

#include <lexical_analysis.hpp>

#include <config_handler/config_handler.hpp>
#include <log_handler/log_handler.hpp>

namespace sane {
    ConfigHandler::ConfigHandler(bool createLog) {
        // This bool is usually false when invoked by e.g. the LogHandler, to avoid circular recursion.
        if (createLog) {
            std::shared_ptr<sane::LogHandler> logHandler = std::make_shared<sane::LogHandler>();
            log = logHandler->createLogger("config");
        }
    }

    /**
     * Reads config file into a nlohmann::json object and returns it.
     *
     * @return
     */
    nlohmann::json ConfigHandler::getConfig() {
        nlohmann::json config;
        // Open config file.
        std::ifstream ifs{CONFIG_FILE};

        // Read config ifstream into a JSON object.
        try {
            config = nlohmann::json::parse(ifs);
        } catch (nlohmann::detail::parse_error &parseExc) {
            log->warn("getConfig() failed to parse JSON input: " + std::string(parseExc.what()));
            log->flush();
//            throw parseExc;
        }

        // Close config file.
        ifs.close();

        return config;
    }

    /**
     * Writes nlohmann::json object into a config file.
     *
     * @return
     */
    void ConfigHandler::setConfig(nlohmann::json &t_json) {
        std::ofstream ofs(CONFIG_FILE);
        ofs << std::setw(4) << t_json << std::endl;

        ofs.close();
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
            log->error("Empty config file!");
            return false;
        }

        std::vector<std::string> sections = tokenize(t_section, SECTION_SEPARATOR);
        nlohmann::json valueJson = config;
        for (const auto& section : sections) {
            if (valueJson.find(section) != valueJson.end()) {
                valueJson = valueJson[section];
            } else {
                log->error(std::string("No such section: \"").append(section).append(
                           "\" (\"").append(t_section).append("\")"));
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
                    log->debug("currentDepth: " + valueJson.dump());
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
        nlohmann::json config = getConfig();

        std::string retval;

        retval = getSection(t_section).get<std::string>();

        return retval;
    }

    bool ConfigHandler::isString(const std::string &t_section) {
        // Open config file.
        nlohmann::json config = getConfig();

        auto section = getSection(t_section);

        return section.is_string() and !section.empty();
    }

    bool ConfigHandler::isNumber(const std::string &t_section) {
        // Open config file.
        nlohmann::json config = getConfig();

        auto section = getSection(t_section);

        return section.is_number();
    }

    bool ConfigHandler::isUnsignedNumber(const std::string &t_section) {
        // Open config file.
        nlohmann::json config = getConfig();

        auto section = getSection(t_section);

        return section.is_number_unsigned();
    }


    /**
     * Read a config section and return its value as an int.
     *
     * @param t_section String of path to JSON/config section.
     * @return          Result or 0.
     */
    int ConfigHandler::getInt(const std::string &t_section)  {
        // Open config file.
        nlohmann::json config = getConfig();

        int retval;

        auto section = getSection(t_section);

        if (section.is_number()) {
            retval = section.get<int>();
        } else {
            log->error("getInt(" + t_section + ") ERROR: NaN: " + section.dump());
            retval = 0;
        }

        return retval;
    }

    /**
     * Read a config section and return its value as a long int.
     *
     * @param t_section String of path to JSON/config section.
     * @return          Result or 0.
     */
    long int ConfigHandler::getLongInt(const std::string &t_section)  {
        // Open config file.
        nlohmann::json config = getConfig();

        long int retval = 0;

        auto section = getSection(t_section);

        if (section.is_number()) {
            retval = section.get<long int>();
        } else {
            log->error("getLongInt(" + t_section + ") ERROR: NaN: " + section.dump());
        }

        return retval;
    }

    /**
     * Read a config section and return its value as a unsigned int.
     *
     * @param t_section String of path to JSON/config section.
     * @return          Result or 0.
     */
    long int ConfigHandler::getUnsignedInt(const std::string &t_section)  {
        // Open config file.
        nlohmann::json config = getConfig();

        unsigned int retval = 0;

        auto section = getSection(t_section);

        if (section.is_number()) {
            if (section.is_number_unsigned()) {
                retval = section.get<unsigned int>();
            } else {
                log->error("getUnsignedInt(" + t_section + ") ERROR: Not unsigned/positive: " + section.dump());
            }
        } else {
            log->error("getUnsignedInt(" + t_section + ") ERROR: NaN: " + section.dump());
        }

        return retval;
    }

    const std::list<std::string> ConfigHandler::getStringList(const std::string &t_section) {
        // Open config file.
        nlohmann::json config = getConfig();

        // ISO C++03 14.2/4: The member template name must be prefixed by the keyword template.
        return getSection(t_section).template get<std::list<std::string>>();
    }
} // namespace sane