#include <gtest/gtest.h>
#include "mystd/array_stack.hpp"

namespace my::stackbased {
namespace testing {

// construction

TEST(ArrayTestStack, DefaultConstructor) {
    array<int, 5> arr;
    EXPECT_EQ(arr.size(), 5);   // size init
    EXPECT_EQ(arr[0], 0);       // value init
}

TEST(ArrayTestStack, CopyConstructor) {
    array<int, 3> arr1{1,2,3};
    array<int, 3> arr2{arr1};
    EXPECT_EQ(arr1[0] , arr2[0]);
    EXPECT_EQ(arr1[2] , arr2[2]);
}

TEST(ArrayTestStack, InitializerList) {
    array<int, 3> arr = {1,2,3};
    EXPECT_EQ(arr[0], 1);
    EXPECT_EQ(arr[2], 3);

    array<int, 5> arr2 = {1,2,3};
    EXPECT_EQ(arr2[4], 0);
}

TEST(ArrayTestStack, OutOfRange) {
    EXPECT_THROW((my::stackbased::array<int, 2>{1,2,3}), std::out_of_range);
}

// access
TEST(ArrayTestStack, At) {
    array<int, 3> arr = {1,2,3};
    EXPECT_EQ(arr.at(2), 3);
    EXPECT_THROW((arr.at(3)), std::out_of_range);
}

TEST(ArrayTestStack, FrontAndBack) {
    array<int, 3> arr = {1,2,3};
    EXPECT_EQ(arr.front(), 1);
    EXPECT_EQ(arr.back(), 3);
}

TEST(ArrayTestStack, Iterators) {
    array<int, 3> arr = {1,2,3};

    int sum = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 6);

    sum = 0;
    for (const auto el : arr) {
        sum += el;
    }
    EXPECT_EQ(sum, 6);
}

// modification
TEST(ArrayTestStack, Fill) {
    array<int, 13> arr;
    arr.fill(42);
    EXPECT_EQ(arr[0], 42);
    EXPECT_EQ(arr[12], 42);
}

TEST(ArrayTestStack, Swap) {
    array<int, 2> arr1{1,2};
    array<int, 2> arr2{3,4};
    arr1.swap(arr2);
    EXPECT_EQ(arr1[0], 3);
    EXPECT_EQ(arr2[0], 1);
}

TEST(ArrayTestStack, Comparations) {
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
