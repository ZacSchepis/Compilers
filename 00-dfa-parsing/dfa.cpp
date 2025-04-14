/*
* Zac Schepis
* CS4488 - Spring2025
* Project 1 - Lexical Anaylsis (DFA+Flex)
* Part 1
* All code provided here is my own code
*/
#include <cctype>
#include "dfa.h"
#include <iostream>


void StateMachine::reset() {
    // called to reset the state machine to its initial state.
    // This allows me to run tests over and over again with the
    // same StateMachine.
}
// Schepis, identifier_valid(int c, int f)
// checks if it is an identifier
int identifier_valid(int c, int f){
    // if the character passed in is the first character, and it is a digit,
    // return false
    if(f==0 && isdigit(c)) return 0;
    // otherwise, check if it is any alphanumeric character
    // or an underscore
    return isalnum(c) || c == '_';
}
// Schepis, rank_character(int c, int f)
// takes a character (int representing) and an f to 
// validate the first character. 
// returns a TokenType representation based
// on the character and its position
TokenType rank_character(int c, int f){
    // end of input
    if(c == -1) return TokEOF;
    // Integers
    if(isdigit(c)) return TokInteger;
    // C++ identifiers
    if(identifier_valid(c, f)) return TokIdentifier;
    // tab,  space, carriage return, newline...
    if(isspace(c)) return TokWhitespace;
    // error
    return TokError;
}
// Schepis, next(StringStack &stack, std::string &lexeme)
// Gets the next token
TokenType StateMachine::next(StringStack &stack, std::string &lexeme) {
    lexeme.clear();
    int c = stack.get();
    TokenType t = rank_character(c, 0);
    TokenType prev = t;
    // Consume all consecutive whitespace characters
    // "in one swell foop" -Beard, 2024
    if (t == TokWhitespace) {
        while (isspace(c)) {
            lexeme += c;
            c = stack.get();
        }
        if(c != -1) stack.unget();
        return TokWhitespace;
    }
    // Consume only one error token at a time
    // and then return the TokenType early
    if(t == TokError) {
        lexeme += c; 
        return t;
    }
    // Otherwise, rank the characters until a different token is found
    while (prev == t) {
        if (t == TokEOF) break;
        lexeme += c;
        c = stack.get();
        prev = t;
        t = rank_character(c, 1);
        if(prev == TokIdentifier && t == TokInteger) t = prev;
    }
    if (prev != t && t != TokEOF) stack.unget();
    return prev;
}
/*
* Zac Schepis
* CS4488 - Spring2025
* Project 1 - Lexical Anaylsis (DFA+Flex)
* Part 1
* All code provided here is my own code
*/