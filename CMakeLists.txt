cmake_minimum_required(VERSION 3.23)
project(FiniteAutomaton)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

add_executable(TEST test.cpp dfa.cpp cdfa.cpp nfa.cpp)
target_link_libraries(TEST gtest gtest_main)

SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")