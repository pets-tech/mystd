#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "mystd/some_trees/treap.hpp"

namespace my::testing {

using treap_int = my::treap<int, KeyOfIdentity<int>>;
using treap_pair_int = my::treap<std::pair<int, int>, KeyOfPair<int, int>>;

TEST(TreapTest, InsertAndSize) {
  treap_int t;
  EXPECT_TRUE(t.empty());
  t.insert(5);
  t.insert(2);
  t.insert(8);
  EXPECT_EQ(t.size(), 3u);
  EXPECT_FALSE(t.empty());

  t.insert(8);
  t.insert(8);
  t.insert(8);
  EXPECT_EQ(t.size(), 6u);
  // t.print();
}

TEST(TreapTest, InsertAndSizePair) {
  treap_pair_int t;
  EXPECT_TRUE(t.empty());
  t.insert({5, 50});
  t.insert({2, 20});
  t.insert({8, 80});
  EXPECT_EQ(t.size(), 3u);
  EXPECT_FALSE(t.empty());

  t.insert({8, 80});
  t.insert({8, 80});
  t.insert({8, 80});
  EXPECT_EQ(t.size(), 6u);
}

TEST(TreapTest, Print) {
  treap_int t;
  for (auto i = 0; i < 20; ++i) {
    t.insert(i);
  }
  // t.print();
}

TEST(TreapTest, FindExisting) {
  treap_int t;
  t.insert(10);
  t.insert(20);
  auto it = t.find(10);
  EXPECT_NE(it, t.end());
  EXPECT_EQ(*it, 10);
}

TEST(TreapTest, FindNonExisting) {
  treap_int t;
  t.insert(1);
  t.insert(2);
  auto it = t.find(3);
  EXPECT_EQ(it, t.end());
}

TEST(TreapTest, EraseSingle) {
  treap_int t;
  t.insert(42);
  EXPECT_EQ(t.size(), 1u);
  t.erase(42);
  EXPECT_EQ(t.size(), 0u);
  EXPECT_EQ(t.find(42), t.end());
}

TEST(TreapTest, EraseOneOfMany) {
  treap_int t;
  t.insert(3);
  t.insert(7);
  t.insert(1);
  t.erase(7);
  EXPECT_EQ(t.size(), 2u);
  EXPECT_EQ(t.find(7), t.end());
  EXPECT_NE(t.find(3), t.end());
  EXPECT_NE(t.find(1), t.end());
}

TEST(TreapTest, Clear) {
  treap_int t;
  for (int i = 0; i < 10; ++i) t.insert(i);
  EXPECT_EQ(t.size(), 10u);
  t.clear();
  EXPECT_TRUE(t.empty());
  EXPECT_EQ(t.size(), 0u);
}

TEST(TreapTest, IterationOrder) {
  treap_int t;
  std::vector<int> vals = {5, 2, 8, 1, 7};
  for (int v : vals) t.insert(v);
  // t.print();

  std::vector<int> collected;
  for (auto it = t.begin(); it != t.end(); ++it) collected.push_back(*it);
  std::vector<int> expected = {1, 2, 5, 7, 8};
  EXPECT_EQ(collected, expected);
}

TEST(TreapTest, CopyConstructor) {
  treap_int t;
  t.insert(10);
  t.insert(20);
  treap_int t2 = t;
  EXPECT_EQ(t2.size(), 2u);
  EXPECT_NE(t2.find(10), t2.end());
  EXPECT_NE(t2.find(20), t2.end());
}

TEST(TreapTest, MoveConstructor) {
  treap_int t;
  t.insert(100);
  t.insert(200);
  treap_int t2 = std::move(t);
  EXPECT_EQ(t2.size(), 2u);
  EXPECT_NE(t2.find(100), t2.end());
  EXPECT_NE(t2.find(200), t2.end());
}

TEST(TreapTest, AssignmentCopy) {
  treap_int t1;
  t1.insert(1);
  t1.insert(2);
  treap_int t2;
  t2.insert(10);
  t2 = t1;
  EXPECT_EQ(t2.size(), 2u);
  EXPECT_NE(t2.find(1), t2.end());
  EXPECT_NE(t2.find(2), t2.end());
}

TEST(TreapTest, AssignmentMove) {
  treap_int t1;
  t1.insert(50);
  t1.insert(60);
  treap_int t2;
  t2 = std::move(t1);
  EXPECT_EQ(t2.size(), 2u);
  EXPECT_NE(t2.find(50), t2.end());
  EXPECT_NE(t2.find(60), t2.end());
}

}  // namespace my::testing
