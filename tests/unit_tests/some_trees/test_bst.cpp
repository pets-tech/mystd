#include <gtest/gtest.h>

#include <utility>

#include "mystd/some_trees/bst.hpp"

namespace my::testing {

TEST(BSTTest, InsertAndSearchRecursive) {
  my::binary_search_tree<int, std::string> t;
  EXPECT_FALSE(t.contains(1));
  t.insert({1, "one"});
  EXPECT_TRUE(t.contains(1));
  EXPECT_FALSE(t.contains(2));
  t.insert({2, "two"});
  EXPECT_TRUE(t.contains(2));
}

TEST(BSTTest, InsertAndSearchIterative) {
  my::binary_search_tree<int, std::string> t;
  EXPECT_FALSE(t.containsi(1));
  t.inserti({1, "one"});
  EXPECT_TRUE(t.containsi(1));
  EXPECT_FALSE(t.containsi(2));
  t.inserti({2, "two"});
  EXPECT_TRUE(t.containsi(2));
}

TEST(BSTTest, EraseLeafNode) {
  my::binary_search_tree<int, std::string> t;
  t.insert({5, "five"});
  t.insert({3, "three"});
  t.insert({7, "seven"});
  EXPECT_TRUE(t.contains(3));
  t.erase(3);
  EXPECT_FALSE(t.contains(3));
}

TEST(BSTTest, EraseNodeWithOneChild) {
  my::binary_search_tree<int, std::string> t;
  t.insert({5, "five"});
  t.insert({3, "three"});
  t.insert({7, "seven"});
  t.insert({6, "six"});
  t.insert({8, "eight"});
  EXPECT_TRUE(t.contains(7));
  t.erase(7);  // childs 6,8
  EXPECT_FALSE(t.contains(7));
  EXPECT_TRUE(t.contains(6));
  EXPECT_TRUE(t.contains(8));
}

TEST(BSTTest, EraseNodeWithTwoChildren) {
  my::binary_search_tree<int, std::string> t;
  t.insert({5, "five"});
  t.insert({3, "three"});
  t.insert({7, "seven"});
  t.insert({6, "six"});
  t.insert({8, "eight"});
  EXPECT_TRUE(t.contains(7));
  t.erase(7);
  EXPECT_FALSE(t.contains(7));
  EXPECT_TRUE(t.contains(6));
  EXPECT_TRUE(t.contains(8));
}

TEST(BSTTest, CopyConstructor) {
  my::binary_search_tree<int, std::string> t1;
  t1.insert({1, "one"});
  t1.insert({2, "two"});

  my::binary_search_tree<int, std::string> t2(t1);
  EXPECT_TRUE(t2.contains(1));
  EXPECT_TRUE(t2.contains(2));
  t1.insert({3, "three"});
  EXPECT_FALSE(t2.contains(3));

  my::binary_search_tree<int, std::string> t3;
  t3 = t2;
  EXPECT_TRUE(t3.contains(1));
  EXPECT_TRUE(t3.contains(2));
  t2.insert({3, "three"});
  EXPECT_FALSE(t3.contains(3));
}

TEST(BSTTest, MoveConstructor) {
  my::binary_search_tree<int, std::string> t1;
  t1.insert({42, "answer"});
  my::binary_search_tree<int, std::string> t2(std::move(t1));
  EXPECT_TRUE(t2.contains(42));
  EXPECT_FALSE(t1.contains(42));

  my::binary_search_tree<int, std::string> t3;
  t3 = std::move(t2);
  EXPECT_TRUE(t3.contains(42));
  EXPECT_FALSE(t2.contains(42));
}

TEST(BSTTest, EmptyTreeErase) {
  my::binary_search_tree<int, std::string> t;
  EXPECT_FALSE(t.contains(10));
  EXPECT_NO_THROW(t.erase(10));
}

TEST(BSTTest, InsertDuplicateReplacesValue) {
  my::binary_search_tree<int, std::string> t;
  t.insert({1, "one"});
  t.insert({1, "uno"});
  EXPECT_TRUE(t.contains(1));
  EXPECT_EQ(t.at(1), "uno");
}

TEST(BSTTest, Iterators) {
  my::binary_search_tree<int, std::string> t;
  t.insert({5, "five"});
  t.insert({3, "three"});
  t.insert({7, "seven"});
  t.insert({6, "six"});
  t.insert({8, "eight"});

  auto it = t.begin();
  auto ite = t.end();

  EXPECT_EQ((*it).second, "three");

  std::vector<int> answer = {3, 5, 6, 7, 8};
  int i = 0;
  for (const auto& n : t) {
    EXPECT_EQ(n.first, answer[i++]);
  }
}

}  // namespace my::testing
