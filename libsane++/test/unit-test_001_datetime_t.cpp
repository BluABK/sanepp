#include <catch2/catch.hpp>

#include <types.hpp>

TEST_CASE ("1: Testing sane custom types: datetime_t: Initialize with variable ISO 8601 syntax.") {
    // ISO 8601: YYYY-MM-DDThh:mm:ss.sTZD
    std::string iso8601;

    // Insantiate a datetime
    sane::datetime_t datetime = sane::datetime_t();

    SECTION("ISO 8601 with no UTC designator: (2019-08-14T23:29:30).") {
        iso8601 = "2019-08-14T23:29:30";

        datetime.fromISO8601(iso8601);

        REQUIRE(datetime.iso8601 == iso8601);
        REQUIRE(datetime.millisecond == 0);
    }

    SECTION("ISO 8601 with no decimals: (2019-08-14T23:29:30.Z).") {
        iso8601 = "2019-08-14T23:29:30.Z";

        datetime.fromISO8601(iso8601);

        REQUIRE(datetime.iso8601 == iso8601);
        REQUIRE(datetime.millisecond == 0);
    }

    SECTION("ISO 8601 with 1 ms decimal: (2019-08-14T23:29:30.6Z).") {
        iso8601 = "2019-08-14T23:29:30.6Z";

        datetime.fromISO8601(iso8601);

        REQUIRE(datetime.iso8601 == iso8601);
        REQUIRE(datetime.millisecond == 6);
    }

    SECTION("ISO 8601 with 2 ms decimals: (2019-08-14T23:29:30.09Z).") {
        iso8601 = "2019-08-14T23:29:30.09Z";

        datetime.fromISO8601(iso8601);

        REQUIRE(datetime.iso8601 == iso8601);
        REQUIRE(datetime.millisecond == 9);
    }

    SECTION("ISO 8601 with 3 ms decimals: (2019-08-14T23:29:30.078Z).") {
        iso8601 = "2019-08-14T23:29:30.078Z";

        datetime.fromISO8601(iso8601);

        REQUIRE(datetime.iso8601 == iso8601);
        REQUIRE(datetime.millisecond == 78);
    }
}