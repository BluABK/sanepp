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
     * Proxy function that converts a vector of C++ strings to a traditional C-String array,
     * and then calls doSqlite3ExecStatement with the converted array as param.
     *
     * @param list
     * @return
     */
    int callDBWithCArgs(const std::vector<std::string> &list) {
        std::vector<const char*> strings;
        for (const auto & i : list)
            strings.push_back(i.c_str());
        int returnStatus = doSqlite3ExecStatement(strings.size(), strings.data());

        return returnStatus;
    }

    /**
     * Performs a sql statement using sqlite3_exec
     * @param argc
     * @param argv
     * @return
     */
    int doSqlite3ExecStatement(int argc, const char **argv) {
        const char *databaseName = argv[0];
        const char *sqlStatement = argv[1];
        sqlite3 *db;
        char *errorMessages = nullptr;
        int rc;

        // Check that the required amount of args were given.
        if (argc != 2) {
            std::cerr << argc << std::endl;
            fprintf(stderr, "Usage: DATABASE SQL-STATEMENT\n");
            return (1);
        }
        // Open the database file.
        rc = sqlite3_open(databaseName, &db);
        if (rc) {
            fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
            sqlite3_close(db);
            return (1);
        }
        // Execute the passed SQL statement and for every result row that it finds it will call callback.
        rc = sqlite3_exec(db, sqlStatement, callback, nullptr, &errorMessages);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", errorMessages);
            sqlite3_free(errorMessages);
        }

        sqlite3_close(db);
        return 0;
    }
}