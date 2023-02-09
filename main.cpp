#include <iostream>
#include "common/catch2/catch_amalgamated.hpp"
#include "common/rio/rio.h"

int test(){
    return 3;
}

TEST_CASE("Unit Test", "[Test]"){
    REQUIRE ( test() == 3 );
    REQUIRE ( test() == 3 );
}