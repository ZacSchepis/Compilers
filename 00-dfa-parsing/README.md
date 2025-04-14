
# Project 1: Lexical Analysis

The goal of lexical analysis is to divide the input
up into "words". Another piece of code, a parser,
will be responsible for turning those words into
a language (a grammar).

There are two parts of this assignment.

- Implement a DFA in C++
- Implement that same DFA in "flex"

## Part I: DFA in C++

You need to modify the files `dfa.cpp` and `dfa.h`.

The class `StateMachine` is incomplete. Specifically,
the `reset()` and `next()` member functions are not
implemented. `reset()` acts something like a constructor:
it should prepare a `StateMachine` object to be reused
(reinitialize it to its starting state... whatever
that means for you).

`next()` is where the action truly happens. You're
given a `StringStack` object. You can fetch one character
at a time from the StringStack and assemble the next
token. `next()` should return a value from `tokens.h`
representing the type of the current match and set the
`lexeme` variable to the text that matched the pattern.

If you read too many characters, you can use the `unget()`
member function of `StringStack` to return the current
character to the stack.

Your DFA should match the following patterns:

- C++ identifiers (`TokIdentifier`)
- Non-negative Integers (`TokInteger`)
- whitespace (space, newline, tab, carriage return) (`TokWhitespace`)
- end-of-input (`TokEOF`)
- anything else (`TokError`)

`next()` will be called over and over again until
it returns `TokEOF`.

## Part II: `flex` implementation

Writing DFA's by hand is fun, right? If you don't agree,
that's fine, and luckily there's a tool called `flex`
which will generate lexical analyzers.

I've helpfully provided a skeleton (`lex.l`) which you
can modify. You should only modify the code between the `%%`
markers because the test code depends on the code above and
below that.

Here you need to provide the regular expressions and
C++ code to run when each one is matched. The format
of that part of the file is:

```
RegularExpression1   { C++ code1(); }

RegularExpression2   { C++ code2(); }
```

`flex` helpfully creates a variable called `yytext`
that contains the lexeme, so there's really nothing
for you to do except return statements.

## Testing

The project has built-in tests. In CLion you can set
the target to a specific test OR "All CTest" to run all
the tests. You know you are done when all of the tests
pass.

- `test_stringstack`: if these tests fail, let me know
- `test_dfa`: tests of your DFA
- `test_lexer`: tests of your `flex` code

## Building

I've had good luck building this assignment in CLion
Visual Studio Code (with the Visual Studio toolchain),
and emacs (under Linux).

Visual Studio gives me an error I haven't figure out:

`/implib missing argument`

From the command line with a Windows Visual Studio
command prompt, you can build and test like this:

From the Linux (and probably mac) command line:

```
jason@ELECEC07645WRIG:/mnt/c/Users/Jason Wright/Desktop/4481/p1$ mkdir linux
jason@ELECEC07645WRIG:/mnt/c/Users/Jason Wright/Desktop/4481/p1$ cd linux
jason@ELECEC07645WRIG:/mnt/c/Users/Jason Wright/Desktop/4481/p1/linux$ cmake ..
-- The CXX compiler identification is GNU 11.4.0
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: /usr/bin/c++ - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Performing Test HAVE_FLAG__ffile_prefix_map__mnt_c_Users_Jason_Wright_Desktop_4481_p1_linux__deps_catch2_src__
-- Performing Test HAVE_FLAG__ffile_prefix_map__mnt_c_Users_Jason_Wright_Desktop_4481_p1_linux__deps_catch2_src__ - Failed
-- Found FLEX: /usr/bin/flex (found version "2.6.4")
-- The C compiler identification is GNU 11.4.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/c/Users/Jason Wright/Desktop/4481/p1/linux```
jason@ELECEC07645WRIG:/mnt/c/Users/Jason Wright/Desktop/4481/p1/linux$ cmake --build .
[  1%] [FLEX][MyScanner] Building scanner with flex 2.6.4
[  2%] Building CXX object CMakeFiles/p1.dir/main.cc.o
[  3%] Building CXX object CMakeFiles/p1.dir/lexer.cc.o
[  4%] Building CXX object CMakeFiles/p1.dir/StringStack.cpp.o
[  5%] Linking CXX executable p1
[  5%] Built target p1
[  5%] Building CXX object _deps/catch2-build/src/CMakeFiles/Catch2.dir/catch2/benchmark/catch_chronometer.cpp.o
[  6%] Building CXX object _deps/catch2-build/src/CMakeFiles/Catch2.dir/catch2/benchmark/detail/catch_benchmark_function.cpp.o
...
[ 96%] Building CXX object CMakeFiles/test_stringstack.dir/test_stringstack.cpp.o
[ 96%] Building CXX object CMakeFiles/test_dfa.dir/test_dfa.cpp.o
[ 96%] Building CXX object CMakeFiles/test_stringstack.dir/StringStack.cpp.o
[ 98%] Building CXX object CMakeFiles/test_dfa.dir/StringStack.cpp.o
[ 98%] Building CXX object CMakeFiles/test_dfa.dir/dfa.cpp.o
[ 99%] Linking CXX executable test_stringstack
[100%] Linking CXX executable test_dfa
[100%] Built target test_stringstack
[100%] Built target test_dfa
jason@ELECEC07645WRIG:/mnt/c/Users/Jason Wright/Desktop/4481/p1/linux$  ctest
Test project /mnt/c/Users/Jason Wright/Desktop/4481/p1/linux
    Start 1: test_stringstack
1/3 Test #1: test_stringstack .................   Passed    0.14 sec
    Start 2: test_dfa
2/3 Test #2: test_dfa .........................   Passed    0.10 sec
    Start 3: test_lexer
3/3 Test #3: test_lexer .......................   Passed    0.14 sec

100% tests passed, 0 tests failed out of 3

Total Test time (real) =   0.51 sec
```

And windows... Not the command line for testing is slightly
different from Linux/Mac:

```text
C:\Users\Jason Wright\Desktop\4481\p1>mkdir win

C:\Users\Jason Wright\Desktop\4481\p1>cd win

C:\Users\Jason Wright\Desktop\4481\p1\win>cmake ..
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.19041.0 to target Windows 10.0.22621.
...
-- Build files have been written to: C:/Users/Jason Wright/Desktop/4481/p1/win

C:\Users\Jason Wright\Desktop\4481\p1\win>cmake --build .
MSBuild version 17.8.3+195e7f5a3 for .NET Framework

1>Checking Build System
...
Building Custom Rule C:/Users/Jason Wright/Desktop/4481/p1/CMakeLists.txt

C:\Users\Jason Wright\Desktop\4481\p1\win>ctest -C Debug
Test project C:/Users/Jason Wright/Desktop/4481/p1/win
Start 1: test_stringstack
1/2 Test #1: test_stringstack .................   Passed    1.26 sec
Start 2: test_dfa
2/2 Test #2: test_dfa .........................   Passed    0.61 sec
Start 3: test_lexer
3/3 Test #3: test_lexer .......................   Passed    0.14 sec

100% tests passed, 0 tests failed out of 3

Total Test time (real) =   2.01 sec
```

## What to turn in

You need to turn in exactly 3 files:

- `dfa.cpp`: your DFA implementation
- `dfa.h`: your DFA declarations
- `lex.l`: your `flex` implementation of the lexical analyzer
