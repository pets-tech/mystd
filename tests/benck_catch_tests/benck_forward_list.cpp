#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <forward_list>
#include "mystd/forward_list.hpp"


constexpr int N = 500;

TEST_CASE("Benchmark forward_list push_front", "[benchmark]") {
    BENCHMARK("std::forward_list push_front") {
        std::forward_list<int> list;
        for (int i = 0; i < N; ++i)
            list.push_front(i);
        return list.front();
    };

    BENCHMARK("my::forward_list push_front") {
        my::forward_list<int, N> list;
        for (int i = 0; i < N; ++i)
            list.push_front(i);
        return list.front();
    };
}

TEST_CASE("Benchmark forward_list pop_front", "[benchmark]") {
    BENCHMARK("std::forward_list pop_front") {
        std::forward_list<int> list;
        for (int i = 0; i < N; ++i)
            list.push_front(i);

        while (!list.empty()) {
            list.pop_front();
        }
        return list.empty();
    };

    BENCHMARK("my::forward_list pop_front") {
        my::forward_list<int, N> list;
        for (int i = 0; i < N; ++i)
            list.push_front(i);

        while (!list.empty()) {
            list.pop_front();
        }
        return list.empty();
    };
}

TEST_CASE("Benchmark forward_list iteration", "[benchmark]") {
    std::forward_list<int> std_list;
    my::forward_list<int, N> my_list;

    for (int i = 0; i < N; ++i) {
        std_list.push_front(i);
        my_list.push_front(i);
    }

    BENCHMARK("std::forward_list iteration") {
        volatile int sum = 0;
        for (int x : std_list)
            sum += x;
        return sum;
    };

    BENCHMARK("my::forward_list iteration") {
        volatile int sum = 0;
        for (auto it = my_list.begin(); it != my_list.end(); ++it)
            sum += *it;
        return sum;
    };
}