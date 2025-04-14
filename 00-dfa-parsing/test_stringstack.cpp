#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "StringStack.h"

TEST_CASE("Basics", "[short]") {
    StringStack ss("abc");
    REQUIRE(ss.get() == 'a');
    REQUIRE(ss.get() == 'b');
    REQUIRE(ss.get() == 'c');
    REQUIRE(ss.get() == -1);
}

TEST_CASE("Unget", "[short]") {
    StringStack ss("abc");
    REQUIRE(ss.unget() == -1);
    REQUIRE(ss.get() == 'a');
    REQUIRE(ss.unget() != -1);
    REQUIRE(ss.get() == 'a');
    REQUIRE(ss.get() == 'b');
    REQUIRE(ss.get() == 'c');
    REQUIRE(ss.unget() == 0);
    REQUIRE(ss.get() == 'c');
    REQUIRE(ss.get() == -1);
}
