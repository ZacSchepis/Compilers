
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <ranges>
#include <cassert>
#include "helpers.h"

// You need to write a parser that accepts the following language:
// E -> E E +
// E -> E E *
// E -> num
//
// Note: You will need to eliminate left recursion, do left factorization,
// and compute the FIRST and FOLLOW sets. Use those results to generate your
// parser. You may either produce a recursive descent parser OR a table
// driven parser. You can reuse your table driven parser from the previous
// exercise (with a different table, of course).

// This is the function you need to write. The definition is at the bottom
// of this file.
bool parser(std::stack<Symbol> &input);

// Translate a test string into a stack of Terminals for parsing.
std::stack<Symbol> makeSymbolStack(const std::string &s);

TEST_CASE("postfixparser") {
    SUBCASE("n$") {
        auto input = makeSymbolStack("n$");
        auto valid_syntax = parser(input);
        REQUIRE(valid_syntax);
    }

    SUBCASE("nn+$") {
        auto input = makeSymbolStack("nn+$");
        auto valid_syntax = parser(input);
        REQUIRE(valid_syntax);
    }

    SUBCASE("nn*$") {
        auto input = makeSymbolStack("nn*$");
        auto valid_syntax = parser(input);
        REQUIRE(valid_syntax);
    }

    SUBCASE("nn+nn**$") {
        auto input = makeSymbolStack("nn+nn**$");
        auto valid_syntax = parser(input);
        REQUIRE(valid_syntax);
    }

    SUBCASE("nn+nn*n**$") {
        auto input = makeSymbolStack("nn+nn**$");
        auto valid_syntax = parser(input);
        REQUIRE(valid_syntax);
    }

        // From here down are invalid syntax

    SUBCASE("$") {
        auto input = makeSymbolStack("$");
        auto valid_syntax = parser(input);
        REQUIRE(!valid_syntax);
    }

    SUBCASE("nn$") {
        auto input = makeSymbolStack("nn$");
        auto valid_syntax = parser(input);
        REQUIRE(!valid_syntax);
    }

    SUBCASE("n+$") {
        auto input = makeSymbolStack("n+$");
        auto valid_syntax = parser(input);
        REQUIRE(!valid_syntax);
    }

    SUBCASE("n*$") {
        auto input = makeSymbolStack("n*$");
        auto valid_syntax = parser(input);
        REQUIRE(!valid_syntax);
    }

    SUBCASE("nnn+nn*n**$") {
        auto input = makeSymbolStack("nnn+nn**$");
        auto valid_syntax = parser(input);
        REQUIRE(!valid_syntax);
    }
}

// Translate a test string into a stack of Terminals for parsing.
std::stack<Symbol> makeSymbolStack(const std::string &s) {
    std::stack<Symbol> r;
    for (const auto c: s | std::views::reverse) {
        if (c == 'n')
            r.push(Terminal("num", "1234"));
        else if (c == '$')
            r.push(Terminal("$", "EOF"));
        else if (c == '+')
            r.push(Terminal("+", "+"));
        else if (c == '*')
            r.push(Terminal("*", "*"));
        else {
            // Bad character in the string.
            assert(false);
        }
    }
    return r;
}

// MODIFICATIONS BELOW THIS LINE ONLY

/* Name: Zac Schepis
 * HW3 - LL1
 * Date:3/3/25
 * All work below is my own. Except the Table code, I referenced the code
 * you used to create the tableDriven test as a guide to structure my grammar
 * for this language
 * */
bool
parseTable(std::stack<Symbol> &input, const Grammar &g,
           const ParseTable &table, std::vector<std::string> &output) {
    std::stack<Symbol> parseStack;
    parseStack.push(NonTerminal("A"));
    while(!parseStack.empty()) {
        Symbol top = parseStack.top();
        Symbol lookahead = input.top();
        if (top == lookahead) {
            parseStack.pop();
            input.pop();
        } else if (!top.isTerminal() && table.contains(NonTerminal(top))) {
            auto &rules = table.at(NonTerminal(top));
            if (lookahead.isTerminal() && rules.contains(Terminal(lookahead))) {
                parseStack.pop();
                auto &rule = rules.at(Terminal(lookahead));
                for (auto it = rule.second.rbegin(); it != rule.second.rend(); ++it) {
                    if (!it->getId().empty()) {
                        parseStack.push(*it);
                    }
                }
            } else { return false; }
        } else { return false; }
    }
    return input.empty();
}

bool parser(std::stack<Symbol> &input) {
    Grammar g = {
            {NonTerminal("A"), {NonTerminal("E"), Terminal("$", "EOF")}},
            {NonTerminal("E"), {Terminal("num", "1234"), NonTerminal("F")}},
            {NonTerminal("F"), {NonTerminal("E"), NonTerminal("G")}},
            {NonTerminal("F"), {}},
            {NonTerminal("G"), {Terminal("+"), NonTerminal("F")}},
            {NonTerminal("G"), {Terminal("*"), NonTerminal("F")}},

    };
    auto t = ParseTable();
    t[NonTerminal("A")] = std::map<Terminal, ProductionRule>();
    t[NonTerminal("E")] = std::map<Terminal, ProductionRule>();
    t[NonTerminal("F")] = std::map<Terminal, ProductionRule>();
    t[NonTerminal("G")] = std::map<Terminal, ProductionRule>();

    t[NonTerminal("A")].insert(std::pair{Terminal("num", "1234"), g[0]});
    t[NonTerminal("E")].insert(std::pair{Terminal("num", "1234"), g[1]});
    t[NonTerminal("F")].insert(std::pair{Terminal("num", "1234"), g[2]});
    t[NonTerminal("F")].insert(std::pair{Terminal("+"), g[3]});
    t[NonTerminal("F")].insert(std::pair{Terminal("*"), g[3]});
    t[NonTerminal("F")].insert(std::pair{Terminal("$", "EOF"), g[3]});

    t[NonTerminal("G")].insert(std::pair{Terminal("+"), g[4]});
    t[NonTerminal("G")].insert(std::pair{Terminal("*"), g[5]});

    std::vector<std::string> output;
    return parseTable(input, g, t, output);
}
/* Name: Zac Schepis
 * HW3 - LL1
 * Date:3/3/25
 * All work within this small section below that line is my own
 * */