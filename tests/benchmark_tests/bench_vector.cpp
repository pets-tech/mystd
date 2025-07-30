#include <benchmark/benchmark.h>

#include <array>

#include "mystd/vector.hpp"

static void BM_ArrayFill(benchmark::State& state) {
  my::vector<int> arr;
  for (auto _ : state) {
    arr.fill(42);
    benchmark::DoNotOptimize(arr.data());
  }
}
BENCHMARK(BM_ArrayFill);

static void BM_STDArrayFill(benchmark::State& state) {
  std::vector<int> arr;
  for (auto _ : state) {
    arr.fill(42);
    benchmark::DoNotOptimize(arr.data());
  }
}
BENCHMARK(BM_STDArrayFill);

static void BM_ArrayAccess(benchmark::State& state) {
  my::vector<int> arr;
  arr.fill(1);
  volatile int sum = 0;
  for (auto _ : state) {
    for (size_t i = 0; i < arr.size(); ++i) {
      sum += arr[i];
    }
  }
}
BENCHMARK(BM_ArrayAccess);

static void BM_STDArrayAccess(benchmark::State& state) {
  std::vector<int> arr;
  arr.fill(1);
  volatile int sum = 0;
  for (auto _ : state) {
    for (size_t i = 0; i < arr.size(); ++i) {
      sum += arr[i];
    }
  }
}
BENCHMARK(BM_STDArrayAccess);

static void BM_ArraySwap(benchmark::State& state) {
  my::vector<int> arr1, arr2;
  for (auto _ : state) {
    arr1.swap(arr2);
    benchmark::DoNotOptimize(arr1.data());
  }
}
BENCHMARK(BM_ArraySwap);

static void BM_STDArraySwap(benchmark::State& state) {
  std::array<int> arr1, arr2;
  for (auto _ : state) {
    arr1.swap(arr2);
    benchmark::DoNotOptimize(arr1.data());
  }
}
BENCHMARK(BM_STDArraySwap);

BENCHMARK_MAIN();
