# LL(1) parsing

In this project, you'll create 3 different parsers. The first two
are for the language we've talked about in class (a lot), and the
final one will be for the language in HW3.

## Recursive Descent

For this part of the assignment, you'll create a recursive descent
parser for the grammar below:

    X -> T $
    T -> R
    T -> a T c
    R -> b R
    R ->

Where X, T, and R are non-terminals, and a, b, and c are terminals.

Your function, `parseRecurse` is given two arguments:

 - a stack representing the input (the "top" element of the stack is the next element to be parsed)
 - an output vector

The output vector should have a new element appended representing the
identifier of the terminal or non-terminal just processed. For example,
for an "empty" parse, the output vector should contain: X, T, R, $.
It should be fairly easy to spot the right location(s) to put the
call(s) to `push_back`.

Your parse function should return `true` if the string is accepted by
the language and `false` otherwise.

## Table Driven

For this part of the assignment, I have given you a table, and you must
provide the parsing logic that uses the table to parse the input. The
language is the same as the previous language. The table is represented
as a map indexed by a `NonTerminal` which itself contains a map indexed
by a `Terminal`. If no element exists in either map, then a syntax
error has occurred.

Like the previous part, you must append identifier of the symbol just
processed to the output vector as you parse.

Your parse function should return `true` if the string is accepted by
the language and `false` otherwise.

## A Different Language

For this part of the assignment, you're free to choose between a
recursive descent parser and a table driven parser. You'll use your
work on HW3 to inform the creation of your parser. The language
is below:

    E -> E E *
    E -> E E +
    E -> num

`num` is a terminal (it represents the class of numbers). `*` and `+`
are terminals as well. `E` is the only non-terminal. Note: this
language will not work directly for an LL(1) parser. You must
eliminate left recursion, perform left factorization and compute the
Nullable, FIRST, and FOLLOW sets before diving into this one.

# What to turn in

I need 3 files back from you:

  - recursiveDescent.cpp
  - tableDriven.cpp
  - postfixParser.cpp

In all the files, your modifications are only allowed below the
literal text: `// MODIFICATIONS BELOW THIS LINE ONLY`
I will merge the lower part of the file with my own tests while
grading.

You may optionally include a README file describing your
implementation.

# Rubric

Correct implementation of recursive descent parser: 10pts

Correct implementation of table driven LL(1) parser: 10pts

Correct implementation of postfix parser: 30 points

Please note: I really read the code. Please don't waste my
time with implementations that don't actually implement the
algorithms. Correct output without a correct algorithm is worth
a 0 on the whole assignment.
