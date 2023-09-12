#include "catch_amalgamated.hpp"
#include "catch_amalgamated.cpp"


int fits_bits(int x, int n);

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( saturating_add( 0, 0) == 0 );
    REQUIRE( saturating_add( 0x80000000, 0x80FFFFFF) == 1 );
    REQUIRE( saturating_add( 0x80000000, 0x7FFFFFFF) == 0 );
    REQUIRE( saturating_add( 0x7FFFFFFF, 0x7FFFFFFF) == 1 );
}