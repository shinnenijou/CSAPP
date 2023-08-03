#include "catch_amalgamated.hpp"
#include "catch_amalgamated.cpp"

int saturating_add(int x, int y)
{
    unsigned ux = x, uy = y, uxy = ux + uy;
    unsigned mask = INT_MIN;
    
    return ((ux ^ uy ^ mask) & mask)         // 同号mask, 异号0
        && !((uxy ^ ux ^ mask) & mask);       // 同号mask, 异号0
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( saturating_add( 0, 0) == 0 );
    REQUIRE( saturating_add( 0x80000000, 0x80FFFFFF) == 1 );
    REQUIRE( saturating_add( 0x80000000, 0x7FFFFFFF) == 0 );
    REQUIRE( saturating_add( 0x7FFFFFFF, 0x7FFFFFFF) == 1 );
}