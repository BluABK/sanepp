#include <iostream>
#include <list>
#include <vector>
#include <sqlite3.h>
#include "db_handler.hpp"
namespace sane {
    // A way to store the callback results.
    std::vector<std::string> callbackResults;

    void addCallbackResult(const std::string &result) {
        callbackResults.push_back(result);
    }

    std::vector<std::string> getCallbackResults() {
        return callbackResults;
    }

    void clearCallbackResults() {
        callbackResults.clear();
    }

    /**
     * Callback function for sqlite3_exec() to call for every result row that it finds.
     *
     * @param NotUsed               An open sqlite3* database, in this case not used but declared to match signature.
     * @param resultSetColumnCount  The number of columns in the result set.
     * @param rowData               The row's data.
     * @param columnNames           The column names.
     * @return                      0.
     */
    static int callback(void *NotUsed, int resultSetColumnCount, char** rowData, char **columnNames) {
        int index;

        for (index = 0; index < resultSetColumnCount; index++) {
            std::cout <<  "The data in column " << columnNames[index] << " is: " << rowData[index] << std::endl;
            if (rowData[index]) {
                std::cout << columnNames[index] << " = " << rowData[index] << std::endl;
                addCallbackResult(rowData[index]);
            } else {
                std::cout << columnNames[index] << " = " << "NULL" << std::endl;
                addCallbackResult("NULL");
            }
        }

        printf("\n");

        return 0;
    }

    /**
     * Performs a SQLite statement using sqlite3_exec
     *
     * @param t_dbName         Filename for the database.
     * @param t_sqlStatement   A SQLite statement.
     * @return
     */
    int doSqlite3ExecStatement(const std::string &t_dbName, const std::string &t_sqlStatement) {
        sqlite3 *db;
        char *errorMessages = nullptr;
        int rc;

        // Open the database file.
        rc = sqlite3_open(t_dbName.c_str(), &db);
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return (SQLITE_ERROR);
        }

        // Execute the passed SQL statement and for every result row that it finds it will call callback.
        rc = sqlite3_exec(db, t_sqlStatement.c_str(), callback, nullptr, &errorMessages);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", errorMessages);
            sqlite3_free(errorMessages);
        }

        // Close database after all successful operations.
        sqlite3_close(db);

        return SQLITE_OK;
    }
}