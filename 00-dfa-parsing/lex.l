/*
* Zac Schepis
* CS4488 - Spring2025
* Project 1 - Lexical Anaylsis (DFA+Flex)
* Part 2
* All code provided here is my own code
*/
%{
#include "tokens.h"
%}

%option noyywrap
%option nounistd
%option never-interactive

%%/* code should go here */
[a-zA-Z_]+[a-zA-Z0-9_]* {
                            return TokIdentifier;
                        }

[0-9]+                 {
                            return TokInteger;
                        }

[ \t\n\r]+                {
                            return TokWhitespace;
                        }
<<EOF>>                 {
                            return TokEOF;
                        }

.                       {
                            return TokError;
                        }

%%

void lex_set_string(const char *buf) {
    yy_scan_string(buf);
    yy_init = 0;
}
/*
* Zac Schepis
* CS4488 - Spring2025
* Project 1 - Lexical Anaylsis (DFA+Flex)
* Part 2
* All code provided here is my own code
*/