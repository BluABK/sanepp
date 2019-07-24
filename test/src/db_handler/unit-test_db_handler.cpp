#include <iostream>
#include <fstream>
#include <cstdio>
#include <vector>

#include <catch2/catch.hpp>

#include <db_handler.hpp>

TEST_CASE ("1: Testing sane::DBHandler: CREATE TABLE test_db_functionality") {
    // Setup
    std::string databaseFilename = "sane_db_handler_test.db";
    std::vector<std::string> sqlWriteStatement;
    int returnStatus;

    // Create the table
    sqlWriteStatement = {databaseFilename, "CREATE TABLE test_db_functionality ("
                                           "SomeID int, SomeWord varchar(255)"
                                           ");"};
    returnStatus = sane::callDBWithCArgs(sqlWriteStatement);

    REQUIRE(returnStatus == 0);
}

TEST_CASE( "2: Testing sane::DBHandler" ) {
    // Setup
    std::string databaseFilename = "sane_db_handler_test.db";
    std::vector<std::string> sqlWriteStatement;
    int returnStatus;
    std::vector<std::string> callbackResultZ;

    SECTION ("Write to TABLE test_db_functionality") {
        // Add some rows
        sqlWriteStatement = {databaseFilename, "INSERT INTO test_db_functionality (SomeID, SomeWord)"
                                               "VALUES (0, 'Database');"};
        returnStatus =  sane::callDBWithCArgs(sqlWriteStatement);
        REQUIRE(returnStatus == 0);

        sqlWriteStatement = {databaseFilename, "INSERT INTO test_db_functionality (SomeID, SomeWord)"
                                               "VALUES (1, 'Self-Test');"};
        returnStatus = sane::callDBWithCArgs(sqlWriteStatement);
        REQUIRE(returnStatus == 0);
    }

    SECTION("Read from TABLE test_db_functionality") {
        // Read some rows
        sqlWriteStatement = {databaseFilename, "SELECT SomeWord FROM test_db_functionality;"};
        returnStatus =  sane::callDBWithCArgs(sqlWriteStatement);

        REQUIRE(returnStatus == 0);

        callbackResultZ = sane::getCallbackResults();
        REQUIRE(callbackResultZ.size() == 2);
        REQUIRE(callbackResultZ.at(0) == "Database");
        REQUIRE(callbackResultZ.at(1) == "Self-Test");

        sane::clearCallbackResults();
        REQUIRE(sane::getCallbackResults().empty());
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