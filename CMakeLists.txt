cmake_minimum_required(VERSION 3.9)
project(KEY_VALUE_STORE)

include_directories(build/gen, membership_protocol, network)
set(CMAKE_CXX_FLAGS "-Wall -g -std=c++14")
add_subdirectory(network)
add_subdirectory(membership_protocol)
add_subdirectory(utils)
add_subdirectory(build/gen)

add_executable(main main.cpp)