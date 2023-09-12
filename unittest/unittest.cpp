#include "catch_amalgamated.hpp"
#include "catch_amalgamated.cpp"

int fits_bits(int x, int n);

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( fits_bits( 0x78, 1) == 0 );
    REQUIRE( fits_bits( 0x78, 7) == 0 );
    REQUIRE( fits_bits( 0x78, 8) == 1 );
    REQUIRE( fits_bits( 0x78, 9) == 1 );
    REQUIRE( fits_bits( 0xFF, 1) == 0 );
    REQUIRE( fits_bits( 0xFF, 7) == 0 );
    REQUIRE( fits_bits( 0xFF, 8) == 0 );
    REQUIRE( fits_bits( 0xFF, 9) == 1 );
    REQUIRE( fits_bits( 0xFFFFFFFA, 1) == 0 );
    REQUIRE( fits_bits( 0xFFFFFFFA, 2) == 0 );
    REQUIRE( fits_bits( 0xFFFFFFFA, 3) == 0 );
    REQUIRE( fits_bits( 0xFFFFFFFA, 4) == 1 );
    REQUIRE( fits_bits( 0xFFFFFFFA, 5) == 1 );

}