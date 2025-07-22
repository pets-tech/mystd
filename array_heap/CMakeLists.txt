cmake_minimum_required(VERSION 3.14)
project(array_heap)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL Release)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fsanitize=address,undefined -fno-omit-frame-pointer --coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address,undefined --coverage")
endif()

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

include_directories(include)
add_library(mylib examples/main.cpp include/array_heap/array.hpp)


# unit tests
enable_testing()
add_executable(unit_tests tests/unit_tests.cpp)
target_link_libraries(unit_tests PRIVATE GTest::GTest GTest::Main mylib)
add_test(NAME unit_tests COMMAND unit_tests)

 # bechmark tests
add_executable(benchmark_tests tests/benchmark_tests.cpp)
target_link_libraries(benchmark_tests PRIVATE benchmark::benchmark mylib)

# examples
add_executable(main examples/main.cpp)