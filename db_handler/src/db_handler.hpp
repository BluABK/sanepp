#ifndef SANE_DB_HANDLER_HPP
#define SANE_DB_HANDLER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <functional>
#include <type_traits>

#include <sqlite3.h>

namespace sane {
    // sqlite_exec called with custom callback function provided by parameter.
    // FIXME: **DISABLED**: Works for one single query and then SQLite throws "query aborted"
    #if generic_false_statement
    typedef int (*sqlite3_callback_)(void*,int,char**, char**);
    int execSqlStatementCustom(sqlite3_callback_ t_callbackFunction, const std::string &t_dbName,
            const std::string &t_sqlStatement);
    #endif

    // sqlite_exec called with default callback function
    int execSqlStatement(const std::string &t_dbName, const std::string &t_sqlStatement);


    // A way to store the callback results.
    void addCallbackResult(const std::string &result);

    std::vector<std::string> getCallbackResults();

    void clearCallbackResults();
}

#endif //SANE_DB_HANDLER_HPP
