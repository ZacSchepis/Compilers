#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include "tokens.h"
#include "StringStack.h"

extern int yylex(void);
extern char *yytext;

extern void lex_set_string(const char *buf);

TokenType lexer(std::string& lexeme) {
    int r = yylex();
    lexeme = (yytext == NULL) ? "(null)" : yytext;
    return static_cast<TokenType>(r);
}

TEST_CASE("Empty string: EOF", "[short]") {
    TokenType t;
    std::string lexeme;
    lex_set_string("");
    t = lexer(lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Lone Identifier", "[short]") {
    TokenType t;
    lex_set_string("thisisatest");
    std::string lexeme;
    t = lexer(lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "thisisatest");
    t = lexer(lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Lone Integer", "[short]") {
    TokenType t;
    std::string lexeme;
    lex_set_string("12345");
    t = lexer(lexeme);
    REQUIRE(t == TokInteger);
    REQUIRE(lexeme == "12345");
    t = lexer(lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Lone WhiteSpace", "[short]") {
    TokenType t;
    std::string lexeme;
    lex_set_string(" \t\n\r");
    t = lexer(lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " \t\n\r");
    t = lexer(lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Lone Error", "[short]") {
    TokenType t;
    std::string lexeme;
    lex_set_string("@");
    t = lexer(lexeme);
    REQUIRE(t == TokError);
    REQUIRE(lexeme == "@");
    t = lexer(lexeme);
    REQUIRE(t == TokEOF);

}

TEST_CASE("Mixing it up", "[short]") {
    TokenType t;
    std::string lexeme;
    lex_set_string("hello 1234\th\n_4");

    t = lexer(lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "hello");

    t = lexer(lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = lexer(lexeme);
    REQUIRE(t == TokInteger);
    REQUIRE(lexeme == "1234");

    t = lexer(lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == "\t");

    t = lexer(lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "h");

    t = lexer(lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == "\n");

    t = lexer(lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "_4");

    t = lexer(lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Mixed with error tokens", "[short]") {
    TokenType t;
    std::string lexeme;
    lex_set_string("O_0 @# % 12 __foom__");

    t = lexer(lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "O_0");

    t = lexer(lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = lexer(lexeme);
    REQUIRE(t == TokError);
    REQUIRE(lexeme == "@");

    t = lexer(lexeme);
    REQUIRE(t == TokError);
    REQUIRE(lexeme == "#");

    t = lexer(lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = lexer(lexeme);
    REQUIRE(t == TokError);
    REQUIRE(lexeme == "%");

    t = lexer(lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = lexer(lexeme);
    REQUIRE(t == TokInteger);
    REQUIRE(lexeme == "12");

    t = lexer(lexeme);
    REQUIRE(t == TokWhitespace);
    REQUIRE(lexeme == " ");

    t = lexer(lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "__foom__");

    t = lexer(lexeme);
    REQUIRE(t == TokEOF);
}

TEST_CASE("Mixed", "[short]") {
    TokenType t;
    std::string lexeme;
    lex_set_string("123abc");

    t = lexer(lexeme);
    REQUIRE(t == TokInteger);
    REQUIRE(lexeme == "123");

    t = lexer(lexeme);
    REQUIRE(t == TokIdentifier);
    REQUIRE(lexeme == "abc");
}
