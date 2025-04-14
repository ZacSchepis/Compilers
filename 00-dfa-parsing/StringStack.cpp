#include <utility>
#include "StringStack.h"

StringStack::StringStack(std::string s) {
    buf = std::move(s);
    ptr = 0;
}

int StringStack::get() {
    if (ptr == buf.size())
        return -1;
    return buf.at(ptr++);
}

int StringStack::unget() {
    if (ptr == 0)
        return -1;
    ptr--;
    return 0;
}
