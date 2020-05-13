cmake_minimum_required(VERSION 3.14)
project(isformula)

find_library(PCRE_LIBRARY pcre)


set(CMAKE_CXX_STANDARD 11)

add_executable(test_pllfv tests/test_pllfv.cpp lib/pllfv.h lib/pllfv.cpp
        lib/pllftree.h lib/pllftree.cpp)

add_executable(test_tt tests/test_truth_table.cpp lib/pllfv.h lib/pllfv.cpp
        lib/pllftree.h lib/pllftree.cpp)

target_link_libraries(test_pllfv ${PCRE_LIBRARY})
target_link_libraries(test_tt ${PCRE_LIBRARY})