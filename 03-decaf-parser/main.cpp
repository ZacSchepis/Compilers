#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include <iostream>
#include <Expression.h>
#include <stdexcept>
#include "parser.h"

[[noreturn]] void yyerror(const char *s) {
    std::cerr << s << std::endl;
    throw std::runtime_error(s);
}

extern void lex_set_string(const char *buf);
extern std::map<std::string, Function *> funcTable;
extern void reset_parser();

TEST_CASE("multiply defined functions") {
    reset_parser();
    lex_set_string(
            "int main(int x) = x + 2\n"
            "int main(int y) = y + 4\n"
    );
    REQUIRE_THROWS_AS(yyparse(), MultiplyDefinedException);
}

TEST_CASE("multiply defined variables") {
    reset_parser();
    lex_set_string(
            "int main(int x, bool x) = x + 2\n"
    );
    REQUIRE_THROWS_AS(yyparse(), MultiplyDefinedException);
}

TEST_CASE("main returns 1") {
    reset_parser();
    lex_set_string(
            "int main(int x) = 1\n"
    );
    yyparse();

    // Construct a "main(1)" expression, then evaluate it.
    SymbolTable sTable;
    std::string main = "main";
    auto args = std::list<Expression *>();
    auto num = new ExpressionInt(1);
    args.push_front(num);
    auto mainExpression = new ExpressionFuncall(&main, &args);
    Value v = mainExpression->eval(funcTable, sTable);

    REQUIRE(v.type == ET_INTEGER);
    REQUIRE(v.intval == 1);
}

TEST_CASE("funcall plus") {
    reset_parser();
    lex_set_string("int main(int x) = x + 2");
    yyparse();
    SymbolTable sTable;

    // Construct a "main(1)" expression, then evaluate it.
    std::string main = "main";
    auto args = std::list<Expression *>();
    auto num = ExpressionInt(1);
    args.push_front(&num);
    auto mainExpression = ExpressionFuncall(&main, &args);
    Value v = mainExpression.eval(funcTable, sTable);

    REQUIRE(v.type == ET_INTEGER);
    REQUIRE(v.intval == 3);

    num.set(4);
    v = mainExpression.eval(funcTable, sTable);
    REQUIRE(v.type == ET_INTEGER);
    REQUIRE(v.intval == 6);
}

TEST_CASE("plus") {
    auto eone = ExpressionInt(1);
    auto etwo = ExpressionInt(2);
    reset_parser();
    SymbolTable sTable;

    SUBCASE("normal addition") {
        auto e = ExpressionPlus(&eone, &etwo);
        Value v = e.eval(funcTable, sTable);
        REQUIRE(v.type == ET_INTEGER);
        REQUIRE(v.intval == 3);
    }
    SUBCASE("1 + bool") {
        auto rhs = ExpressionBool(true);
        auto e = ExpressionPlus(&eone, &rhs);
        REQUIRE_THROWS_AS((void)e.eval(funcTable, sTable), TypeMismatchException);
        rhs = ExpressionBool(false);
        e = ExpressionPlus(&eone, &rhs);
        REQUIRE_THROWS_AS((void)e.eval(funcTable, sTable), TypeMismatchException);
    }
    SUBCASE("bool + two") {
        auto lhs = ExpressionBool(true);
        auto e = ExpressionPlus(&lhs, &etwo);
        REQUIRE_THROWS_AS((void)e.eval(funcTable, sTable), TypeMismatchException);
        lhs = ExpressionBool(false);
        e = ExpressionPlus(&lhs, &etwo);
        REQUIRE_THROWS_AS((void)e.eval(funcTable, sTable), TypeMismatchException);
    }
}

TEST_CASE("equal") {
    auto eone = ExpressionInt(1);
    auto etwo = ExpressionInt(2);
    reset_parser();
    SymbolTable sTable;

    SUBCASE("int normal equal, true") {
        auto e = ExpressionEqual(&eone, &eone);
        Value v = e.eval(funcTable, sTable);
        REQUIRE(v.type == ET_BOOL);
        REQUIRE(v.booleanValue);
    }
    SUBCASE("int normal equal, false") {
        auto e = ExpressionEqual(&eone, &etwo);
        Value v = e.eval(funcTable, sTable);
        REQUIRE(v.type == ET_BOOL);
        REQUIRE(!v.booleanValue);
    }
    SUBCASE("1 = bool") {
        auto rhs = ExpressionBool(true);
        auto e = ExpressionEqual(&eone, &rhs);
        REQUIRE_THROWS_AS((void)e.eval(funcTable, sTable), TypeMismatchException);\
        rhs = ExpressionBool(false);
        e = ExpressionEqual(&eone, &rhs);
        REQUIRE_THROWS_AS((void)e.eval(funcTable, sTable), TypeMismatchException);\
    }
    SUBCASE("bool = 2") {
        auto lhs = ExpressionBool(true);
        auto e = ExpressionEqual(&lhs, &etwo);
        REQUIRE_THROWS_AS((void)e.eval(funcTable, sTable), TypeMismatchException);
        lhs = ExpressionBool(false);
        e = ExpressionEqual(&eone, &lhs);
        REQUIRE_THROWS_AS((void)e.eval(funcTable, sTable), TypeMismatchException);\
    }
    SUBCASE("bool normal") {
        std::vector<bool> vals = {true, false};
        for (auto const rhs: vals) {
            for (auto const lhs : vals) {
                auto elhs = ExpressionBool(lhs);
                auto erhs = ExpressionBool(rhs);
                auto e = ExpressionEqual(&elhs, &erhs);
                auto v = e.eval(funcTable, sTable);
                REQUIRE(v.type == ET_BOOL);
                REQUIRE(v.booleanValue == (lhs == rhs));
            }
        }
    }
}

