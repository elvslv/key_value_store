cmake_minimum_required(VERSION 3.9)
project(KEY_VALUE_STORE)

set(CMAKE_VERBOSE_MAKEFILE ON)

include_directories(${CMAKE_CURRENT_BINARY_DIR}, membership_protocol, network, utils, proto)
set(CMAKE_CXX_FLAGS "-Wall -g -std=c++14")

add_library(key_value_store_lib "")
include("network/CMakeLists.txt")
include("utils/CMakeLists.txt")

add_executable(app main.cpp)
target_link_libraries(app key_value_store_lib)

add_subdirectory(network)
add_subdirectory(utils)
add_subdirectory(membership_protocol)
add_subdirectory(proto)

target_link_libraries(app proto ${PROTOBUF_LIBRARY})

enable_testing()
add_subdirectory(tests)
add_test(test1 key_value_store_tests)
