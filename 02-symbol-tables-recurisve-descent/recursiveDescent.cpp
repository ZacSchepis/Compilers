#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ranges>
#include "helpers.h"

bool parseRecurse(std::stack<std::string>& input, std::vector<std::string>& output);

TEST_CASE("recursive descent") {
    SUBCASE("aaabbbccc$") {
        auto input = makeStringStack("aaabbbccc$");
        std::vector<std::string> output;
        bool syntax_valid = parseRecurse(input, output);
        REQUIRE(syntax_valid);
        REQUIRE(output.size() == 19);
        REQUIRE(output[0] == "X");
        REQUIRE(output[1] == "T");
        REQUIRE(output[2] == "a");
        REQUIRE(output[3] == "T");
        REQUIRE(output[4] == "a");
        REQUIRE(output[5] == "T");
        REQUIRE(output[6] == "a");
        REQUIRE(output[7] == "T");
        REQUIRE(output[8] == "R");
        REQUIRE(output[9] == "b");
        REQUIRE(output[10] == "R");
        REQUIRE(output[11] == "b");
        REQUIRE(output[12] == "R");
        REQUIRE(output[13] == "b");
        REQUIRE(output[14] == "R");
        REQUIRE(output[15] == "c");
        REQUIRE(output[16] == "c");
        REQUIRE(output[17] == "c");
        REQUIRE(output[18] == "$");
    }

    SUBCASE("bbb$") {
        auto input = makeStringStack("bbb$");
        std::vector<std::string> output;
        bool syntax_valid = parseRecurse(input, output);
        REQUIRE(syntax_valid);
        REQUIRE(output.size() == 10);
        REQUIRE(output[0] == "X");
        REQUIRE(output[1] == "T");
        REQUIRE(output[2] == "R");
        REQUIRE(output[3] == "b");
        REQUIRE(output[4] == "R");
        REQUIRE(output[5] == "b");
        REQUIRE(output[6] == "R");
        REQUIRE(output[7] == "b");
        REQUIRE(output[8] == "R");
        REQUIRE(output[9] == "$");
    }

    SUBCASE("ab$") {
        auto input = makeStringStack("ab$");
        std::vector<std::string> output;
        bool syntax_valid = parseRecurse(input, output);
        REQUIRE(!syntax_valid);
        REQUIRE(output.size() == 7);
        REQUIRE(output[0] == "X");
        REQUIRE(output[1] == "T");
        REQUIRE(output[2] == "a");
        REQUIRE(output[3] == "T");
        REQUIRE(output[4] == "R");
        REQUIRE(output[5] == "b");
        REQUIRE(output[6] == "R");
    }

    SUBCASE("abcc$") {
        auto input = makeStringStack("abcc$");
        std::vector<std::string> output;
        bool syntax_valid = parseRecurse(input, output);
        REQUIRE(!syntax_valid);
        REQUIRE(output.size() == 8);
        REQUIRE(output[0] == "X");
        REQUIRE(output[1] == "T");
        REQUIRE(output[2] == "a");
        REQUIRE(output[3] == "T");
        REQUIRE(output[4] == "R");
        REQUIRE(output[5] == "b");
        REQUIRE(output[6] == "R");
        REQUIRE(output[7] == "c");
    }

    SUBCASE("aa$") {
        auto input = makeStringStack("aa$");
        std::vector<std::string> output;
        bool syntax_valid = parseRecurse(input, output);
        REQUIRE(!syntax_valid);
        REQUIRE(output.size() == 7);
        REQUIRE(output[0] == "X");
        REQUIRE(output[1] == "T");
        REQUIRE(output[2] == "a");
        REQUIRE(output[3] == "T");
        REQUIRE(output[4] == "a");
        REQUIRE(output[5] == "T");
        REQUIRE(output[6] == "R");
    }

    SUBCASE("$") {
        auto input = makeStringStack("$");
        std::vector<std::string> output;
        bool syntax_valid = parseRecurse(input, output);
        REQUIRE(syntax_valid);
        REQUIRE(output.size() == 4);
        REQUIRE(output[0] == "X");
        REQUIRE(output[1] == "T");
        REQUIRE(output[2] == "R");
        REQUIRE(output[3] == "$");
    }
}

// MODIFICATIONS BELOW THIS LINE ONLY


bool parseRecurse(std::stack<std::string> &input, std::vector<std::string>& output) {
    return false;
}
