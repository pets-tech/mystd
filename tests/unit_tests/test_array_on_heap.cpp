#include <gtest/gtest.h>
#include "mystd/array_heap.hpp"

namespace my::heapbased {
namespace testing {

class Mock {};

TEST(ArrayTestHeap, DefaultConstructor) {
    array<int, 3> a1;
    array<float, 3> a2;
    array<double, 3> a3;
    array<Mock, 100> a4;
    array<int, 1'000'000'000> a_high;
    EXPECT_EQ(a_high.size(), 1'000'000'000);
}

TEST(ArrayTestHeap, Copy) {
    array<int, 3> a = {1,2,3};
    array<int, 3> b(a);
    array<int, 3> c = a;

    EXPECT_EQ(a[0], b[0]);
    EXPECT_EQ(a[0], c[0]);
    EXPECT_EQ(a[2], b[2]);
    EXPECT_EQ(a[2], c[2]);
}

TEST(ArrayTestHeap, Iterators) {
    array<int, 3> a = {1,2,3};
    int i = 0;
    for (const auto& el : a) {
        EXPECT_EQ(el, a[i++]);
    }
    i = 0;
    for (auto it = a.begin(); it != a.end(); ++it) {
        EXPECT_EQ(*it, a[i++]);
    }
}


// modification
TEST(ArrayTestHeap, Fill) {
    array<int, 13> arr;
    arr.fill(42);
    EXPECT_EQ(arr[0], 42);
    EXPECT_EQ(arr[12], 42);
}

TEST(ArrayTestHeap, Swap) {
    array<int, 2> arr1{1,2};
    array<int, 2> arr2{3,4};
    arr1.swap(arr2);
    EXPECT_EQ(arr1[0], 3);
    EXPECT_EQ(arr2[0], 1);
}

TEST(ArrayTestHeap, Comparations) {
    array<int, 2> arr1 = {1,2};
    array<int, 2> arr2 = {1,2};
    array<int, 2> arr3 = {3,4};
    EXPECT_TRUE(arr1 == arr2);
    EXPECT_FALSE(arr1 == arr3);
    EXPECT_TRUE(arr1 >= arr2);
    EXPECT_TRUE(arr1 <= arr2);
    EXPECT_TRUE(arr1 < arr3);
    EXPECT_TRUE(arr3 > arr1);
}

}
}
