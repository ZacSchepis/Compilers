cmake_minimum_required(VERSION 3.27)
project(p2)

set(CMAKE_CXX_STANDARD 20)

add_executable(p2 main.cpp)

enable_testing()

add_executable(tableDriven tableDriven.cpp Symbol.cpp helpers.cpp)

add_test(NAME tableDriven
        COMMAND $<TARGET_FILE:tableDriven> --success)

add_executable(recursiveDescent recursiveDescent.cpp Symbol.cpp helpers.cpp)

add_test(NAME recursiveDescent
        COMMAND $<TARGET_FILE:recursiveDescent> --success)

add_executable(postfixParser postfixParser.cpp Symbol.cpp helpers.cpp)

add_test(NAME postfixParser
        COMMAND $<TARGET_FILE:postfixParser> --success)
