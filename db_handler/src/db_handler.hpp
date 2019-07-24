#ifndef SANE_DB_HANDLER_HPP
#define SANE_DB_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>

namespace sane {
//    int callDBWithCArgs(const std::vector<std::string> &list);
    int doSqlite3ExecStatement(const std::string &t_dbName, const std::string &t_sqlStatement);

    // A way to store the callback results.
    void addCallbackResult(const std::string &result);

    std::vector<std::string> getCallbackResults();

    void clearCallbackResults();
}

#endif //SANE_DB_HANDLER_HPP
