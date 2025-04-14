
#include <iostream>
#include "tokens.h"

extern int yylex(void);
extern char *yytext;

int
main() {
	int l, ret = 0;

	for (;;) {
        l = yylex();
        if (l == TokEOF) {
            printf("EOF\n");
            break;
        } else if (l == TokIdentifier) {
            printf("IDENTIFIER(%s)\n", yytext);
        } else if (l == TokInteger) {
            printf("INTEGER(%s)\n", yytext);
        } else if (l == TokError) {
            printf("ERROR(%s)\n", yytext);
        }
	}
	return ret;
}
