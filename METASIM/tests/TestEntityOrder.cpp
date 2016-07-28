#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "myentity.hpp"

//#include "gtest/gtest.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

using namespace std;

TEST_CASE("TestEventOrder", "testPost")
{ 
    MyEntity me("Pippo");
    SIMUL.run(12);
    REQUIRE(me.isAFirst());
    REQUIRE(me.getCounter() == 2);
}