TEST_CASE("if") {
    auto etrue = ExpressionBool(true);
    auto efalse = ExpressionBool(false);
    auto eEvalTrue = ExpressionInt(42);
    auto eEvalFalse = ExpressionInt(123);
    reset_parser();
    SymbolTable sTable;
    Value vEvalTrue = eEvalTrue.eval(funcTable, sTable);
    Value vEvalFalse = eEvalFalse.eval(funcTable, sTable);

    SUBCASE("if true then x else y") {
        auto eif = ExpressionIf(&etrue, &eEvalTrue, &eEvalFalse);
        Value v = eif.eval(funcTable, sTable);
        REQUIRE(v.type == vEvalTrue.type);
        REQUIRE(v.intval == vEvalTrue.intval);
    }
    SUBCASE("if false then x else y") {
        auto eif = ExpressionIf(&efalse, &eEvalTrue, &eEvalFalse);
        Value v = eif.eval(funcTable, sTable);
        REQUIRE(v.type == vEvalFalse.type);
        REQUIRE(v.intval == vEvalFalse.intval);
    }
}

TEST_CASE("id") {
    SymbolTable sTable;
    VarValue a = {{"a", ET_BOOL}, {.type = ET_BOOL, .booleanValue = true}};
    VarValue b = {{"b", ET_BOOL}, {.type = ET_BOOL, .booleanValue = false}};
    VarValue c = {{"c", ET_INTEGER}, {.type = ET_INTEGER, .intval = 1}};
    VarValue d = {{"d", ET_INTEGER}, {.type = ET_INTEGER, .intval = 2}};
    VarValue e = {{"e", ET_INTEGER}, {.type = ET_INTEGER, .intval = 3}};
    VarValue aprime = {{"a", ET_INTEGER}, {.type = ET_INTEGER, .intval = 5}};
    sTable.push_front(aprime);
    sTable.push_front(e);
    sTable.push_front(d);
    sTable.push_front(c);
    sTable.push_front(b);
    sTable.push_front(a);
    SUBCASE("a") {
        auto s = std::string("a");
        auto id = ExpressionId(&s);
        auto v = id.eval(funcTable, sTable);
        REQUIRE(v.type == ET_BOOL);
        REQUIRE(v.booleanValue == true);
    }
    SUBCASE("b") {
        auto s = std::string("b");
        auto id = ExpressionId(&s);
        auto v = id.eval(funcTable, sTable);
        REQUIRE(v.type == ET_BOOL);
        REQUIRE(v.booleanValue == false);
    }
    SUBCASE("c") {
        auto s = std::string("c");
        auto id = ExpressionId(&s);
        auto v = id.eval(funcTable, sTable);
        REQUIRE(v.type == ET_INTEGER);
        REQUIRE(v.booleanValue == 1);
    }
    SUBCASE("d") {
        auto s = std::string("d");
        auto id = ExpressionId(&s);
        auto v = id.eval(funcTable, sTable);
        REQUIRE(v.type == ET_INTEGER);
        REQUIRE(v.intval == 2);
    }
    SUBCASE("e") {
        auto s = std::string("e");
        auto id = ExpressionId(&s);
        auto v = id.eval(funcTable, sTable);
        REQUIRE(v.type == ET_INTEGER);
        REQUIRE(v.intval == 3);
    }
    SUBCASE("nonexistent") {
        auto s = std::string("nonexistent");
        auto id = ExpressionId(&s);
        REQUIRE_THROWS_AS((void)id.eval(funcTable, sTable), UnknownVariableException);
    }
}

