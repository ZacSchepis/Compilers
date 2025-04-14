
%{
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Expression.h"
extern int yylex(void);
extern void yyerror(const char *s);
#define YYDEBUG 1

std::map<std::string, Function *> funcTable;
%}

%define parse.trace

%union {
  int intval;
  std::string *stringval;
  ExprTypeId *expridtype;
  std::list<ExprTypeId*> *typeidlist;
  std::list<Expression*> *exprlist;
  Expression *expr;
}

%token <stringval> T_IDENTIFIER
%token T_LPAREN
%token T_RPAREN
%token T_COMMA
%token T_INT
%token T_BOOL
%token <intval> T_NUMBER
%token T_IN
%token T_LET
%token T_IF
%token T_THEN
%token T_ELSE

%left T_PLUS
%left T_EQUAL
%right T_IN
%right T_ELSE

%type <expridtype> TypeID
%type <typeidlist> TypeIDs
%type <expr> Exp
%type <exprlist> Exps

%%

Program:    Funs
            ;

Funs:       Fun
            | Fun Funs
            ;

Fun:        TypeID T_LPAREN TypeIDs T_RPAREN T_EQUAL Exp
                {
                    if (funcTable.count($1->id) > 0)
                        throw MultiplyDefinedException();
                    Function *f = new Function();
                    f->name = *$1;
                    f->params = $3;
                    f->expr = $6;
                    funcTable[$1->id] = f;
                }
            ;

TypeID:     T_INT T_IDENTIFIER
                {
                    $$ = new ExprTypeId;
                    $$->type = ET_INTEGER;
                    $$->id = *$2;
                }
            | T_BOOL T_IDENTIFIER
                {
                    $$ = new ExprTypeId;
                    $$->type = ET_BOOL;
                    $$->id = *$2;
                }
            ;

TypeIDs:    TypeID
                {
                    /* This will be the rightmost parameter */
                    $$ = new std::list<ExprTypeId *>;
                    $$->push_back($1);
                }
            | TypeID T_COMMA TypeIDs
                {
                    /* Use the list created by the rightmost parameter above */
                    $$ = $3;
                    for (const auto x: *$$) {
                        if (x->id == $1->id)
                            throw MultiplyDefinedException();
                    }
                    $$->push_front($1);
                }
            ;

Exp         : T_NUMBER
                {
                    // Return an Expression *
                    $$ = new ExpressionInt($1);
                }
            | T_IDENTIFIER
                {
                    $$ = new ExpressionId($1);
                }
            | Exp T_PLUS Exp
                {
                    $$ = new ExpressionPlus($1, $3);
                }
            | Exp T_EQUAL Exp
                {
                    $$ = new ExpressionEqual($1, $3);
                }
            | T_IF Exp T_THEN Exp T_ELSE Exp
                {
                    $$ = new ExpressionIf($2, $4, $6);
                }
            | T_IDENTIFIER T_LPAREN Exps T_RPAREN
                {
                    $$ = new ExpressionFuncall($1, $3);
                }
            | T_LET T_IDENTIFIER T_EQUAL Exp T_IN Exp
                {
                    $$ = new ExpressionLet($2, $4, $6);
                }
            ;

Exps        : Exp
                {
                    $$ = new std::list<Expression*>;
                    $$->push_back($1);
                }
            | Exp T_COMMA Exps
                {
                    $$ = $3;
                    $$->push_front($1);
                }
            ;

%%

void
reset_parser() {
    funcTable.clear();
}
