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

// erase recursive

TEST(BSTTest, EraseLeafNode) {
  my::binary_search_tree<int, std::string> t;
  t.insert({5, "five"});
  t.insert({3, "three"});
  t.insert({7, "seven"});
  EXPECT_TRUE(t.contains(3));
  t.erase(3);
  EXPECT_FALSE(t.contains(3));
  EXPECT_TRUE(t.contains(5));
  EXPECT_TRUE(t.contains(7));
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

// erase iterative

TEST(BSTTest, EraseLeafNodeI) {
  my::binary_search_tree<int, std::string> t;
  t.inserti({5, "five"});
  t.inserti({3, "three"});
  t.inserti({7, "seven"});
  EXPECT_TRUE(t.containsi(3));
  t.erasei(3);
  EXPECT_FALSE(t.containsi(3));
  EXPECT_TRUE(t.containsi(5));
  EXPECT_TRUE(t.containsi(7));
}

TEST(BSTTest, EraseNodeWithOneChildI) {
  my::binary_search_tree<int, std::string> t;
  t.inserti({5, "five"});
  t.inserti({3, "three"});
  t.inserti({7, "seven"});
  t.inserti({6, "six"});
  t.inserti({8, "eight"});
  EXPECT_TRUE(t.containsi(7));
  t.erasei(7);
  EXPECT_FALSE(t.containsi(7));
  EXPECT_TRUE(t.containsi(6));
  EXPECT_TRUE(t.containsi(8));
}

TEST(BSTTest, EraseNodeWithTwoChildrenI) {
  my::binary_search_tree<int, std::string> t;
  t.inserti({5, "five"});
  t.inserti({3, "three"});
  t.inserti({7, "seven"});
  t.inserti({6, "six"});
  t.inserti({8, "eight"});
  EXPECT_TRUE(t.containsi(7));
  t.erasei(7);
  EXPECT_FALSE(t.containsi(7));
  EXPECT_TRUE(t.containsi(6));
  EXPECT_TRUE(t.containsi(8));
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
  EXPECT_NO_THROW(t.erasei(10));
}

TEST(BSTTest, InsertDuplicateReplacesValue) {
  my::binary_search_tree<int, std::string> t;
  t.insert({1, "one"});
  t.insert({1, "uno"});
  EXPECT_TRUE(t.contains(1));
  EXPECT_EQ(t.at(1), "uno");

  t.inserti({2, "two"});
  t.inserti({2, "dos"});
  EXPECT_TRUE(t.containsi(2));
  EXPECT_EQ(t.at(2), "dos");
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
  EXPECT_EQ((*(++it)).second, "five");
  EXPECT_EQ((*(--ite)).second, "eight");
  EXPECT_EQ((*(--ite)).second, "seven");

  std::vector<int> answer = {3, 5, 6, 7, 8};
  int i = 0;
  for (const auto& n : t) {
    EXPECT_EQ(n.first, answer[i++]);
  }
}

TEST(BSTTest, IteratorEmptyTree) {
  my::binary_search_tree<int, std::string> t;
  EXPECT_EQ(t.begin(), t.end());
}

TEST(BSTTest, IteratorSingleNode) {
  my::binary_search_tree<int, std::string> t;
  t.insert({1, "one"});
  auto it = t.begin();
  EXPECT_EQ((*it).first, 1);
  ++it;
  EXPECT_EQ(it, t.end());
}

TEST(BSTTest, Swap) {
  my::binary_search_tree<int, std::string> t1;
  t1.insert({1, "one"});
  t1.insert({2, "two"});
  my::binary_search_tree<int, std::string> t2;
  t2.insert({3, "three"});
  t1.swap(t2);
  EXPECT_TRUE(t1.contains(3));
  EXPECT_FALSE(t1.contains(1));
  EXPECT_TRUE(t2.contains(1));
  EXPECT_TRUE(t2.contains(2));
  EXPECT_FALSE(t2.contains(3));
}

TEST(BSTTest, DeleteRootSingleNode) {
  my::binary_search_tree<int, std::string> t;
  t.insert({1, "one"});
  t.erase(1);
  EXPECT_FALSE(t.contains(1));
  EXPECT_EQ(t.begin(), t.end());
  t.inserti({1, "one"});
  t.erasei(1);
  EXPECT_FALSE(t.containsi(1));
  EXPECT_EQ(t.begin(), t.end());
}

TEST(BSTTest, DeleteNonExistentKey) {
  my::binary_search_tree<int, std::string> t;
  t.insert({1, "one"});
  t.erase(2);
  t.erasei(2);
  EXPECT_TRUE(t.contains(1));
  EXPECT_FALSE(t.contains(2));
}

}  // namespace my::testing
