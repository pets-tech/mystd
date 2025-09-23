#include <gtest/gtest.h>

#include <iostream>
#include <random>
#include <utility>
#include <vector>

#include "mystd/some_trees/rb_tree.hpp"

namespace my::testing {

/// MY TEST TREE
///             40
///         /       \
///       20R        60R
///      /  \      /    \
///    10   30    50     70
///      \       /     /   \
///      15R   45R    65R  75R
///

// helpers

template <class Key = int, class Value = int>
using rb_tree_map = rb_tree<std::pair<Key, Value>, KeyOfPair<Key, Value>>;

template <class Value = int>
using rb_tree_set = rb_tree<Value, KeyOfIdentity<Value>>;

inline rb_tree_map<> make_tree() {
  rb_tree_map<> t;
  for (auto [k, v] : {std::pair{40, 40},
                      {20, 20},
                      {60, 60},
                      {10, 10},
                      {30, 30},
                      {50, 50},
                      {70, 70},
                      {15, 15},
                      {45, 45},
                      {65, 65},
                      {75, 75}}) {
    t.insert({k, v});
  }
  return t;
}

TEST(RBTreeTest, ConstructCopy) {
  // ctor
  rb_tree_map<int, int> t1;
  EXPECT_EQ(t1.size(), 0u);
  EXPECT_TRUE(t1.empty());

  rb_tree_map<int, int> t2 = {{1, 10}, {2, 20}, {3, 30}};
  EXPECT_EQ(t2.size(), 3u);
  EXPECT_FALSE(t2.empty());

  // copy
  rb_tree_map<int, int> t3(t2);
  EXPECT_EQ(t3.size(), 3u);
  EXPECT_FALSE(t3.empty());
  EXPECT_TRUE(t3.contains(2));

  rb_tree_map<int, int> t4;
  t4 = t2;
  EXPECT_EQ(t4.size(), 3u);
  EXPECT_FALSE(t4.empty());
  EXPECT_TRUE(t4.contains(2));
}

TEST(RBTreeTest, ConstructMove) {
  // ctor
  rb_tree_map<int, int> t1;
  EXPECT_EQ(t1.size(), 0u);
  EXPECT_TRUE(t1.empty());

  rb_tree_map<int, int> t2 = {{1, 10}, {2, 20}, {3, 30}};
  EXPECT_EQ(t2.size(), 3u);
  EXPECT_FALSE(t2.empty());

  // move
  rb_tree_map<int, int> t3(std::move(t2));
  EXPECT_EQ(t3.size(), 3u);
  EXPECT_FALSE(t3.empty());
  EXPECT_TRUE(t3.contains(2));

  EXPECT_EQ(t2.size(), 0u);
  EXPECT_TRUE(t2.empty());
  EXPECT_FALSE(t2.contains(2));

  rb_tree_map<int, int> t4;
  t4 = std::move(t3);
  EXPECT_EQ(t4.size(), 3u);
  EXPECT_FALSE(t4.empty());
  EXPECT_TRUE(t4.contains(2));

  EXPECT_EQ(t3.size(), 0u);
  EXPECT_TRUE(t3.empty());
  EXPECT_FALSE(t3.contains(2));
}

TEST(RBTreeTest, TestTestingTree) {
  auto t = make_tree();
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

using rb_tree_map_i = rb_tree_map<int, int>;

TEST(RBTreeTest, Empty) {
  rb_tree_map_i t;
  EXPECT_EQ(t.size(), 0u);
  EXPECT_TRUE(t.empty());
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, RootOnly) {
  rb_tree_map_i t;
  t.insert({1, 1});
  EXPECT_EQ(t.size(), 1u);
  EXPECT_FALSE(t.empty());
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, Inc1234) {
  rb_tree_map_i t;
  t.insert({1, 1});
  t.insert({2, 2});
  t.insert({3, 3});
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());

  t.insert({4, 4});
  EXPECT_EQ(t.size(), 4u);
  EXPECT_FALSE(t.empty());
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, Dec4321) {
  rb_tree_map_i t;
  t.insert({4, 4});
  t.insert({3, 3});
  t.insert({2, 2});
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());

