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
     * **DISCOURAGED** See https://stackoverflow.com/a/31168999
     *
     * @param t_dbName         Filename for the database.
     * @param t_sqlStatement   A SQLite statement.
     * @return
     */
    int doRawSqlite3ExecStatement(const std::string &t_dbName, const std::string &t_sqlStatement) {
        sqlite3 *db;
        char *errorMessages = nullptr;
        int rc;

        // Open the database file.
        rc = sqlite3_open(t_dbName.c_str(), &db);
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return (SQLITE_CANTOPEN);
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

    /**
     * Perform an SQLite3 statement using sqlite3_prepare_v2()/sqlite3_step()/sqlite3_column_*()/sqlite3_finalize()
     * calls so that you can read the data in the same place where you actually need to handle it.
     *
     * Based on https://stackoverflow.com/a/31168999
     *
     * @param t_sql     An SQL statement.
     * @return
     */
    int prepareSqlStatement(const std::string &t_dbName, const std::string &t_sql) {
        sqlite3 *db;
        sqlite3_stmt *sqlite3PreparedStatment;

        int rc;

        // Open the database file.
        rc = sqlite3_open(t_dbName.c_str(), &db);
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return (SQLITE_ERROR);
        }

        rc = sqlite3_prepare_v2(db, t_sql.c_str(), -1, &sqlite3PreparedStatment, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database '" << t_dbName << "': " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return SQLITE_CANTOPEN;
        }
        // Step through the statements and store results to variables.
        while ((rc = sqlite3_step(sqlite3PreparedStatment)) == SQLITE_ROW) {
            int id           = sqlite3_column_int (sqlite3PreparedStatment, 0);
            const char *name = sqlite3_column_text(sqlite3PreparedStatment, 1);
            // ...
        }
        if (rc != SQLITE_DONE) {
            std::cerr << "Error sqlite3_step ended but does not have status SQLITE_DONE: " << sqlite3_errmsg(db) <<
            std::endl;
        }
        sqlite3_finalize(sqlite3PreparedStatment);

        return SQLITE_OK;
    }

    int stepThroughSqlStatement(sqlite3_stmt &t_sqlite3PreparedStatment) {

    }
}