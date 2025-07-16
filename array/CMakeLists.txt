cmake_minimum_required(VERSION 3.14)
project(array)

enable_testing()

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

include_directories(include)

add_library(mylib examples/main.cpp include/array.hpp)

# unit tests
add_executable(unit_tests tests/unit_tests.cpp)
target_link_libraries(unit_tests PRIVATE GTest::GTest GTest::Main mylib)
add_test(NAME unit_tests COMMAND unit_tests)

# benchmark tests
add_executable(benchmark_tests tests/benchmark_tests.cpp)
target_link_libraries(benchmark_tests PRIVATE benchmark::benchmark mylib)

# examples
add_executable(main examples/main.cpp)

