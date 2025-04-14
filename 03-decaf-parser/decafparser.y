%{
//
// Parser Implemented by Zac Schepis over 4/10-4/14
//
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include "Expression.h"
#include <stdexcept>
extern int yylex(void);
extern void yyerror(const char *s);
#define YYDEBUG 1
%}

%define parse.trace


%union {
    int intval;
    bool *boolval;
    std::string *stringval;
    char charVal;
}

%token <stringval> STR T_RCURL T_LCURL T_RPAREN T_LPAREN T_PLUS T_MINUS T_TIMES T_DIVIDE T_MODULUS T_GT T_LT T_GTE T_LTE T_EQ T_NEQ T_PEQ

%token T_AND
%token T_OR


%token <stringval> ID
%token <intval> T_NUMBER


%token T_BOOL T_BREAK T_CALLOUT T_CLASS T_CONTINUE T_ELSE T_FALSE T_FOR T_IF T_INT T_RETURN T_TRUE T_VOID T_COMMENT T_ASSIGN T_MEQ T_PE 

%token T_ALPHA_NUM T_ALPHA T_DIGIT T_HEX_DIGIT T_INT_LIT T_DEC_LIT T_HEX_LIT T_BOOL_LIT T_CHAR_LIT <charVal> T_CHAR T_STRING_LIT T_APOS T_QUOTE T_NOT T_COMMA T_PROGRAM T_HEXA T_OBRA T_COBRA T_EM T_SEMI


%%
Program:   T_CLASS T_PROGRAM T_LCURL typed_fields Methods T_RCURL
            | T_CLASS T_PROGRAM T_LCURL field_decl Methods T_RCURL
            ;

MethodDecl: Type ID T_LPAREN TypeIDList T_RPAREN block 
            | Type ID T_LPAREN T_RPAREN block
            | T_VOID ID T_LPAREN TypeIDList T_RPAREN block
            | T_VOID ID T_LPAREN T_RPAREN block;
typed_fields: /* empty */ | Type field_decl_list T_SEMI;
field_decl_list : /* empty */ | field_decl T_COMMA field_decl_list | field_decl;

field_decl: ID | ID T_OBRA T_NUMBER T_COBRA;

IDList:     ID | IDList T_COMMA ID ;
TypeIDList: TypeID | TypeIDList T_COMMA TypeID;

Methods:    MethodDecl 
            | MethodDecl Methods ;

retType:     Type | T_VOID;
Exp:         ID
             | MethodCall
             | Literal
             | Exp BinOp Exp
             | T_MINUS Exp
             | T_EM Exp
             | T_LPAREN Exp T_RPAREN
             | T_NUMBER
             ;
stmts:       /* empty */ 
                | stmts Statement;

vardecl:     Type Location
             | Type Location T_ASSIGN Exp
             | Type Location T_COMMA IDList
             ;
             
vardecls:        /* empty */
                | vardecls vardecl T_SEMI;
block:       T_LCURL vardecls stmts T_RCURL;
Statement:   T_SEMI 
             | ID AssignOp Exp T_SEMI
             | MethodCall T_SEMI
             | T_IF T_LPAREN Exp T_RPAREN block T_ELSE block
             | T_IF T_LPAREN Exp T_RPAREN block
             | T_FOR ID T_ASSIGN Exp T_COMMA Exp block
             | T_RETURN Exp T_SEMI
             | T_BREAK T_SEMI
             | T_CONTINUE T_SEMI
             | block
             ;
Exprs:       Exp
             | Exp T_COMMA Exprs;

MethodCall:  ID T_LPAREN Exprs T_RPAREN
             | ID T_LPAREN T_RPAREN
             | T_CALLOUT T_LPAREN STR T_COMMA CalloutArgs T_RPAREN
             | T_CALLOUT T_LPAREN STR T_RPAREN
             ;

CalloutArgs: CalloutArg
             | CalloutArgs T_COMMA CalloutArg
             ;
CalloutArg:   Exp | StringLiteral;
Location:    ID
             | ID T_OBRA Exp T_COBRA
             ;

TypeID:      Type ID;

AssignOp:    T_ASSIGN
             | T_MEQ
             | T_PEQ
             ;
BinOp:       ArithOp | RelOp | EqOp | CondOp;
ArithOp:     T_PLUS
             | T_MINUS
             | T_TIMES
             | T_DIVIDE
             | T_MODULUS
             ;

RelOp:       T_LT
             | T_GT
             | T_GTE
             | T_LTE
             ;
EqOp:        T_EQ
             | T_NEQ
             ;
CondOp:      T_AND
             | T_OR
             ;

chars:       /* empty string */
             | char_val chars
             ;
char_val:    T_CHAR;
Literal:     IntLiteral | CharLiteral | BoolLiteral;
StringLiteral: T_QUOTE chars T_QUOTE;
CharLiteral: T_APOS char_val T_APOS;

IntLiteral: T_NUMBER;
BoolLiteral: T_TRUE
             | T_FALSE
             ;
Type:        T_INT | T_BOOL;

%%
void yyerror(const char *s) {
    std::cerr << s << std::endl;
    throw std::runtime_error(s);
}
extern int lex_set_buffer(const char *);

int
main(int argc, char *argv[]) {
    yydebug = 1;

    if (argc < 2) {
        std::cerr << "missing argument" << std::endl;
        return 1;
    } else if (argc > 2) {
        std::cerr << "too many arguments" << std::endl;
    }

    if (lex_set_buffer(argv[1]))
        return 1;

    return yyparse();
}

