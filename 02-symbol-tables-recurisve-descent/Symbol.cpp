//
// Created by Jason Wright on 2/2/2024.
//

#include "Symbol.h"
#include <cassert>

bool
Symbol::operator==(const Symbol &other) const {
    return (terminal == other.terminal && id == other.id);
}

bool
Symbol::operator<(Symbol const &other) const {
    if (terminal == other.terminal)
        return (id < other.id);
    if (terminal)
        return false;
    return true;
}

Symbol::operator NonTerminal() const {
    assert(!terminal);
    return NonTerminal(id);
}
Symbol::operator Terminal() const {
    assert(terminal);
    return Terminal(getId());
}
