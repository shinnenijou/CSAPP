#include <iostream>
#include "common/catch2/catch_amalgamated.hpp"
#include "common/rio/rio.h"

int test(int num){
    return num;
}

TEST_CASE("Unit Test", "[Test]"){
    REQUIRE ( test(3) == 3 );
    REQUIRE ( test(3) == 3 );
}