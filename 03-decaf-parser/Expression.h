//
// Created by Jason Wright on 3/17/2024.
//

#ifndef P2_EXPRESSION_H
#define P2_EXPRESSION_H


#include <string>
#include <vector>
#include <utility>
#include <list>
#include <map>

// We have two datatypes: boolean and integer
enum ExprType {
    ET_INTEGER = 100,
    ET_BOOL = 101
};

// This a variable name and its type
class ExprTypeId {
public:
    std::string id;
    ExprType type;
};

// A value is a type and a value (expressed as a union)
struct Value {
    ExprType type;
    union {
        bool booleanValue;
        int intval;
    };
};

// A VarValue is a name and a value (should this be a pair?)
class VarValue {
public:
    ExprTypeId name;
    Value value;
};

class Function;

// FunctionTable is a table indexed by function name and contains a Function.
typedef std::map<std::string, Function *> FunctionTable;

// Symbol table is a list of VarValues (names and values). The list is searched from
// head to tail and new entries are commonly added to the beginning.
typedef std::list<VarValue> SymbolTable;

// Expression is the abstract base class for expressions. In Java parlance, this is
// being used as an interface (C++ doesn't have interfaces). Every expression type
// below must implement its own "eval" method which recursively
class Expression {
public:
    [[nodiscard]] virtual Value eval(FunctionTable &functionTable, SymbolTable &symbolTable) const = 0;
};

// An Integer expression. This is one of the simplest expression types: construct a Value
// and return it.
class [[maybe_unused]] ExpressionInt : public Expression {
private:
    Value val;
public:
    [[nodiscard]] Value eval(FunctionTable &functionTable, SymbolTable &symbolTable) const override { return val; };

    ExpressionInt() = delete;

    [[maybe_unused]] explicit ExpressionInt(int intval) {
        val.type = ET_INTEGER;
        val.intval = intval;
    }

    // This function is useful for testing
    void set(int intval) {
        val.type = ET_INTEGER;
        val.intval = intval;
    }
};

// ExpressionBool: a class very similar to ExpressionInt except it represents an
// expression as a true or false Value.
class [[maybe_unused]] ExpressionBool : public Expression {
private:
    Value val;
public:
    [[nodiscard]] Value eval(FunctionTable &functionTable, SymbolTable &symbolTable) const override { return val; };

    ExpressionBool() = delete;

    [[maybe_unused]] explicit ExpressionBool(bool v) {
        val.type = ET_BOOL;
        val.booleanValue = v;
    }
};

// ExpressionPlus: represents an addition. This evaluator must evaluate the
// left and right expressions and with those values, perform a type-aware
// addition operation. If either the right or left value is not an integer,
// throw a TypeMismatchException.
class [[maybe_unused]] ExpressionPlus : public Expression {
private:
    Expression *left, *right;
public:
    [[nodiscard]] Value eval(FunctionTable &functionTable, SymbolTable &symbolTable) const override;

    ExpressionPlus() = delete;

    [[maybe_unused]] explicit ExpressionPlus(Expression *l, Expression *r) : left(l), right(r) {}
};

// ExpressionEqual: represents a test for equality. This evaluator must evaluate the
// left and right expressions and with those values, perform a type-aware comparison
// operation. If the types do not match, throw a TypeMismatchException.
class [[maybe_unused]] ExpressionEqual : public Expression {
private:
    Expression *left, *right;
public:
    [[nodiscard]] Value eval(FunctionTable &functionTable, SymbolTable &symbolTable) const override;

    ExpressionEqual() = delete;

    [[maybe_unused]] explicit ExpressionEqual(Expression *l, Expression *r) : left(l), right(r) {}
};

// ExpressionIf: evaluate an "if" expression. This type of expression has three parts:
// a condition (which must evaluate to a boolean). Based on that boolean, either the
// trueSide or falseSide expression (but not both) are evaluates and the result is
// returned. If the condition does not evaluate to a boolean, the evaluator should
// throw a TypeMismatchException.
class [[maybe_unused]] ExpressionIf : public Expression {
private:
    Expression *cond, *trueSide, *falseSide;
public:
    [[nodiscard]] Value eval(FunctionTable &functionTable, SymbolTable &symbolTable) const override;

