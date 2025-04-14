#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include "StringStack.h"
#include "dfa.h"

TEST_CASE("Empty string: EOF", "[short]") {
    StateMachine sm;
    StringStack ss("");
    std::string lexeme;
    TokenType t = sm.next(ss, lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Lone Identifier", "[short]") {
    StateMachine sm;
    StringStack ss("thisisatest");
    std::string lexeme;
    TokenType t = sm.next(ss, lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "thisisatest");
    t = sm.next(ss, lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Lone Integer", "[short]") {
    StateMachine sm;
    StringStack ss("12345");
    std::string lexeme;
    TokenType t = sm.next(ss, lexeme);
    REQUIRE(t == TokInteger);
    REQUIRE(lexeme == "12345");
    t = sm.next(ss, lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Lone WhiteSpace", "[short]") {
    StateMachine sm;
    StringStack ss(" \t\n\r");
    std::string lexeme;
    TokenType t = sm.next(ss, lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " \t\n\r");
    t = sm.next(ss, lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Lone Error", "[short]") {
    StateMachine sm;
    StringStack ss("@");
    std::string lexeme;
    TokenType t = sm.next(ss, lexeme);
    REQUIRE(t == TokError);
    REQUIRE(lexeme == "@");
    t = sm.next(ss, lexeme);
    REQUIRE(t == TokEOF);

}

TEST_CASE("Mixing it up", "[short]") {
    StateMachine sm;
    StringStack ss("hello 1234\th\n_4");
    std::string lexeme;

    TokenType t = sm.next(ss, lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "hello");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokInteger);
    REQUIRE(lexeme == "1234");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == "\t");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "h");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == "\n");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "_4");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Mixed with error tokens", "[short]") {
    StateMachine sm;
    StringStack ss("O_0 @# % 12 __foom__");
    std::string lexeme;

    TokenType t = sm.next(ss, lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "O_0");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokError);
    REQUIRE(lexeme == "@");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokError);
    REQUIRE(lexeme == "#");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokError);
    REQUIRE(lexeme == "%");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokInteger);
    REQUIRE(lexeme == "12");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "__foom__");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Mixed", "[short]") {
    StateMachine sm;
    StringStack ss("123abc");
    std::string lexeme;

    TokenType t = sm.next(ss, lexeme);
    REQUIRE(t == TokInteger);
    REQUIRE(lexeme == "123");

    t = sm.next(ss, lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "abc");
}
