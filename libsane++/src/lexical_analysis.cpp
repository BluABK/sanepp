#include <sstream>
#include <vector>
#include <string>

#include <lexical_analysis.hpp>

namespace sane {
    /**
     * Tokenize a std::string with a given delimiter char.
     *
     * @param t_input   String to tokenize.
     * @param t_delim   Delimiter char.
     * @return          std::vector<std::string> tokens.
     */
    std::vector<std::string> tokenize(const std::string &t_input, char t_delim)  {
        std::vector<std::string> tokens;
        std::stringstream   mySstream(t_input);
        std::string         temp;

        while( getline( mySstream, temp, t_delim ) ) {
            tokens.push_back( temp );
        }

        return tokens;
    }
} // namespace sane
