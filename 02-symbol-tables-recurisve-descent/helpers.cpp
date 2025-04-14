//
// Created by Jason Wright on 2/11/2024.
//

#include "helpers.h"
#include <ranges>

// How to print a "Symbol"
std::ostream &operator<<(std::ostream &os, const Symbol &s) {
    if (s.isTerminal())
        os << s.getId();
    else
        os << '(' << s.getId() << ')';
    return os;
}


// How to print a Production Rule
// (note the use of "first" and "second" to access the LHS and RHS)
std::ostream &operator<<(std::ostream& os, ProductionRule const &r) {
    os << r.first << "->" << r.second;
    return os;
}

// How to print a Grammar
std::ostream &operator<<(std::ostream& os, const Grammar &g) {
    for (auto const &r: g)
        os << r << std::endl;
    return os;
}

std::stack<std::string>
makeStringStack(const std::string &s) {
    std::stack<std::string> st;

    for (char it : std::ranges::reverse_view(s)) {
        std::string x;
        x.append(1, it);
        st.push(x);
    }
    return st;
}
