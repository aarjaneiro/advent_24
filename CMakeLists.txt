cmake_minimum_required(VERSION 3.29)
project(advent_24)

set(CMAKE_CXX_STANDARD 20)
add_compile_definitions(ADVENT_24_DIR="${CMAKE_CURRENT_SOURCE_DIR}")
add_subdirectory(day_1)
add_subdirectory(day_2)
