#include "catch_amalgamated.hpp"
#include "catch_amalgamated.cpp"

#include <climits>

int saturating_add(int x, int y);

TEST_CASE( "Addition that asturates to TMin or TMax" ,"[saturating_add]" ) {
    REQUIRE( saturating_add(1, 1) == 2 );
    REQUIRE( saturating_add(INT_MAX - 1, 1) == INT_MAX );
    REQUIRE( saturating_add(INT_MAX - 1, 2) == INT_MAX );
    REQUIRE( saturating_add(INT_MAX, 1) == INT_MAX );
    REQUIRE( saturating_add(INT_MAX, INT_MAX) == INT_MAX );
    REQUIRE( saturating_add(INT_MAX, -1) == INT_MAX - 1 );

    REQUIRE( saturating_add(-1, -1) == -2);
    REQUIRE( saturating_add(INT_MIN + 1, -1) == INT_MIN );
    REQUIRE( saturating_add(INT_MIN + 1, -2) == INT_MIN );
    REQUIRE( saturating_add(INT_MIN, -1) == INT_MIN );
    REQUIRE( saturating_add(INT_MIN, INT_MIN) == INT_MIN );
    REQUIRE( saturating_add(INT_MIN, 1) == INT_MIN + 1 );

    REQUIRE( saturating_add(INT_MIN, INT_MAX) ==  -1 );
    REQUIRE( saturating_add(0, 0) ==  0 );

}

int tsub_ok(int, int);

TEST_CASE( "Determine whether arguments can be subtracted without overflow" ,"[tsub_ok]" ) {
    REQUIRE( tsub_ok(1, 1) == 1);
    REQUIRE( tsub_ok(0x80000001, 0) == 1);
    REQUIRE( tsub_ok(0x80000001, 1) == 1);
    REQUIRE( tsub_ok(0x80000001, 2) == 0);
    REQUIRE( tsub_ok(0x80000001, INT_MIN) == 1);
    REQUIRE( tsub_ok(0, INT_MIN) == 0);
    REQUIRE( tsub_ok(-1, INT_MIN) == 1);
    REQUIRE( tsub_ok(INT_MAX - 1, 0) == 1);
    REQUIRE( tsub_ok(INT_MAX - 1, -1) == 1);
    REQUIRE( tsub_ok(INT_MAX - 1, -2) == 0);
}