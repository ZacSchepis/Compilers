//
// Created by Jason Wright on 2/11/2024.
//

#ifndef P2_HELPERS_H
#define P2_HELPERS_H

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include "Symbol.h"

// A production rule is a NonTerminal (left hand side) and
// a vector of Symbols (right hand side). I use "pair" to
// represent them.
typedef std::pair<NonTerminal, std::vector<Symbol>> ProductionRule;

// A grammar is just a vector of Production Rules. The left hand side
// of the first rule is assumed to be the start symbol.
typedef std::vector<ProductionRule> Grammar;

// a ParseTable is a map of maps. The idea is that you can index it
// by a NonTerminal and a Terminal to get the corresponding production
// rule. Note: the mapping may not exist (missing production during
// parsing means that there is no transition).
typedef std::map<NonTerminal, std::map<Terminal, ProductionRule>> ParseTable;

bool parseRecurse(std::stack<std::string>& input, std::vector<std::string>& output);
bool parseRecurseT(std::stack<std::string>& input, std::vector<std::string>& output);
bool match(std::stack<std::string> &input, const std::string &id, std::vector<std::string> &output);
bool parseRecurseR(std::stack<std::string> &input, std::vector<std::string> &output);

std::ostream &operator<<(std::ostream &os, const Symbol &s);
std::ostream &operator<<(std::ostream& os, ProductionRule const &r);
std::ostream &operator<<(std::ostream& os, const Grammar &g);

std::stack<std::string> makeStringStack(const std::string &s);

// How to print a "Stack"
template <typename T>
std::ostream &operator<<(std::ostream &os, std::stack<T> const &m) {
    std::stack<T> s = m;
    std::string sep;

    while (!s.empty()) {
        os << sep << s.top();
        s.pop();
        sep = " ";
    }
    return os;
}

// How to print a vector
template <typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &m) {
    std::string sep;

    for (auto s: m) {
        os << sep << s;
        sep = " ";
    }
    return os;
}

#endif //P2_HELPERS_H
