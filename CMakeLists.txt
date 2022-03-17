cmake_minimum_required(VERSION 3.20)
project(b_tree C)

set(CMAKE_C_STANDARD 99)
add_subdirectory(idx)

add_executable(b_tree main.c)

target_link_libraries(b_tree PUBLIC idx)
