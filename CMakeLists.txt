cmake_minimum_required(VERSION 3.14)
project(mystd)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMAND on)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0 -O0 -fsanitize=address,undefined -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
endif()

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)
find_package(Catch2 3 REQUIRED)

include_directories(include)

add_library(mystd INTERFACE)
target_include_directories(mystd INTERFACE  include/mystd/*)

# EXAMPLES
# add_executable(main examples/main.cpp)

# TESTS

# unit tests
enable_testing()
file(GLOB_RECURSE UNIT_TEST_SOURCES tests/unit_tests/*.cpp)
add_executable(unit_tests ${UNIT_TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE GTest::GTest GTest::Main mystd)
add_test(NAME unit_tests COMMAND unit_tests)
gtest_discover_tests(unit_tests)

 # bechmark tests
file(GLOB_RECURSE BENCHMARK_TEST_SOURCES tests/benck_catch_tests/*.cpp)
# add_executable(benchmark_tests ${BENCHMARK_TEST_SOURCES})
# target_link_libraries(benchmark_tests PRIVATE benchmark::benchmark mystd)
add_executable(catch_bench ${BENCHMARK_TEST_SOURCES})
target_link_libraries(catch_bench PRIVATE Catch2::Catch2WithMain)
