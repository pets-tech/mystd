#include <gtest/gtest.h>

#include <mystd/hashtable.hpp>
#include <utility>

namespace my::testing {

template <class Key>
struct Identity {
  const Key& operator()(const Key& k) const noexcept { return k; }
};

template <class Key, class Value>
struct KeyOfValue {
  const Key& operator()(const std::pair<const Key, Value>& v) const noexcept { return v.first; }
};

TEST(HashTableTest, Initialization) {
  my::hashtable<int, int, Identity<int>> hset;
  hset.insert(1);
  hset.insert(1);
  hset.insert(1);
  hset.insert(1);
  EXPECT_EQ(hset.size(), 1);

  hset.insert(2);
  EXPECT_EQ(hset.size(), 2);

  my::hashtable<std::pair<char, int>, int, KeyOfValue<char, int>> hmap;
  hmap.insert(std::pair<char, int>('a', 10));
  hmap.insert(std::pair<char, int>('a', 10));
  hmap.insert(std::pair<char, int>('a', 10));
  hmap.insert(std::pair<char, int>('a', 10));
  EXPECT_EQ(hmap.size(), 1);

  hmap.insert(std::pair<char, int>('b', 20));
  EXPECT_EQ(hmap.size(), 2);
}

TEST(HashTableTest, Iterators) {
  my::hashtable<int, int, Identity<int>> hset;
  my::hashtable<std::pair<char, int>, int, KeyOfValue<char, int>> hmap;

  for (size_t i = 0; i < 100; ++i) {
    hset.insert(i);
    hmap.insert({i, i * 10});
  }

  auto it_set = hset.begin();
  auto ite_set = hset.end();
  for (; it_set != ite_set; ++it_set) {
    // EXPECT_TRUE(*it_set);
  }

  auto it_map = hmap.begin();
  auto ite_map = hmap.end();

  while (it_map != ite_map) {
    EXPECT_EQ(it_map->second, it_map->first * 10);
    ++it_map;
  }

  auto cit_map = hmap.cbegin();
  auto cite_map = hmap.cend();
}

TEST(HashTableTest, Find) {
  my::hashtable<int, int, Identity<int>> hset;

  EXPECT_EQ(hset.find(42), hset.end());

  my::hashtable<std::pair<char, int>, int, KeyOfValue<char, int>> hmap;

  EXPECT_EQ(hmap.find('a'), hmap.end());

  for (size_t i = 0; i < 100; ++i) {
    hset.insert(i);
    hmap.insert({i, i * 10});
  }

  for (size_t i = 0; i < 100; ++i) {
    EXPECT_EQ(*hset.find(i), i);
  }

  for (size_t i = 0; i < 100; ++i) {
    EXPECT_EQ((*hmap.find(i)).second, i * 10);
  }
}

TEST(HashTableTest, CopyMoveCtors) {
  using hashmap = my::hashtable<std::pair<char, int>, int, KeyOfValue<char, int>>;
  hashmap hmap1;

  EXPECT_TRUE(hmap1.empty());

  hmap1.insert({'a', 1});
  hmap1.insert({'b', 2});

  hashmap hmap2(hmap1);
  hashmap hmap3;
  hmap3 = hmap1;

  EXPECT_EQ((*hmap1.find('a')).second, 1);
  EXPECT_EQ((*hmap1.find('b')).second, 2);

  (*hmap1.find('a')).second = 3;
  (*hmap1.find('b')).second = 4;

  EXPECT_EQ((*hmap2.find('a')).second, 1);
  EXPECT_EQ((*hmap2.find('b')).second, 2);

  EXPECT_EQ((*hmap3.find('a')).second, 1);
  EXPECT_EQ((*hmap3.find('b')).second, 2);

  hashmap hmap4(std::move(hmap1));

  EXPECT_TRUE(hmap1.empty());

  EXPECT_EQ((*hmap4.find('a')).second, 3);
  EXPECT_EQ((*hmap4.find('b')).second, 4);

  hashmap hmap5;
  hmap5 = std::move(hmap4);

  EXPECT_TRUE(hmap4.empty());

  EXPECT_EQ((*hmap5.find('a')).second, 3);
  EXPECT_EQ((*hmap5.find('b')).second, 4);
}

}  // namespace my::testing
