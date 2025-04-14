
%{
#include <string>
#include <vector>
#include <list>
#include "Expression.h"
#include "parser.h"
%}

%option noyywrap
%option nounistd
%option never-interactive

%%

"if"                    return T_IF;
"("                     return T_LPAREN;
")"                     return T_RPAREN;
"="                     return T_EQUAL;
","                     return T_COMMA;
"int"                   return T_INT;
"bool"                  return T_BOOL;
"+"                     return T_PLUS;
"in"                    return T_IN;
"let"                   return T_LET;
"then"                  return T_THEN;
"else"                  return T_ELSE;

[ \t]+                  { /* ignore */ }
\n                      { yylineno++; }

[0-9]+                  {
                            yylval.intval = atoi(yytext);
                            return T_NUMBER;
                        }

[a-zA-Z_][A-Za-z0-9_]*  {
                            yylval.stringval = new std::string(yytext);
                            return T_IDENTIFIER;
                        }

<<EOF>>                 return 0;

%%

void lex_set_string(const char *buf) {
    yy_scan_string(buf);
    yy_init = 0;
}
