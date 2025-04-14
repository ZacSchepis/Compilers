#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ranges>
#include "helpers.h"

bool parseTable(std::stack<std::string> &input, const Grammar &g, const ParseTable &table, std::vector<std::string> &output);

TEST_CASE("table driven") {
    Grammar g = {
            {NonTerminal("X"), {NonTerminal("T"), Terminal("$")}},
            {NonTerminal("T"), {Terminal("a"),    NonTerminal("T"), Terminal("c")}},
            {NonTerminal("T"), {NonTerminal("R")}},
            {NonTerminal("R"), {}},
            {NonTerminal("R"), {Terminal("b"),    NonTerminal("R")}}
    };

    auto t = ParseTable();
    t[NonTerminal("X")] = std::map<Terminal, ProductionRule>();
    t[NonTerminal("T")] = std::map<Terminal, ProductionRule>();
    t[NonTerminal("R")] = std::map<Terminal, ProductionRule>();

    t[NonTerminal("X")].insert(std::pair{Terminal("a"), g[0]});
    t[NonTerminal("X")].insert(std::pair{Terminal("b"), g[0]});
    t[NonTerminal("X")].insert(std::pair{Terminal("$"), g[0]});

    t[NonTerminal("T")].insert(std::pair{Terminal("a"), g[1]});
    t[NonTerminal("T")].insert(std::pair{Terminal("b"), g[2]});
    t[NonTerminal("T")].insert(std::pair{Terminal("c"), g[2]});
    t[NonTerminal("T")].insert(std::pair{Terminal("$"), g[2]});

    t[NonTerminal("R")].insert(std::pair{Terminal("b"), g[4]});
    t[NonTerminal("R")].insert(std::pair{Terminal("c"), g[3]});
    t[NonTerminal("R")].insert(std::pair{Terminal("$"), g[3]});

    SUBCASE("valid string") {
        auto input = makeStringStack("aaabbbccc$");
        std::vector<std::string> output;
        bool syntax_valid = parseTable(input, g, t, output);
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

    SUBCASE("all b's") {
        auto input = makeStringStack("bbb$");
        std::vector<std::string> output;
        bool syntax_valid = parseTable(input, g, t, output);
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
        bool syntax_valid = parseTable(input, g, t, output);
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
        bool syntax_valid = parseTable(input, g, t, output);
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
        bool syntax_valid = parseTable(input, g, t, output);
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
        bool syntax_valid = parseTable(input, g, t, output);
        REQUIRE(syntax_valid);
        REQUIRE(output.size() == 4);
        REQUIRE(output[0] == "X");
        REQUIRE(output[1] == "T");
        REQUIRE(output[2] == "R");
        REQUIRE(output[3] == "$");
    }
}

// MODIFICATIONS BELOW THIS LINE ONLY

/* Name: Zac Schepis
 * HW3 - LL1
 * Date:3/3/25
 * All work below is my own
 * */
bool
parseTable(std::stack<std::string> &input, const Grammar &g,
           const ParseTable &table, std::vector<std::string> &output) {
    std::stack<std::string> parseStack;
    parseStack.push("X");
    while(!parseStack.empty() ) {
        std::string top = parseStack.top();
        std::string lookahead = input.top();
        if (top == lookahead) {
            parseStack.pop();
            input.pop();
            output.push_back(top);
        } else if (table.contains(NonTerminal(top))) {
            auto &rules = table.at(NonTerminal(top));
            if (rules.contains(Terminal(lookahead))) {
                parseStack.pop();
                auto &rule = rules.at(Terminal(lookahead));
                output.push_back(top);
                for (auto it = rule.second.rbegin(); it != rule.second.rend(); ++it) {
                    if (!it->getId().empty()) {
                        parseStack.push(it->getId());
                    }
                }
            } else { return false; }
        } else { return false; }
    }
    return input.empty();
}
/* Name: Zac Schepis
 * HW3 - LL1
 * Date:3/3/25
 * All work within this small section below that line is my own
 * */
