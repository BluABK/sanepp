#ifndef SANE_LEXICAL_ANALYSIS_HPP
#define SANE_LEXICAL_ANALYSIS_HPP

#include <vector>
#include <string>

namespace sane {
    std::vector<std::string> tokenize(const std::string &t_input, char t_delim);
} // namespace sane

#endif //SANE_LEXICAL_ANALYSIS_HPP
