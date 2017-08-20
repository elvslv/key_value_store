cmake_minimum_required(VERSION 3.9)
project(KEY_VALUE_STORE)

set(CMAKE_CXX_FLAGS "-Wall -g -std=c++11")
add_subdirectory(network)

add_executable(main main.cpp)