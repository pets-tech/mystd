#include <gtest/gtest.h>

#include "mystd/unordered_map.hpp"

namespace my::testing {

TEST(UnorderedMapTest, Initialization) {
  my::unordered_map<char, int> hash_table;
  EXPECT_TRUE(hash_table.empty());
  EXPECT_EQ(hash_table.size(), 0);

  my::unordered_map<char, int> m1 = {{'a', 1}, {'b', 2}, {'c', 3}};
  EXPECT_EQ(m1.size(), 3);
  EXPECT_TRUE(m1.find('a'));
  EXPECT_TRUE(m1.find('b'));
  EXPECT_TRUE(m1.find('c'));

  EXPECT_EQ(m1['a'], 1);
  EXPECT_EQ(m1['b'], 2);
  EXPECT_EQ(m1['c'], 3);
}

TEST(UnorderedMapTest, InsertAndFind) {
  my::unordered_map<char, int> m;

  m.insert('a', 1);
  m.insert('b', 2);
  m.insert('c', 3);

  EXPECT_TRUE(m.find('a'));
  EXPECT_TRUE(m.find('b'));
  EXPECT_TRUE(m.find('c'));
  EXPECT_FALSE(m.find('d'));
}

TEST(UnorderedMapTest, Erase) {
  my::unordered_map<char, int> m;

  m.insert('a', 1);
  m.insert('b', 2);
  m.insert('c', 3);

  EXPECT_EQ(m.size(), 3);

  m.erase('a');
  EXPECT_EQ(m.size(), 2);

  m.erase('b');
  EXPECT_EQ(m.size(), 1);
  m.erase('c');
  EXPECT_EQ(m.size(), 0);
}

TEST(UnorderedMapTest, OperatorBrackets) {
  my::unordered_map<char, int> m;

  m['a'] = 1;
  m['a'] += 41;

  EXPECT_EQ(m['a'], 42);
  EXPECT_EQ(m['b'], 0);

  m['b'] = 2;
  EXPECT_EQ(m['b'], 2);
}

TEST(UnorderedMapTest, Rehash) {
  my::unordered_map<int, int> m(3);

  for (size_t i = 0; i < 100; ++i) {
    m.insert(i, i * 10);
  }

  for (size_t i = 0; i < 100; ++i) {
    EXPECT_TRUE(m.find(i));
    EXPECT_EQ(m[i], i * 10);
  }
}

struct BadHash {
  size_t operator()(int) const { return 0; }
};

TEST(UnorderedMapTest, Collision) {
  my::unordered_map<int, int, BadHash> m(3);

  for (size_t i = 0; i < 100; ++i) {
    m.insert(i, i * 10);
  }

  m[10] = 42;
  m.erase(0);

  for (size_t i = 1; i < 100; ++i) {
    EXPECT_TRUE(m.find(i));

    if (i == 10) {
      EXPECT_EQ(m[i], 42);
      continue;
    }

    EXPECT_EQ(m[i], i * 10);
  }
}

struct BadHashInt {
  size_t operator()(int x) const { return x % 5; }
};

TEST(UnorderedMapTest, Iterator) {
  my::unordered_map<int, int> m1;

  for (size_t i = 0; i < 100; ++i) {
    m1[i] = i * 10;
  }

  auto it = m1.begin();
  auto ite = m1.end();
  for (; it != ite; ++it) {
    EXPECT_EQ(it->second, it->first * 10);
  }

  for (const auto& kv : m1) {
    EXPECT_EQ(kv.second, kv.first * 10);
  }
}

}  // namespace my::testing
