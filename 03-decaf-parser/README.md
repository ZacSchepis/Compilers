# Interpreter and Parsing

There are two parts to this assignment:

 - Implementation of a interpreter for a simple language
 - Implementation of a parser (using bison/flex) for a different language (DECAF)

## Interpreter Implementation

I've provided the flex and bison descriptions for the
language described in Chapter 5 (Interpretation) in the text.
Your job is to finish the implementation of the interpreter.
You'll likely find Figure 5.2 very helpful (and the surrounding
text).

You need to implement several functions, and they are all found in `Expression.cpp`.
These functions are called during the evaluation process of interpreted
expressions and descriptions of what they need to accomplish can be found in
`Expression.h`

There are MANY tests builtin to test your implementation of the required
functions (`main.cpp`).

## Implementation of a parser

The second part of this assignment is to create parser using flex and bison.
The language is a language called DECAF, and you can find its description in
the file `decaf.pdf` in this directory. You are ONLY going to create a parser
for this assignment... not a compiler or an interpreter. In other words, all
of your bison production rules will have no {}'s. The goal is to produce a
parser that correctly recognizes the language of strings that should be accepted
by a DECAF interpreter or parser.

To complete this exercise, you'll need to modify `decaflex.l` and `decafparser.y`
to add tokens and production rules. Where you draw the line between the two files
is up to you. remember: CFGs can do everything that DFAs can do... and more, but
it is generally faster and easier to follow if you split the responsibilities
between the two: use the lexer to recognize "words" and the parser to recognize
larger structures. Feel free to use my `lex.l` and `parser.y` for the simple
language as a reference.

When a build is done, a `decaf` executable (`decaf.exe` on Windows) is created.
It expects to be given a file via standard input and will print nothing if the
file has correct syntax and throw an exception if there's a syntax error (harsh, right?)

```
C:\Users\Jason Wright\Documents\4481\p3\build\Debug>echo likeyeahwhatever | decaf

C:\Users\Jason Wright\Documents\4481\p3\build\Debug>echo moo | decaf
moosyntax error
```

# What to turn in

You need to turn in 3 files:

  - `Expression.cpp`
  - `decaflex.l`
  - `decafparser.y`

You may also turn in DECAF files with correct syntax.

# Rubric

A correct implementation of the interpreter functionality is worth 50 points.

A correct implementation of the DECAF parser is worth 50 points.
