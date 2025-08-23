#include <gtest/gtest.h>

#include <utility>

#include "mystd/some_trees/bst.hpp"

namespace my::testing {

TEST(BSTTest, InsertAndSearch) {
  binary_search_tree<int, int> bst;

  bst.insert({5, 50});
  bst.insert({3, 30});
  bst.insert({7, 70});

  auto* n1 = bst.search({5, 0});
  ASSERT_NE(n1, nullptr);
  EXPECT_EQ(n1->data.first, 5);
  EXPECT_EQ(n1->data.second, 50);

  auto* n2 = bst.search({3, 0});
  ASSERT_NE(n2, nullptr);
  EXPECT_EQ(n2->data.first, 3);

  auto* n3 = bst.search({7, 0});
  ASSERT_NE(n3, nullptr);
  EXPECT_EQ(n3->data.first, 7);

  auto* n4 = bst.search({42, 0});
  EXPECT_EQ(n4, nullptr);
}

TEST(BSTTest, ParentPointersAreCorrect) {
  binary_search_tree<int, int> bst;

  bst.insert({10, 100});
  bst.insert({5, 50});
  bst.insert({15, 150});

  auto* root = bst.search({10, 0});
  auto* left = bst.search({5, 0});
  auto* right = bst.search({15, 0});

  ASSERT_NE(root, nullptr);
  ASSERT_NE(left, nullptr);
  ASSERT_NE(right, nullptr);

  EXPECT_EQ(left->parent, root);
  EXPECT_EQ(right->parent, root);
  EXPECT_EQ(root->parent, nullptr);
}

TEST(BSTTest, DeleteLeaf) {
  binary_search_tree<int, int> bst;
  bst.insert({5, 50});
  bst.insert({3, 30});
  bst.insert({7, 70});

  bst.erase({3, 0});

  EXPECT_EQ(bst.search({3, 0}), nullptr);
  EXPECT_NE(bst.search({5, 0}), nullptr);
  EXPECT_NE(bst.search({7, 0}), nullptr);
}

TEST(BSTTest, DeleteNodeWithOneChild) {
  binary_search_tree<int, int> bst;
  bst.insert({10, 100});
  bst.insert({5, 50});
  bst.insert({2, 20});

  bst.erase({5, 0});

  EXPECT_EQ(bst.search({5, 0}), nullptr);
  auto* n = bst.search({2, 0});
  ASSERT_NE(n, nullptr);
  EXPECT_EQ(n->parent->data.first, 10);
}

TEST(BSTTest, DeleteNodeWithTwoChildren) {
  binary_search_tree<int, int> bst;
  bst.insert({10, 100});
  bst.insert({5, 50});
  bst.insert({15, 150});
  bst.insert({12, 120});
  bst.insert({18, 180});

  bst.erase({15, 0});

  EXPECT_EQ(bst.search({15, 0}), nullptr);
  EXPECT_NE(bst.search({12, 0}), nullptr);
  EXPECT_NE(bst.search({18, 0}), nullptr);
}

TEST(BSTTest, ClearTree) {
  binary_search_tree<int, int> bst;
  bst.insert({1, 10});
  bst.insert({2, 20});
  bst.insert({3, 30});

  bst.clear();

  EXPECT_EQ(bst.search({1, 0}), nullptr);
  EXPECT_EQ(bst.search({2, 0}), nullptr);
  EXPECT_EQ(bst.search({3, 0}), nullptr);
}

}  // namespace my::testing
