cmake_minimum_required(VERSION 3.14)
project(mystd)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# Compiler flags
if(CMAKE_BUILD_TYPE STREQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif(CMAKE_BUILD_TYPE STREQUAL Debug)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined")
endif()

# Coverage flags
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    else()
        message(WARNING "Coverage is only supported with GCC or Clang")
    endif()
endif()

# Find dependencies
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    message(FATAL_ERROR "GTest not found")
endif()
find_package(benchmark QUIET)
if(NOT benchmark_FOUND)
    message(FATAL_ERROR "benchmark not found")
endif()
find_package(Catch2 3 QUIET)
if(NOT Catch2_FOUND)
    message(FATAL_ERROR "Catch2 not found")
endif()

find_package(Doxygen REQUIRED)
configure_file(
    docs/Doxyfile
    ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    @ONLY
)

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
file(GLOB_RECURSE BENCHMARK_TEST_SOURCES tests/bench_catch_tests/*.cpp)
# add_executable(benchmark_tests ${BENCHMARK_TEST_SOURCES})
# target_link_libraries(benchmark_tests PRIVATE benchmark::benchmark mystd)
add_executable(catch_bench ${BENCHMARK_TEST_SOURCES})
target_link_libraries(catch_bench PRIVATE Catch2::Catch2WithMain)

# Coverage report generation
if(ENABLE_COVERAGE)
    find_program(LCOV lcov)
    find_program(GENHTML genhtml)
    if(LCOV AND GENHTML)
        add_custom_target(coverage
            COMMAND ${LCOV} --directory . --capture --output-file coverage.info
            COMMAND ${LCOV} --remove coverage.info '/usr/*' '*/tests/*' --output-file coverage.info
            COMMAND ${GENHTML} coverage.info --output-directory coverage_report
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating coverage report"
        )
    else()
        message(WARNING "lcov or genhtml not found, coverage report generation disabled")
    endif()
endif()

add_custom_command(
    OUTPUT ${CMAKE_BINARY_DIR}/docs/doxygen/xml/index.xml
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/docs/doxygen
    COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
    DEPENDS ${CMAKE_SOURCE_DIR}/include/mystd/
    COMMENT "Generating Doxygen XML..."
)

add_custom_target(docs
    DEPENDS ${CMAKE_BINARY_DIR}/docs/doxygen/xml/index.xml
    COMMAND ${SPHINX_EXECUTABLE} -M html ${CMAKE_SOURCE_DIR}/docs/sphinx ${CMAKE_BINARY_DIR}/docs/sphinx
)
