/*
* Zac Schepis
* CS4488 - Spring2025
* Project 1 - Lexical Anaylsis (DFA+Flex)
* Part 1
* I didn't modify this code
* Because it turned out that you didnt use Reset much
* and my implementation did not require any extra variables
*/
#ifndef P1_DFA_H
#define P1_DFA_H

#include "StringStack.h"
#include "tokens.h"

class StateMachine {
private:
    // You might need a variable or two here.

public:
    StateMachine() { reset(); }

    // reset the DFA to its initial state
    void reset();

    // Return the "next" token from the StringStack
    // In addition to setting the type, you should also set "lexeme"
    // to be the text that matched the token.
    TokenType next(StringStack& stack, std::string& lexeme);
};

/*
* Zac Schepis
* CS4488 - Spring2025
* Project 1 - Lexical Anaylsis (DFA+Flex)
* Part 1
* I didn't modify this code
* Because it turned out that you didnt use Reset much
* and my implementation did not require any extra variables
*/
#endif //P1_DFA_H
