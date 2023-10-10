cmake_minimum_required(VERSION 3.14)

project(Tests)

link_libraries("string-math")
include_directories("../src")

add_executable(definition definition.cpp)
add_executable(result result.cpp)

enable_testing()

set(CTEST_OUTPUT_ON_FAILURE ON)

add_test(NAME DefinitionTests COMMAND definition)
add_test(NAME ResultTests COMMAND result)
