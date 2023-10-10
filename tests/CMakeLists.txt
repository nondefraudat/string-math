cmake_minimum_required(VERSION 3.14)

project(Tests)

link_libraries("string-math")
include_directories("${CMAKE_SOURCE_DIR}/src")

add_executable(definition definition.cpp)
