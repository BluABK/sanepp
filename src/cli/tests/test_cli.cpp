#include <catch2/catch.hpp>

#include "cli.hpp"

// Instantiate the CLI class.
std::shared_ptr<sane::CLI> cli = std::make_shared<sane::CLI>();

TEST_CASE( "Strings are padded with correct length (pass)", "[multi-file:2]" ) {
    REQUIRE( cli->padStringValue("Test", 10).length() == 10 );
}

TEST_CASE( "Strings longer than padding max length aren't padded (pass)", "[multi-file:2]" ) {
    REQUIRE( cli->padStringValue("Test", 0).length() == std::string("Test").length() );
    REQUIRE( cli->padStringValue("Test", 1).length() == std::string("Test").length() );
    REQUIRE( cli->padStringValue("Test", 2).length() == std::string("Test").length() );
    REQUIRE( cli->padStringValue("Test", 3).length() == std::string("Test").length() );
    REQUIRE( cli->padStringValue("Test", 4).length() == std::string("Test").length() );
}