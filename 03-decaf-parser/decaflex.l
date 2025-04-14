
%{
//
// Lexer Implemented by Zac Schepis over 4/10-4/14
//
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "Expression.h"
#include "decafparser.h"
int lineno = 0;

char parse_char(const char *text) {
    if (text[1] != '\\') {
        return text[1];  // Normal ASCII character
    } else {
        switch (text[2]) {
            case 'n': return '\n';
            case 't': return '\t';
            case '\\': return '\\';
            case '\'': return '\'';
            case '\"': return '\"';
            default:
                exit(1);
        }
    }
}

%}

%option noyywrap
%option nounistd
%option never-interactive

%%


"class"                         return T_CLASS;
"Program"                       return T_PROGRAM;
\{                             return T_LCURL;
\}                             return T_RCURL;
\(                             return T_LPAREN;
\)                             return T_RPAREN;
!                             return T_EM;
"["                             return T_OBRA;
\]                              return T_COBRA;
\+                             return T_PLUS;
\-                             return T_MINUS;
\*                             return T_TIMES;
\/                             return T_DIVIDE;
"%"                             return T_MODULUS;
\>                             return T_GT;
\<                             return T_LT;
\>\=                            return T_GTE;
\<\=                            return T_LTE;
\=\=                            return T_EQ;
\!\=                            return T_NEQ;
\&\&                            return T_AND;
\|\|                            return T_OR;
","                             return T_COMMA;
"boolean"                       return T_BOOL;
"break"                         return T_BREAK;
"callout"                       return T_CALLOUT;
"class"                         return T_CLASS;
"continue"                      return T_CONTINUE;
"else"                          return T_ELSE;
"for"                           return T_FOR;
"if"                            return T_IF;
"int"                           return T_INT;
"return"                        return T_RETURN;
"true"                          return T_TRUE;
"void"                          return T_VOID;
"="                             return T_ASSIGN;
"-="                            return T_MEQ;
"+="                            return T_PEQ;
\"                              return T_QUOTE;
\'                              return T_APOS;
"!"                             return T_NOT;
";"                             return T_SEMI;
[ \t\n]+               { /* empty */ }
\n          {lineno++;}
'([^\\'\n]|\\[nt'"\\])'           {
                                                yylval.charVal = parse_char(yytext);
                                                return T_CHAR;
                                            }

0[xX][0-9a-fA-F]+           {yylval.intval = strtol(yytext, NULL, 16); return T_NUMBER;}
[1-9][0-9]*                  {yylval.intval = strtol(yytext, NULL, 10); return T_NUMBER;}
0                           {yylval.intval = 0; return T_NUMBER;}

[a-zA-Z_][A-Za-z0-9_]*      {   // alpha_num
                                yylval.stringval = new std::string(yytext);
                                return ID;
                            }
" "
.
\/\/(.*)"\n"
<<EOF>>                 return 0;

%%

int lex_set_buffer(const char *fname) {
    YY_BUFFER_STATE bs;
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        std::cerr << "can't open: " << fname << std::endl;
        return 1;
    }

    bs = yy_create_buffer(f, YY_BUF_SIZE);
    yy_switch_to_buffer(bs);
    return 0;
}