TEST_CASE("let") {
    // Note: if "id" tests are failing, these tests are likely to fail, too.
    SymbolTable sTable;
    VarValue a = {{"a", ET_BOOL}, {.type = ET_BOOL, .booleanValue = true}};
    VarValue b = {{"b", ET_BOOL}, {.type = ET_BOOL, .booleanValue = false}};
    VarValue c = {{"c", ET_INTEGER}, {.type = ET_INTEGER, .intval = 1}};
    VarValue d = {{"d", ET_INTEGER}, {.type = ET_INTEGER, .intval = 2}};
    VarValue e = {{"e", ET_INTEGER}, {.type = ET_INTEGER, .intval = 3}};
    VarValue aprime = {{"a", ET_INTEGER}, {.type = ET_INTEGER, .intval = 5}};
    // Start with a primed symbol table
    sTable.push_front(aprime);
    sTable.push_front(e);
    sTable.push_front(d);
    sTable.push_front(c);
    sTable.push_front(b);
    sTable.push_front(a);
    SUBCASE("new int variable") {
        std::vector<int> vals = {1, 2, 3, 4, 5};
        for (auto const &i: vals) {
            auto varname = std::string("X");
            auto eint = ExpressionInt(i);
            auto eid = ExpressionId(&varname);
            auto exp = ExpressionLet(&varname, &eint, &eid);
            Value v = exp.eval(funcTable, sTable);
            REQUIRE(v.type == ET_INTEGER);
            REQUIRE(v.intval == i);
        }
    }
    SUBCASE("new bool variable") {
        std::vector<bool> vals = {true, false};
        for (auto const &i: vals) {
            auto varname = std::string("X");
            auto eint = ExpressionBool(i);
            auto eid = ExpressionId(&varname);
            auto exp = ExpressionLet(&varname, &eint, &eid);
            Value v = exp.eval(funcTable, sTable);
            REQUIRE(v.type == ET_BOOL);
            REQUIRE(v.booleanValue == i);
        }
    }
    SUBCASE("shadow existing variable") {
        std::vector<int> vals = {1, 2, 3, 4, 5};
        for (auto const &i: vals) {
            auto varname = std::string("c");
            auto eint = ExpressionInt(i);
            auto eid = ExpressionId(&varname);
            auto exp = ExpressionLet(&varname, &eint, &eid);
            auto v = exp.eval(funcTable, sTable);
            REQUIRE(v.type == ET_INTEGER);
            REQUIRE(v.intval == i);
        }
    }
}

TEST_CASE("functional fun :)") {
    SUBCASE("missing an arg") {
        reset_parser();
        lex_set_string("int main(int x, bool y) = x + x");
        yyparse();
        SymbolTable sTable;
    
        // Construct a "main(1)" expression, then evaluate it.
        std::string main = "main";
        auto args = std::list<Expression *>();
        auto num = ExpressionInt(1);
        auto boolVal = ExpressionBool(true);
        args.push_front(&num);
        auto mainExpression = ExpressionFuncall(&main, &args);
        REQUIRE_THROWS_AS((void)mainExpression.eval(funcTable, sTable), NotEnoughArgumentsException);
    }
    SUBCASE("arg mismatches") {
        reset_parser();
        lex_set_string("int main(int x, bool y) = x + x");
        yyparse();
        SymbolTable sTable;
    
        // Construct a "main(1)" expression, then evaluate it.
        std::string main = "main";
        auto args = std::list<Expression *>();
        auto num = ExpressionInt(1);
        auto boolVal = ExpressionBool(true);
        args.push_front(&num);
        args.push_front(&boolVal);
        auto mainExpression = ExpressionFuncall(&main, &args);
        REQUIRE_THROWS_AS((void)mainExpression.eval(funcTable, sTable), TypeMismatchException);
    }
    SUBCASE("nested functions :)") {
        reset_parser();
        lex_set_string(
            "int cantstop(int x) = x + 4\n"
            "int wontstop(int x) = cantstop(x) + 12\n"
            "int main(int y) = wontstop(y)\n"
        );
        yyparse();
        std::string main = "main";
        auto args = std::list<Expression*>();
        auto num = ExpressionInt(10);
        SymbolTable sTable;
        args.push_front(&num);
        auto mainExpression = ExpressionFuncall(&main, &args);
        Value v = mainExpression.eval(funcTable, sTable);
        REQUIRE(v.type == ET_INTEGER);
        REQUIRE(v.intval == 26);


    }
    SUBCASE("nested with conditions") {
        reset_parser();
        lex_set_string(
            "int cantstop(int x) = x + 4\n"
            "int wontstop(int x) = cantstop(x) + 12\n"
            "int main(int y) = wontstop(y)\n"
            "int gotstopped(int x, int y) = if main(x) = y then 1 else 4"
        );
        yyparse();
        std::string gotstopped = "gotstopped";
        SymbolTable sTable;
        auto args = std::list<Expression*>();
        auto num = ExpressionInt(10);
        auto num2 = ExpressionInt(26);
        args.push_front(&num2);
        args.push_front(&num);
        auto gotStoppedExpression = ExpressionFuncall(&gotstopped, &args);
        Value v2 = gotStoppedExpression.eval(funcTable, sTable);
        REQUIRE(v2.type == ET_INTEGER);
        REQUIRE(v2.intval == 1);
    }
    SUBCASE("too many arguments!!") {
        reset_parser();
        lex_set_string(
            "int main(int x, int y, int z) = 4"
        );
        yyparse();
        std::string main = "main";
        SymbolTable sTable;
        auto args = std::list<Expression*>();
        for(int argN = 0; argN < 10; argN++){
            auto argE = ExpressionInt(argN);
            args.push_front(&argE);
        }
        auto mainExpression = ExpressionFuncall(&main, &args);
        REQUIRE_THROWS_AS((void)mainExpression.eval(funcTable, sTable), TooManyArgumentsException);
    }
}