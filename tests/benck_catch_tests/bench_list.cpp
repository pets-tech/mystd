#define CATCH_CONFIG_MAIN

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

#include <list>
#include "mystd/list_fixed_array.hpp"
#include "mystd/list_linked_nodes.hpp"


constexpr int N = 1000;

TEST_CASE("Benchmark list push_front", "[benchmark]") {
    BENCHMARK("std::list push_front") {
        std::list<int> lst;
        for (int i = 0; i < N; ++i)
            lst.push_front(i);
        return lst.front();
    };

    BENCHMARK("my::arraybased::list push_front") {
        my::arraybased::list<int, N> lst;
        for (int i = 0; i < N; ++i)
            lst.push_front(i);
        return lst.front();
    };

    BENCHMARK("my::heapbased::list push_front") {
        my::heapbased::list<int> lst;
        for (int i = 0; i < N; ++i)
            lst.push_front(i);
        return lst.front();
    };
}

TEST_CASE("Benchmark list pop_front", "[benchmark]") {
    BENCHMARK("std::list pop_front") {
        std::list<int> lst;
        for (int i = 0; i < N; ++i)
            lst.push_front(i);

        while (!lst.empty()) {
            lst.pop_front();
        }
        return lst.empty();
    };

    BENCHMARK("my::arraybased::list pop_front") {
        my::arraybased::list<int, N> lst;
        for (int i = 0; i < N; ++i)
            lst.push_front(i);

        while (!lst.empty()) {
            lst.pop_front();
        }
        return lst.empty();
    };
 
    BENCHMARK("my::heapbased::list pop_front") {
        my::heapbased::list<int> lst;
        for (int i = 0; i < N; ++i)
            lst.push_front(i);

        while (!lst.empty()) {
            lst.pop_front();
        }
        return lst.empty();
    };
}

TEST_CASE("Benchmark list iteration", "[benchmark]") {
    std::list<int> std_list;
    my::arraybased::list<int, N> my_list;
    my::heapbased::list<int> my_list_heap;

    for (int i = 0; i < N; ++i) {
        std_list.push_front(i);
        my_list.push_front(i);
        my_list_heap.push_front(i);
    }

    BENCHMARK("std::list iteration") {
        volatile int sum = 0;
        for (int x : std_list)
            sum += x;
        return sum;
    };

    BENCHMARK("my::arraybased::list iteration") {
        volatile int sum = 0;
        for (auto it = my_list.begin(); it != my_list.end(); ++it)
            sum += *it;
        return sum;
    };

    BENCHMARK("my::heapbased::list iteration") {
        volatile int sum = 0;
        for (auto it = my_list_heap.begin(); it != my_list_heap.end(); ++it)
            sum += *it;
        return sum;
    };
}