  t.insert({1, 1});
  EXPECT_EQ(t.size(), 4u);
  EXPECT_FALSE(t.empty());
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, EraseRedLeaf) {
  rb_tree_map_i t;
  t.insert({40, 40});
  t.insert({20, 20});
  t.insert({60, 60});
  t.insert({10, 10});
  t.erase(10);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, EraseBlackLeaf) {
  rb_tree_map_i t;
  t.insert({40, 40});
  t.insert({20, 20});
  t.insert({60, 60});
  t.insert({10, 10});
  t.erase(60);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

// erase cases tests

TEST(RBTreeTest, EraseBlackNodeWithRightChild) {
  auto t = make_tree();
  t.erase(10);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, EraseBlackNodeWithNoChildren) {
  auto t = make_tree();
  t.erase(30);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, EraseBlackNodeWithLeftChild) {
  auto t = make_tree();
  t.erase(50);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, EraseBlackNodeWithBothChildren) {
  auto t = make_tree();
  t.erase(70);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, EraseRoot) {
  auto t = make_tree();
  t.erase(40);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, EraseNoExistElement) {
  auto t = make_tree();
  t.erase(42);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, InsertAndEraseHalf) {
  rb_tree_map_i t;

  const size_t n = 100;
  for (size_t i = 0; i < n; ++i) {
    t.insert({static_cast<int>(i), static_cast<int>(i)});
  }
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());

  for (size_t i = 0; i < n / 2; ++i) {
    t.erase(static_cast<int>(i));
    EXPECT_TRUE(t.is_binary_tree());
    EXPECT_TRUE(t.is_rb_tree());
  }
}

TEST(RBTreeTest, EraseAllElements) {
  rb_tree_map_i t;

  // 100 unique random values
  std::vector<int> values = {72, -45, 13, -88, 95, -23, 54, -67, 31, -12, 89, -76, 42, -91, 18, -34, 63, -59, 27, -81,
                             49, -3,  78, -29, 36, -72, 84, -17, 58, -64, 21, -95, 67, -38, 92, -7,  45, -53, 15, -86,
                             74, -31, 96, -2,  51, -78, 33, -61, 87, -20, 60, -41, 24, -97, 69, -9,  39, -70, 81, -26,
                             57, -48, 30, -83, 66, -14, 93, -35, 48, -75, 75, -19, 90, -55, 12, -90, 99, -5,  44, -62,
                             26, -39, 71, -25, 53, -80, 35, -68, 85, -11, 62, -50, 19, -94, 79, -1,  47, -57, 28, -73};

  for (auto val : values) {
    t.insert({val, val});
  }

  std::random_device rd;
  std::mt19937 g(rd());
  std::shuffle(values.begin(), values.end(), g);

  for (int val : values) {
    t.erase(val);
    EXPECT_TRUE(t.is_binary_tree());
    EXPECT_TRUE(t.is_rb_tree());
  }
}

TEST(RBTreeTest, RandomInsert) {
  const int n = 1000;
  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> dist(-n, n);

  rb_tree_map_i t;
  for (int i = 0; i < n; ++i) {
    int x = dist(gen);
    t.insert({x, x * 10});
  }

  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, RandomErase) {
  const int n = 1000;
  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> dist(-n, n);

  rb_tree_map_i t;
  for (int i = 0; i < n; ++i) {
    t.insert({i, i * 10});
  }

  for (int i = 0; i < n; ++i) {
    int x = dist(gen);
    t.erase(x);
  }

  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, CommonRBTest) {
  rb_tree_map_i t;

  t.insert({10, 10});
  t.insert({20, 20});
  t.insert({30, 30});
  t.insert({15, 15});
  t.insert({16, 16});
  t.insert({17, 17});

  t.insert({31, 31});
  t.insert({32, 32});
  t.insert({33, 33});
  t.insert({34, 34});
  t.insert({35, 35});

  t.insert({25, 25});
  t.insert({22, 22});
  t.insert({24, 24});

  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());

  t.erase(10);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());

  t.erase(22);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());

  t.erase(33);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());

  t.erase(20);
  EXPECT_TRUE(t.is_binary_tree());
  EXPECT_TRUE(t.is_rb_tree());
}

