#include <catch2/catch.hpp>

#include <sanepp_rusty_db.h>

TEST_CASE( "Testing sane::database" ) {
    SECTION( "Check that DB is linked to C" ) {
        std::string retval = test_rust_linkage();
        REQUIRE(retval == "Tested rust linkage");
    }
}