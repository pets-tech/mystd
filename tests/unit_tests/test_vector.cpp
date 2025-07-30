#include <gtest/gtest.h>

#include "mystd/vector.hpp"

namespace my::testing {

class Mock {};

TEST(VectorTest, DefaultConstructor) {
  vector<int, 3> a1;
  vector<float, 3> a2;
  vector<double, 3> a3;
  vector<Mock, 100> a4;
  vector<int, 1'000'000'000> a_high;
  EXPECT_EQ(a_high.capacity(), 1'000'000'000);
}

TEST(VectorTest, Copy) {
  vector<int, 3> a = {1, 2, 3};
  vector<int, 3> b(a);
  vector<int, 3> c = a;

  EXPECT_EQ(a[0], b[0]);
  EXPECT_EQ(a[0], c[0]);
  EXPECT_EQ(a[2], b[2]);
  EXPECT_EQ(a[2], c[2]);
}

TEST(VectorTest, Iterators) {
  vector<int, 3> a = {1, 2, 3};
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
TEST(VectorTest, Fill) {
  vector<int, 13> arr;
  arr.fill(42);
  EXPECT_EQ(arr[0], 42);
  EXPECT_EQ(arr[12], 42);
}

TEST(VectorTest, Swap) {
  vector<int, 2> arr1{1, 2};
  vector<int, 2> arr2{3, 4};
  arr1.swap(arr2);
  EXPECT_EQ(arr1[0], 3);
  EXPECT_EQ(arr1[1], 4);
  EXPECT_EQ(arr2[0], 1);
  EXPECT_EQ(arr2[1], 2);
}

TEST(VectorTest, Comparations) {
  vector<int> arr1 = {1, 2};
  vector<int> arr2 = {1, 2};
  vector<int> arr3 = {3, 4};
  EXPECT_TRUE(arr1 == arr2);
  EXPECT_FALSE(arr1 == arr3);
  EXPECT_TRUE(arr1 >= arr2);
  EXPECT_TRUE(arr1 <= arr2);
  EXPECT_TRUE(arr1 < arr3);
  EXPECT_TRUE(arr3 > arr1);
}

}  // namespace my::testing
