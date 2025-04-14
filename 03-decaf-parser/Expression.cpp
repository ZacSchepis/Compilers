//
// Created by Jason Wright on 3/17/2024.
// Modified by Zac Schepis over 4/10-4/14, implementing the below methods
//

#include "Expression.h"

#include <iostream>
Value ExpressionPlus::eval(FunctionTable &functionTable, SymbolTable &symbolTable) const {
    auto lEval = this->left->eval(functionTable, symbolTable);
    auto rEval = this->right->eval(functionTable, symbolTable); 
    if(lEval.type != rEval.type) {
        throw TypeMismatchException();
    }
    Value res;
    switch (rEval.type){
        case ET_INTEGER: {
            res.type = rEval.type;
            res.intval = rEval.intval + lEval.intval;
            return res;
        } 
        default:
            throw TypeMismatchException();
    }
}

Value ExpressionEqual::eval(FunctionTable &functionTable, SymbolTable &symbolTable) const {
    auto lEval = this->left->eval(functionTable, symbolTable);
    auto rEval = this->right->eval(functionTable, symbolTable); 
    if(lEval.type != rEval.type) {
        throw TypeMismatchException();
    }
    Value res;
    res.type = ET_BOOL;
    switch(lEval.type){
        case ET_INTEGER:{
            res.booleanValue = (lEval.intval == rEval.intval);
            break;
        }
        case ET_BOOL: {
            res.booleanValue = (lEval.booleanValue == rEval.booleanValue);
            break;
        }
    }
    return res;
}

Value ExpressionIf::eval(FunctionTable &functionTable, SymbolTable &symbolTable) const {
    auto condCheck = this->cond->eval(functionTable, symbolTable);
    if(condCheck.type != ET_BOOL) throw TypeMismatchException();
    if(condCheck.booleanValue){
        return this->trueSide->eval(functionTable, symbolTable);
    } else {
        return this->falseSide->eval(functionTable, symbolTable);
    }

}

// Lookup the variable's value and return it.
Value ExpressionId::eval(FunctionTable &functionTable, SymbolTable &symbolTable) const {
    for(const auto& v : symbolTable){
        if(v.name.id == this->id) {
            return v.value;
        }
    }
    throw UnknownVariableException();
}
// The evaluator should throw exceptions for the following conditions:
Value ExpressionFuncall::eval(FunctionTable &functionTable, SymbolTable &symbolTable) const {
    auto it = functionTable.find(this->funname);
    // UnknownFunctionException - the named function does not exist
    if(it == functionTable.end()) throw UnknownFunctionException();
    auto t = it->second;
    // TooManyArgumentsException - too many arguments are provided
    if(t->params->size() > this->argexpr.size()) throw NotEnoughArgumentsException();
    // NotEnoughArgumentsException - not enough arguments to the function have been provided
    else if(t->params->size() < this->argexpr.size()) throw TooManyArgumentsException();
    else {
        Value res;
        SymbolTable local = symbolTable;
        auto paramsCopy = *(t->params);
        auto paramIt = paramsCopy.begin();
        auto argIt = this->argexpr.begin();
        while(paramIt != paramsCopy.end() && argIt != this->argexpr.end()){
            Value argValue = (*argIt)->eval(functionTable, symbolTable);
            if((*paramIt)->type != argValue.type){
                throw TypeMismatchException();
            }
            VarValue boundVar = {{(*paramIt)->id, argValue.type}, argValue};
            local.push_front(boundVar);
            ++paramIt;
            ++argIt;
        }


        return t->expr->eval(functionTable, local);
    }
}

Value ExpressionLet::eval(FunctionTable &functionTable, SymbolTable &symbolTable) const {
    auto varValue = this->binding->eval(functionTable, symbolTable);
    VarValue new_ = {{this->varname, varValue.type}, varValue};
    symbolTable.push_front(new_);
    Value result = this->block->eval(functionTable, symbolTable);
    symbolTable.pop_front();
    return result;
}
