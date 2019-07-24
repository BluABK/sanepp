#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <string>

#include <catch2/catch.hpp>

#include <db_handler.hpp>

TEST_CASE ("1: Testing sane::DBHandler.prepareAndRunSqlStatement: CREATE TABLE test_db_functionality") {
    // Setup
    const std::string databaseFilename = "sane_db_handler_test.db";
    int returnStatus;

    // Create the table
    const std::string sqlStatement = "CREATE TABLE test_db_functionality ("
                                     "SomeID int, SomeWord varchar(255)"
                                     ");";
    returnStatus = sane::prepareAndRunSqlStatement(sane::dummyCallback, databaseFilename, sqlStatement);

    REQUIRE(returnStatus == 0);
}

// Define a callback function to retrieve values
const unsigned char *word1_cstring;
const unsigned char *word2_cstring;

int myCallback(sqlite3_stmt *t_sqlite3PreparedStatement) {
    int rc;

    while ((rc = sqlite3_step(t_sqlite3PreparedStatement)) == SQLITE_ROW) {
        std::cout << sqlite3_column_text(t_sqlite3PreparedStatement, 0);
        std::cout << sqlite3_column_text(t_sqlite3PreparedStatement, 1);

        word1_cstring = sqlite3_column_text(t_sqlite3PreparedStatement, 0);
        word2_cstring = sqlite3_column_text(t_sqlite3PreparedStatement, 1);
    }

    return SQLITE_DONE;
}

TEST_CASE( "2: Testing sane::DBHandler.prepareAndRunSqlStatement" ) {
    // Setup
    const std::string databaseFilename = "sane_db_handler_test.db";
    std::string sqlStatement;
    int returnStatus;
    std::vector<std::string> callbackResultZ;

    SECTION ("Write to TABLE test_db_functionality") {
        // Add some rows
        sqlStatement = "INSERT INTO test_db_functionality (SomeID, SomeWord)"
                       "VALUES (0, 'Database');";
        returnStatus = sane::prepareAndRunSqlStatement(sane::dummyCallback, databaseFilename, sqlStatement);
        REQUIRE(returnStatus == SQLITE_OK);

        sqlStatement = "INSERT INTO test_db_functionality (SomeID, SomeWord)"
                       "VALUES (1, 'Self-Test');";
        returnStatus = sane::prepareAndRunSqlStatement(sane::dummyCallback,databaseFilename, sqlStatement);
        REQUIRE(returnStatus == SQLITE_OK);
    }

    SECTION("Read from TABLE test_db_functionality") {
        // Read some rows
        sqlStatement = "SELECT SomeWord FROM test_db_functionality;";
        returnStatus = sane::prepareAndRunSqlStatement(myCallback, databaseFilename, sqlStatement);

        REQUIRE(returnStatus == SQLITE_OK);

        const auto * word1_solution = (const unsigned char*) "Database";
        const auto * word2_solution = (const unsigned char*) "Self-Test";
        REQUIRE(word1_cstring == word1_solution);
        REQUIRE(word2_cstring == word2_solution);
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