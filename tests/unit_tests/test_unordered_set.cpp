#include <gtest/gtest.h>

#include "mystd/unordered_set.hpp"

namespace my::testing {

TEST(UnorderedSetTest, Initialization) {
  my::unordered_set<int> uset;
  EXPECT_TRUE(uset.empty());
  EXPECT_EQ(uset.size(), 0);
  EXPECT_EQ(uset.bucket_count(), 8);
}

TEST(UnorderedSetTest, InitializerList) {
  my::unordered_set<int> uset = {1, 2, 3, 3, 3, 3};
  EXPECT_FALSE(uset.empty());
  EXPECT_EQ(uset.size(), 3);
  EXPECT_EQ(uset.bucket_count(), 8);
}

TEST(UnorderedSetTest, Find) {
  my::unordered_set<int> uset1 = {1, 2, 3, 3, 3, 3};
  EXPECT_TRUE(uset1.find(1) != uset1.end());
  EXPECT_TRUE(uset1.find(2) != uset1.end());
  EXPECT_TRUE(uset1.find(3) != uset1.end());
  EXPECT_TRUE(uset1.find(4) == uset1.end());
}

TEST(UnorderedSetTest, Erase) {
  my::unordered_set<int> uset1 = {1, 2, 3, 3, 3, 3};

  uset1.erase(1);
  EXPECT_EQ(uset1.size(), 2);
  EXPECT_TRUE(uset1.find(1) == uset1.end());

  uset1.erase(2);
  EXPECT_EQ(uset1.size(), 1);
  EXPECT_TRUE(uset1.find(2) == uset1.end());

  uset1.erase(3);
  EXPECT_EQ(uset1.size(), 0);
  EXPECT_TRUE(uset1.find(3) == uset1.end());

  EXPECT_TRUE(uset1.empty());
}

TEST(UnorderedSetTest, CopyMode) {
  my::unordered_set<int> uset1 = {1, 2, 3, 3, 3, 3};

  my::unordered_set<int> uset2(uset1);
  EXPECT_EQ(uset2.size(), 3);

  my::unordered_set<int> uset3;
  uset3 = uset1;
  EXPECT_EQ(uset3.size(), 3);

  my::unordered_set<int> uset4(std::move(uset1));
  EXPECT_EQ(uset4.size(), 3);
  EXPECT_TRUE(uset1.empty());

  my::unordered_set<int> uset5;
  uset5 = std::move(uset4);
  EXPECT_EQ(uset5.size(), 3);
  EXPECT_TRUE(uset4.empty());
}

TEST(UnorderedSetTest, Iterators) {
  my::unordered_set<int> uset = {0, 1, 2, 3, 4, 5};

  for (auto& v : uset) {
    EXPECT_EQ(*uset.find(v), v);
  }

  auto it = uset.cbegin();
  auto ite = uset.cend();
}

TEST(UnorderedSetTest, Rehashing) {
  my::unordered_set<int> uset(3);

  EXPECT_EQ(uset.bucket_count(), 3);

  for (size_t i = 0; i < 100; ++i) {
    uset.insert(i);
  }

  EXPECT_EQ(uset.bucket_count(), 192);
}

}  // namespace my::testing
