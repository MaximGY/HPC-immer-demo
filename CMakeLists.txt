cmake_minimum_required(VERSION 3.26)
project(HPC_demo)

set(CMAKE_CXX_STANDARD 14)

add_executable(immer immer.cpp)
add_executable(std std.cpp)
