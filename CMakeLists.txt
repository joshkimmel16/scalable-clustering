cmake_minimum_required(VERSION 3.10.0)
project(scalable-clustering)

# Make sure developers do not run cmake in the main project directory, to keep
# build artifacts from becoming clutter
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed.
    Please make a new directory (called a build directory) and run CMake from there.
    You may need to remove CMakeCache.txt." )
endif()

# Turn on debug builds if we are building in a devel shell
if (CMAKE_BUILD_TYPE STREQUAL "" AND "$ENV{DEVEL_SHELL}" STREQUAL "1")
    message(STATUS "Setting debug build type by default in devel shell")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Output binaries to a sub directory "bin"
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Code Coverage Configuration
add_library(coverage_config INTERFACE)
option(CODE_COVERAGE "Enable coverage reporting" OFF)
if(CODE_COVERAGE)
message("Adding coverage flags")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(coverage_config INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
endif(CODE_COVERAGE)


# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

include_directories(include)

#add_libraries
add_library(datapoint src/datapoint.cc)
target_link_libraries(datapoint coverage_config)
add_library(dataparser src/dataparser.cc)
target_link_libraries(dataparser coverage_config)
add_library(compare src/compare.cc)
target_link_libraries(compare coverage_config)
add_library(config src/config.cc)
target_link_libraries(config coverage_config)
add_library(clustergraph src/clustergraph.cc)
target_link_libraries(clustergraph coverage_config)
add_library(reporter src/reporter.cc)
target_link_libraries(reporter coverage_config)

#add_executable
add_executable(main src/main.cc)
target_link_libraries(main reporter dataparser config compare datapoint clustergraph coverage_config)

#add test executables
add_executable(datapoint_test tests/datapoint_test.cc)
target_link_libraries(datapoint_test datapoint gtest_main coverage_config)
add_executable(compare_test tests/compare_test.cc)
target_link_libraries(compare_test compare datapoint gtest_main coverage_config)
add_executable(config_test tests/config_test.cc)
target_link_libraries(config_test config compare datapoint gtest_main coverage_config)
add_executable(clustergraph_test tests/clustergraph_test.cc)
target_link_libraries(clustergraph_test clustergraph config compare datapoint gtest_main coverage_config)
add_executable(dataparser_test tests/dataparser_test.cc)
target_link_libraries(dataparser_test dataparser config compare datapoint clustergraph gtest_main coverage_config)
add_executable(reporter_test tests/reporter_test.cc)
target_link_libraries(reporter_test reporter dataparser config compare datapoint clustergraph gtest_main coverage_config)

message(${CMAKE_CURRENT_SOURCE_DIR})
#discover tests
gtest_discover_tests(datapoint_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(compare_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(config_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(clustergraph_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(dataparser_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)
gtest_discover_tests(reporter_test WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/tests)

