//
// Created by Jason Wright on 2/2/2024.
//

#ifndef P2_SYMBOL_H
#define P2_SYMBOL_H

#include <string>
#include <utility>

class Terminal;
class NonTerminal;

class Symbol {
public:
    Symbol(std::string i, std::string v, bool t) : id(std::move(i)), value(std::move(v)), terminal(t) {};
    Symbol() = delete;
    bool operator<(Symbol const &) const;
    [[nodiscard]] bool isTerminal() const { return terminal; }
    [[nodiscard]] std::string getId() const { return id; }
    [[nodiscard]] std::string getValue() const { return value; }
    explicit operator Terminal() const;
    explicit operator NonTerminal() const;
    bool operator==(const Symbol &) const;
private:
    std::string id;
    std::string value;
    bool terminal;
};

class Terminal: public Symbol {
public:
    Terminal() = delete;
    explicit Terminal(std::string id) : Symbol(std::move(id), "[not given]", true) {}
    explicit Terminal(std::string id, std::string value): Symbol(std::move(id), std::move(value), true) {}
};

class NonTerminal: public Symbol {
public:
    NonTerminal() = delete;
    explicit NonTerminal(const std::string& id) : Symbol(id, id, false) {}
};

#endif //P2_SYMBOL_H
