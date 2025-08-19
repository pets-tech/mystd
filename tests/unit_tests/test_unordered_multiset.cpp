#include <gtest/gtest.h>

#include "mystd/unordered_multiset.hpp"

namespace my::testing {

TEST(UnorderedMultiSetTest, Initialization) {
  my::unordered_multiset<int> ms;
  EXPECT_TRUE(ms.empty());
  EXPECT_EQ(ms.size(), 0);
}

TEST(UnorderedMultiSetTest, InitializerList) {
  my::unordered_multiset<int> ms = {1, 1, 2, 3};
  EXPECT_FALSE(ms.empty());
  EXPECT_EQ(ms.size(), 4);
  EXPECT_EQ(ms.count(1), 2);
  EXPECT_EQ(ms.count(2), 1);
  EXPECT_EQ(ms.count(3), 1);
}

TEST(UnorderedMultiSetTest, Insert) {
  my::unordered_multiset<int> ms = {1, 2, 2};

  EXPECT_EQ(ms.count(1), 1);
  EXPECT_EQ(ms.count(2), 2);

  ms.insert(1);
  ms.insert(2);
  ms.insert(3);

  EXPECT_EQ(ms.count(1), 2);
  EXPECT_EQ(ms.count(2), 3);
  EXPECT_EQ(ms.count(3), 1);
}

TEST(UnorderedMultiSetTest, Erase) {
  my::unordered_multiset<int> ms = {1, 1, 2, 3};

  EXPECT_EQ(ms.count(1), 2);
  EXPECT_EQ(ms.count(2), 1);
  EXPECT_EQ(ms.count(3), 1);

  ms.erase(1);
  EXPECT_EQ(ms.count(1), 1);

  ms.erase(2);
  EXPECT_EQ(ms.count(2), 0);

  ms.erase(3);
  EXPECT_EQ(ms.count(3), 0);
}

TEST(UnorderedMultiSetTest, Rehash) {
  my::unordered_multiset<int> ms(3);

  for (int i = 0; i < 100; ++i) {
    ms.insert(i);
  }

  EXPECT_EQ(ms.bucket_count(), 192);
}

}  // namespace my::testing
