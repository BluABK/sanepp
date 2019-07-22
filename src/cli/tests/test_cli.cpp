#include <catch2/catch.hpp>

#include "cli.hpp"

TEST_CASE( "Testing sane::CLI" ) {
    // Instantiate the CLI class.
    std::shared_ptr<sane::CLI> cli = std::make_shared<sane::CLI>();

    SECTION( "[function] padStringValue" ) {

        SECTION( "Strings are padded with correct length.", "[multi-file:2]" ) {
            REQUIRE( cli->padStringValue("Test", 10).length() == 10 );
        }

        SECTION( "Strings longer than padding max length aren't padded.", "[multi-file:2]" ) {
            REQUIRE( cli->padStringValue("Test", 0).length() == std::string("Test").length() );
            REQUIRE( cli->padStringValue("Test", 1).length() == std::string("Test").length() );
            REQUIRE( cli->padStringValue("Test", 2).length() == std::string("Test").length() );
            REQUIRE( cli->padStringValue("Test", 3).length() == std::string("Test").length() );
            REQUIRE( cli->padStringValue("Test", 4).length() == std::string("Test").length() );
        }
    }
}