    ExpressionIf() = delete;

    [[maybe_unused]] explicit ExpressionIf(Expression *c, Expression *t, Expression *f) :
            cond(c), trueSide(t), falseSide(f) {}
};

// ExpressionId: the evaluator for this expression must look up the symbol in the
// symbol table. throw an UnknownVariableException if the name is not found. Otherwise,
// return it's value.
class [[maybe_unused]] ExpressionId : public Expression {
private:
    std::string id;
public:
    [[nodiscard]] Value eval(FunctionTable &functionTable, SymbolTable &symbolTable) const override;

    ExpressionId() = delete;

    [[maybe_unused]] explicit ExpressionId(std::string *sp) : id(*sp) {}
};

// ExpressionFuncall: id(arg1, ...argN)
// The evaluator here must look up the function in the function table. If not found
// throw a UnknownFunctionException. Next, expressions in the argument list are
// matched with names and added to the symbol table. The function's expression is
// then evaluated and the result saved as the future return value. Finally, all
// entries added to the symbol table are removed and the return value is returned.
//
// The evaluator should throw exceptions for the following conditions:
//   TooManyArgumentsException - too many arguments are provided
//   NotEnoughArgumentsException - not enough arguments to the function have been provided
//   TypeMismatchException - the function returns a data type that doesn't match it's
//                            declared type, an argument does not match the type required.
//   UnknownFunctionException - the named function does not exist
class [[maybe_unused]] ExpressionFuncall : public Expression {
private:
    std::string funname;
    std::list<Expression *> argexpr;
public:
    [[nodiscard]] Value eval(FunctionTable &functionTable, SymbolTable &symbolTable) const override;

    ExpressionFuncall() = delete;

    [[maybe_unused]] explicit ExpressionFuncall(std::string *name, std::list<Expression *> *a) :
            funname(*name) {
        argexpr = std::move(*a);
    }
};

// ExpressionLet:
// The evaluator should create a new variable binding (name -> value) in the symbol table.
// Then the "in" expression is evaluated and the result saved as the future return value.
// The entry is removed from the symbol table and the return value is returned.
class [[maybe_unused]] ExpressionLet : public Expression {
private:
    std::string varname;
    Expression *binding;
    Expression *block;
public:
    [[nodiscard]] Value eval(FunctionTable &functionTable, SymbolTable &symbolTable) const override;

    ExpressionLet() = delete;

    [[maybe_unused]] explicit ExpressionLet(std::string *name, Expression *val, Expression *code) :
            varname(*name), binding(val), block(code) {}
};

class TypeMismatchException : public std::exception {
private:
    std::string msg;
public:
    TypeMismatchException() { msg = "type mismatch"; }
    [[nodiscard]] const char *what() const noexcept override { return msg.c_str(); }
};

class UnknownVariableException : public std::exception {
private:
    std::string msg;
public:
    UnknownVariableException() { msg = "unknown variable"; }
    [[nodiscard]] const char *what() const noexcept override { return msg.c_str(); }
};

class UnknownFunctionException : public std::exception {
private:
    std::string msg;
public:
    UnknownFunctionException() { msg = "unknown function"; }
    [[nodiscard]] const char *what() const noexcept override { return msg.c_str(); }
};

class NotEnoughArgumentsException : public std::exception {
private:
    std::string msg;
public:
    NotEnoughArgumentsException() { msg = "Not enough arguments"; }
    [[nodiscard]] const char *what() const noexcept override { return msg.c_str(); }
};

class TooManyArgumentsException : public std::exception {
private:
    std::string msg;
public:
    TooManyArgumentsException() { msg = "Too many arguments"; }
    [[nodiscard]] const char *what() const noexcept override { return msg.c_str(); }
};

class MultiplyDefinedException : public std::exception {
private:
    std::string msg;
public:
    MultiplyDefinedException() { msg = "Multiply defined"; }
    [[nodiscard]] const char *what() const noexcept override { return msg.c_str(); }
};

class Function {
public:
    ExprTypeId name;
    std::list<ExprTypeId *> *params;
    Expression *expr;
};

#endif //P2_EXPRESSION_H
