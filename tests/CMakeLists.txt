cmake_minimum_required(VERSION 3.14)

project(Tests)

link_libraries("string-math")
include_directories("../src")

file(GLOB PROJECT_SOURCES *.cpp *.hpp)
add_executable(tests ${PROJECT_SOURCES})

set(CTEST_OUTPUT_ON_FAILURE ON)

add_test(NAME Tests COMMAND tests)
