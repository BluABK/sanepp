#ifndef SANE_DB_HANDLER_HPP
#define SANE_DB_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <sqlite3.h>

namespace sane {
    // Method a: sqlite3_exec.
    int doRawSqlite3ExecStatement(const std::string &t_dbName, const std::string &t_sqlStatement);

    // A way to store the callback results.
    void addCallbackResult(const std::string &result);

    std::vector<std::string> getCallbackResults();

    void clearCallbackResults();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Method B: The better method using prepared statements.
    int prepareAndRunSqlStatement(std::function<int(sqlite3_stmt *sqlite3PreparedStatement)> &t_callbackFunction,
                                  const std::string &t_dbName, const std::string &t_sql);
}

#endif //SANE_DB_HANDLER_HPP
