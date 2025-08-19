cmake_minimum_required(VERSION 3.14)
project(mystd LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose build type" FORCE)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# -----------------------------------------------------------------------------
# mystd lib with cxx 24
# -----------------------------------------------------------------------------
add_library(mystd INTERFACE)
target_include_directories(mystd INTERFACE ${CMAKE_SOURCE_DIR}/include)

target_compile_features(mystd INTERFACE cxx_std_23)

# -----------------------------------------------------------------------------
# targets
# -----------------------------------------------------------------------------
add_library(project_options INTERFACE)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(project_options INTERFACE -O3)
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(project_options INTERFACE -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(project_options INTERFACE -fsanitize=address,undefined)
endif()

# coverage
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE AND (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang"))
    target_compile_options(project_options INTERFACE --coverage -fprofile-arcs -ftest-coverage)
    target_link_options(project_options INTERFACE --coverage)
endif()

# -----------------------------------------------------------------------------
# Requirenments
# -----------------------------------------------------------------------------
find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)
find_package(Catch2 3 REQUIRED)

# -----------------------------------------------------------------------------
# Documentation
# -----------------------------------------------------------------------------
find_package(Doxygen QUIET)
if(Doxygen_FOUND)
    configure_file(docs/Doxyfile ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/docs/doxygen/xml/index.xml
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/docs/doxygen
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        DEPENDS ${CMAKE_SOURCE_DIR}/include/mystd/
        COMMENT "Generating Doxygen XML..."
    )

    add_custom_target(doxygen_docs
        DEPENDS ${CMAKE_BINARY_DIR}/docs/doxygen/xml/index.xml
    )

    add_custom_target(docs
        DEPENDS doxygen_docs
        COMMAND ${SPHINX_EXECUTABLE} -M html ${CMAKE_SOURCE_DIR}/docs/sphinx ${CMAKE_BINARY_DIR}/docs/sphinx
    )
endif()

# -----------------------------------------------------------------------------
# Tests
# -----------------------------------------------------------------------------
enable_testing()

file(GLOB_RECURSE UNIT_TEST_SOURCES tests/unit_tests/*.cpp)
add_executable(unit_tests ${UNIT_TEST_SOURCES})
target_link_libraries(unit_tests PRIVATE GTest::GTest GTest::Main mystd project_options)
add_test(NAME unit_tests COMMAND unit_tests)
gtest_discover_tests(unit_tests)

file(GLOB_RECURSE BENCHMARK_TEST_SOURCES tests/bench_catch_tests/*.cpp)
add_executable(catch_bench ${BENCHMARK_TEST_SOURCES})
target_link_libraries(catch_bench PRIVATE Catch2::Catch2WithMain mystd project_options)

# -----------------------------------------------------------------------------
# coverage report
# -----------------------------------------------------------------------------
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
