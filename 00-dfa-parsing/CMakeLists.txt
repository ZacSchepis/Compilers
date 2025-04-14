
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# I'm using Catch2 for unit testing
include(FetchContent)
FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.4.0)

FetchContent_MakeAvailable(Catch2)

# flex must be available.
# - Windows: I installed it with Chocolatey (package: winflexbison3)
# - Linux: apt install bison flex
# - MacOS: ???
find_package(FLEX)

FLEX_TARGET(MyScanner lex.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
#add_executable(p1
#	main.cc
#	${FLEX_MyScanner_OUTPUTS}
#		StringStack.cpp
#		StringStack.h
#)

enable_testing()


add_executable(test_stringstack test_stringstack.cpp StringStack.cpp)
target_link_libraries(test_stringstack PRIVATE Catch2::Catch2WithMain)

add_test(NAME test_stringstack
		 COMMAND $<TARGET_FILE:test_stringstack> --success)

add_executable(test_dfa test_dfa.cpp StringStack.cpp dfa.cpp)
target_link_libraries(test_dfa PRIVATE Catch2::Catch2WithMain)

add_test(NAME test_dfa
		COMMAND $<TARGET_FILE:test_dfa> --success)

add_executable(test_lexer test_lexer.cpp ${FLEX_MyScanner_OUTPUTS})
target_link_libraries(test_lexer PRIVATE Catch2::Catch2WithMain)

add_test(NAME test_lexer
		COMMAND $<TARGET_FILE:test_dfa> --success)

project(p1)
