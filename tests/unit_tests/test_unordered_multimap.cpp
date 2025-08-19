#include <gtest/gtest.h>

#include "mystd/unordered_multimap.hpp"

namespace my::testing {

TEST(UnorderedMultiMapTest, Initialization) {
  my::unordered_multimap<char, int> mm;
  EXPECT_TRUE(mm.empty());
  EXPECT_EQ(mm.size(), 0);
}

TEST(UnorderedMultiMapTest, InitializerList) {
  my::unordered_multimap<char, int> mm = {{'a', 1}, {'a', 1}, {'b', 3}};
  EXPECT_FALSE(mm.empty());
  EXPECT_EQ(mm.size(), 3);
}

TEST(UnorderedMultiMapTest, Insert) {
  my::unordered_multimap<char, int> mm = {{'a', 1}, {'a', 1}, {'b', 3}};

  EXPECT_EQ(mm.count('a'), 2);
  EXPECT_EQ(mm.count('b'), 1);

  mm.insert('a', 2);
  mm.insert('b', 4);

  EXPECT_EQ(mm.count('a'), 3);
  EXPECT_EQ(mm.count('b'), 2);
}

TEST(UnorderedMultiMapTest, Erase) {
  my::unordered_multimap<char, int> mm = {{'a', 1}, {'a', 1}, {'b', 3}};

  EXPECT_EQ(mm.count('a'), 2);
  EXPECT_EQ(mm.count('b'), 1);

  mm.erase('a');
  mm.erase('b');

  EXPECT_EQ(mm.count('a'), 1);
  EXPECT_EQ(mm.count('b'), 0);
}

TEST(UnorderedMultiMapTest, Rehash) {
  my::unordered_multimap<char, int> mm(3);

  for (size_t i = 0; i < 100; ++i) {
    mm.insert(i, i * 10);
  }

  EXPECT_EQ(mm.bucket_count(), 192);
}

}  // namespace my::testing
