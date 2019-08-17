#include <youtube/toolkit.hpp>

namespace sane {
    bool hasItems(const nlohmann::json &t_jsonList) {
        // JSON is valid (not empty object).
        if (!t_jsonList.empty()) {
            // JSON contains pageInfo object.
            if (t_jsonList.find("pageInfo") != t_jsonList.end()) {
                // JSON pageInfo object contains totalResults item.
                if (t_jsonList["pageInfo"].find("totalResults") != t_jsonList["pageInfo"].end()) {
                    // JSON pageInfo object totalResults is a valid numeric value.
                    if (t_jsonList["pageInfo"]["totalResults"].is_number()) {
                        // JSON pageInfo object totalResults numeric value is larger than zero.
                        if (t_jsonList["pageInfo"]["totalResults"].get<int>() > 0) {
                            return true;
                        }
                    } else {
                        std::cerr <<  "hasItems Error: JSON[pageInfo][totalResults] is not numeric:\n"
                                  << t_jsonList.dump(4) << std::endl;
                    }
                } else {
                    std::cerr <<  "hasItems Error: JSON[pageInfo] does not contain totalResults:\n"
                              << t_jsonList.dump(4) << std::endl;
                }
            } else {
                std::cerr <<  "hasItems Error: JSON does not contain pageInfo:\n" << t_jsonList.dump(4) << std::endl;
            }
        }
        return false;
    }
}