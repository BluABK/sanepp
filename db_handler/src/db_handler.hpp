#ifndef SANE_DB_HANDLER_HPP
#define SANE_DB_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <sqlite3.h>

namespace sane {
    int doRawSqlite3ExecStatement(const std::string &t_dbName, const std::string &t_sqlStatement);

    // A way to store the callback results.
    void addCallbackResult(const std::string &result);

    std::vector<std::string> getCallbackResults();

    void clearCallbackResults();
}

#endif //SANE_DB_HANDLER_HPP