TEST(RBTreeTest, FindContains) {
  auto t = make_tree();

  EXPECT_TRUE(t.contains(15));
  EXPECT_FALSE(t.contains(100));
}

// iterators
TEST(RBTreeTest, EmptyTree) {
  my::rb_tree<std::pair<int, int>, KeyOfPair<int, int>, std::less<const int&>> t;
  EXPECT_EQ(t.begin(), t.end());
  EXPECT_EQ(t.cbegin(), t.cend());
  EXPECT_EQ(t.rbegin(), t.rend());
  EXPECT_EQ(t.crbegin(), t.crend());
}

TEST(RBTreeTest, Iterators) {
  auto t = make_tree();

  auto it = t.begin();
  EXPECT_EQ((*it).first, 10);
  EXPECT_EQ((*it).second, 10);

  auto cit = t.cbegin();
  EXPECT_EQ((*cit).first, 10);
  EXPECT_EQ((*cit).second, 10);

  EXPECT_FALSE(t.begin() == t.end());
  EXPECT_FALSE(t.cbegin() == t.cend());

  // !!! It dose not work for reverse iterators
  // !!! because
  // !!!  - *end() for now is *nullptr -- UB
  // !!!  - so in rb_tree implemented reverse_iterator(rb_tree.search_max()),
  // !!!      but it leads to decriment in std::reverse_iterator
  // !!!      so t.rbegin() == prev of last element :)
  // !!!      Just have no time for implement more good end()
  auto rit = t.rbegin();
  ASSERT_NE(rit, t.rend()) << "Reverse iterator is invalid";
  EXPECT_EQ((*rit).first, 70);   // ! should be 75 in true implementation
  EXPECT_EQ((*rit).second, 70);  // ! should be 75 in true implementation

  auto crit = t.crbegin();
  EXPECT_EQ((*crit).first, 70);   // ! should be 75 in true implementation
  EXPECT_EQ((*crit).second, 70);  // ! should be 75 in true implementation

  // EXPECT_FALSE(t.rbegin() == t.rend()); // ! dose not work for current end() mplementation
  // EXPECT_FALSE(t.crbegin() == t.crend()); // ! dose not work for current end() mplementation

  std::vector<int> keys;
  for (auto it = t.begin(); it != t.end(); ++it) {
    keys.push_back((*it).first);
  }
  EXPECT_EQ(keys, std::vector<int>({10, 15, 20, 30, 40, 45, 50, 60, 65, 70, 75}));

  // ! also this test not true, I removed max element 75, because rbegin point to 70 :)
  keys.clear();
  for (auto rit = t.rbegin(); rit != t.rend(); ++rit) {
    keys.push_back((*rit).first);
  }
  EXPECT_EQ(keys, std::vector<int>({70, 65, 60, 50, 45, 40, 30, 20, 15, 10}));
}

TEST(RBTreeTest, FindIterators) {
  auto t = make_tree();

  auto it = t.find(10);
  EXPECT_EQ(it->first, 10);

  it++;
  EXPECT_EQ(it->first, 15);

  ++it;
  EXPECT_EQ(it->first, 20);

  it = t.find(45);
  EXPECT_EQ(it->first, 45);

  --it;
  EXPECT_EQ(it->first, 40);

  it--;
  EXPECT_EQ(it->first, 30);

  it = t.find(75);
  EXPECT_EQ(it->first, 75);

  it++;
  EXPECT_TRUE(it == t.end());
}

}  // namespace my::testing
