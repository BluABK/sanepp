#include <types.hpp>

namespace sane {
    char* strptime(const char* s, const char* f, struct tm* tm) {
        // Isn't the C++ standard lib nice? std::get_time is defined such that its
        // format parameters are the exact same as strptime. Of course, we have to
        // create a string stream first, and imbue it with the current C locale, and
        // we also have to make sure we return the right things if it fails, or
        // if it succeeds, but this is still far simpler an implementation than any
        // of the versions in any of the C standard libraries.
        //
        // src: https://stackoverflow.com/a/33542189
        std::istringstream input(s);
        input.imbue(std::locale(setlocale(LC_ALL, nullptr)));
        input >> std::get_time(tm, f);
        if (input.fail()) {
            return nullptr;
        }
        return (char*)(s + input.tellg());
    }

    /**
     * Tokenize a std::string with a given delimiter char.
     *
     * @param t_input   String to tokenize.
     * @param t_delim   Delimiter char.
     * @return          std::vector<std::string> tokens.
     */
    std::vector<std::string> tokenize2(const std::string &t_input, char t_delim)  {
        std::vector<std::string> tokens;
        std::stringstream   mySstream(t_input);
        std::string         temp;

        while( getline( mySstream, temp, t_delim ) ) {
            tokens.push_back( temp );
        }

        return tokens;
    }
} // namespace sane