# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tests/googletest-download" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/tests/googletest-download" )
 
# Prevent GoogleTest from overriding our compiler/linker options
# when building with Visual Studio
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
 
# Add googletest directly to our build. This adds
# the following targets: gtest, gtest_main, gmock
# and gmock_main
add_subdirectory("${CMAKE_BINARY_DIR}/tests/googletest-src"
                 "${CMAKE_BINARY_DIR}/tests/googletest-build")
 
include_directories("${gtest_SOURCE_DIR}/include"
                    "${gmock_SOURCE_DIR}/include")
 
add_executable(key_value_store_tests "utils/CMakeLists.txt")
target_link_libraries(key_value_store_tests key_value_store_lib gtest_main proto)
add_subdirectory(utils)
