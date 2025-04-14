
#ifndef P1_STRINGSTACK_H
#define P1_STRINGSTACK_H


#include <string>

class StringStack {
public:
    explicit StringStack(std::string s);

    // get the next character from the string. Returns -1 on EOF.
    // note: this returns an int not a char. After checking for integer
    // -1, one can cast the result safely to character.
    int get();

    // Run the pointer back one position.
    int unget();

private:
    std::string::size_type ptr;
    std::string buf;
};


#endif //P1_STRINGSTACK_H
