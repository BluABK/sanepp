#if foo
#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

#include <catch2/catch.hpp>

#include <db_handler.hpp>

/**
 * Callback function for sqlite3_exec() to call for every result row that it finds.
 *
 * @param t_unused       An open sqlite3* database, in this case not used but declared to match signature.
 * @param t_columnCount  The number of columns in the result set.
 * @param t_rowData      The row's data.
 * @param t_columnNames  The column names.
 * @return               0.
 */
static int customCallbackTestCaseOne(void *t_unused, int t_columnCount, char** t_rowData, char **t_columnNames) {
    int index;

    // "Use" the unused function to clear irrelevant unused warnings.
    if (t_unused) {}

    for (index = 0; index < t_columnCount; index++) {
        std::cout <<  "customCallbackTestCaseOne: The data in column " << t_columnNames[index] << " is: " <<
        t_rowData[index] << std::endl;
        if (t_rowData[index]) {
            std::cout << t_columnNames[index] << " = " << t_rowData[index] << std::endl;
        } else {
            std::cout << t_columnNames[index] << " = " << "NULL" << std::endl;
        }
    }

    printf("\n");

    return 0;
}

TEST_CASE ("1: Testing sane::DBHandler.execSqlStatement (default callback): CREATE TABLE test_db_functionality") {
    // Setup
    const std::string databaseFilename = "sane_db_handler_test.db";
    int returnStatus;

    // Create the table
    const std::string sqlStatement = "CREATE TABLE test_db_functionality ("
                                     "SomeID int, SomeWord varchar(255)"
                                     ");";
    returnStatus = sane::execSqlStatementCustom(customCallbackTestCaseOne, databaseFilename, sqlStatement);

    REQUIRE(returnStatus == 0);
}

static int customCallbackTestCaseTwoWrite(void *t_unused, int t_columnCount, char** t_rowData, char **t_columnNames) {
    int index;

    // "Use" the unused function to clear irrelevant unused warnings.
    if (t_unused) {}

    for (index = 0; index < t_columnCount; index++) {
        std::cout <<  "customCallbackTestCaseTwoWrite: The data in column " << t_columnNames[index] << " is: " <<
                  t_rowData[index] << std::endl;
        if (t_rowData[index]) {
            std::cout << t_columnNames[index] << " = " << t_rowData[index] << std::endl;
        } else {
            std::cout << t_columnNames[index] << " = " << "NULL" << std::endl;
        }
    }

    printf("\n");

    return 0;
}

// Define a custom callback function to retrieve values
//const char *word1_cstring;
//const char *word2_cstring;
//std::string word1;
//std::string word2;
std::vector<std::string> readData;

static int customCallbackTestCaseTwoRead(void *t_unused, int t_columnCount, char** t_rowData, char **t_columnNames) {
    int index;

    // "Use" the unused function to clear irrelevant unused warnings.
    if (t_unused) {}

    for (index = 0; index < t_columnCount; index++) {
        std::cout <<  "customCallbackTestCaseTwoRead: The data in column " << t_columnNames[index] << " is: " <<
        t_rowData[index] << std::endl;
        if (t_rowData[index]) {
            std::cout << index << " " << t_columnNames[index] << " = " << t_rowData[index] << std::endl;
            readData.push_back(t_rowData[index]);
        } else {
            std::cout << t_columnNames[index] << " = " << "NULL" << std::endl;
            readData.push_back("NULL");
        }
    }

    return SQLITE_DONE;
}

TEST_CASE( "2: Testing sane::DBHandler.execSqlStatement (custom callback)" ) {
    // Setup
    const std::string databaseFilename = "sane_db_handler_test.db";
    std::string sqlStatement;
    int returnStatus;
    std::vector<std::string> callbackResultZ;

    SECTION ("Write to TABLE test_db_functionality") {
        // Add some rows
        sqlStatement = "INSERT INTO test_db_functionality (SomeID, SomeWord)"
                       "VALUES (0, 'Database');";
        returnStatus = sane::execSqlStatementCustom(customCallbackTestCaseTwoWrite, databaseFilename, sqlStatement);
        REQUIRE(returnStatus == 0);

        sqlStatement = "INSERT INTO test_db_functionality (SomeID, SomeWord)"
                       "VALUES (1, 'Self-Test');";
        returnStatus = sane::execSqlStatementCustom(customCallbackTestCaseTwoWrite, databaseFilename, sqlStatement);
        REQUIRE(returnStatus == 0);
    }

    SECTION("Read from TABLE test_db_functionality") {
        // Read some rows
        sqlStatement = "SELECT SomeWord FROM test_db_functionality;";
        returnStatus = sane::execSqlStatementCustom(customCallbackTestCaseTwoRead, databaseFilename, sqlStatement);
//        returnStatus = sane::execSqlStatement(databaseFilename, sqlStatement);

        REQUIRE(returnStatus == 0);
//        std::cout << readData.at(1);
//        REQUIRE(readData.at(0) == "Database");
//        REQUIRE(readData.at(1) == "Self-Test");

//        callbackResultZ = sane::getCallbackResults();
//        REQUIRE(callbackResultZ.size() == 2);
//        REQUIRE(callbackResultZ.at(0) == "Database");
//        REQUIRE(callbackResultZ.at(1) == "Self-Test");
//
//        sane::clearCallbackResults();
//        REQUIRE(sane::getCallbackResults().empty());
    }

    // Technically just cleanup, but it's nice with a confirmation nevertheless.
    SECTION("Remove the test_db_functionality.db file") {
        // Delete the file.
        std::remove(databaseFilename.c_str());

        // Check that it was actually deleted.
        bool failedToOpenFile = !std::ifstream(databaseFilename);
        if (!failedToOpenFile) {
            std::perror("Did NOT fail to open file (i.e. deletion failed): ");
        }
        REQUIRE(failedToOpenFile);
    }
}
#endif