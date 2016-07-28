#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <tick.hpp>

#include "catch.hpp"

using namespace MetaSim;
using namespace std;

TEST_CASE("TestTick", "testBoolean")
{
    Tick a;
    Tick b(5);
    Tick c(5);

    REQUIRE(a < b);
    REQUIRE(b == c);
    REQUIRE(c >= a);
}

TEST_CASE("TestTick2", "testOperations")
{
    Tick a;
    Tick b(100);
    Tick c("4ns");
    Tick d(".4us");

    REQUIRE((100*c) == d);
    REQUIRE((a+b) == 100);
    REQUIRE((b/100) == 1);
    REQUIRE(a++ == 0);
    REQUIRE(++a == 2);
    REQUIRE((d/400) == 1);
}



