cmake_minimum_required(VERSION 3.22)
project(p3)

set(CMAKE_CXX_STANDARD 20)

# flex and bison must be available.
# - Windows: I installed it with Chocolatey (package: winflexbison3)
# - Linux: apt install bison flex
# - MacOS: ???
find_package(FLEX)

FLEX_TARGET(MyScanner lex.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)

find_package(BISON)

BISON_TARGET(MyParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/parser.h
        COMPILE_FLAGS -Wcounterexamples)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

enable_testing()

add_executable(p3 main.cpp ${BISON_MyParser_OUTPUTS} ${FLEX_MyScanner_OUTPUTS}
        Expression.cpp)

add_test(NAME p3 COMMAND $<TARGET_FILE:p3>)

BISON_TARGET(DecafParser decafparser.y ${CMAKE_CURRENT_BINARY_DIR}/decafparser.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/decafparser.h
        COMPILE_FLAGS -Wcounterexamples)

FLEX_TARGET(DecafScanner decaflex.l  ${CMAKE_CURRENT_BINARY_DIR}/decaflex.cc)

add_executable(decaf ${BISON_DecafParser_OUTPUTS} ${FLEX_DecafScanner_OUTPUTS})


add_test(NAME decaf_test1
         COMMAND decaf test1.decaf
         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME decaf_test-if
        COMMAND decaf test-if.decaf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME decaf_test-ifelse
        COMMAND decaf test-ifelse.decaf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME decaf_test-for
        COMMAND decaf test-for.decaf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_test(NAME decaf_test-noargs
        COMMAND decaf test-noargs.decaf
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
