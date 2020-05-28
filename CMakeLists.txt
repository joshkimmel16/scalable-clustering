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

# Enable cmake testing
include(CTest)
enable_testing()

# Enable GoogleTest
include(GoogleTest)
add_subdirectory(/usr/src/googletest googletest)

include_directories(include)

#add_libraries
add_library(datapoint src/datapoint.cc)
add_library(dataparser src/dataparser.cc)
add_library(compare src/compare.cc)
add_library(config src/config.cc)
add_library(clustergraph src/clustergraph.cc)

#add_executable...
#target_link_libraries

#add executable (for tests)...
#target_link_libarires (for tests)...

#gtest_discover_tests...