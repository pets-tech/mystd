#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "mystd/some_trees/redblack_tree.hpp"

namespace my::testing {

TEST(RedBlackTreeTest, InsertEraseContains) {
  my::redblack_tree<int, int> t;

  for (size_t i = 1; i <= 3; ++i) {
    t.insert({i, i * 10});
  }

  t.print();
  t.erase(2);
  t.print();
}

TEST(RedBlackTreeTest, InsertEraseSimple) {
  my::redblack_tree<int, int> t;
  t.insert({10, 10});
  t.insert({20, 20});
  t.insert({30, 30});

  EXPECT_EQ(t.root->data.first, 20);
  EXPECT_EQ(t.root->left->data.first, 10);
  EXPECT_EQ(t.root->right->data.first, 30);

  EXPECT_EQ(t.root->color, my::node_colors::BLACK);
  EXPECT_EQ(t.root->left->color, my::node_colors::RED);
  EXPECT_EQ(t.root->right->color, my::node_colors::RED);
}

TEST(RedBlackTreeTest, BalanceRotateLeft) {
  my::redblack_tree<int, int> t;
  t.insert({10, 10});
  t.insert({20, 20});
  t.insert({30, 30});

  EXPECT_EQ(t.root->data.first, 20);
  EXPECT_EQ(t.root->left->data.first, 10);
  EXPECT_EQ(t.root->right->data.first, 30);

  EXPECT_EQ(t.root->color, my::node_colors::BLACK);
  EXPECT_EQ(t.root->left->color, my::node_colors::RED);
  EXPECT_EQ(t.root->right->color, my::node_colors::RED);
}

TEST(RedBlackTreeTest, BalanceRotateRight) {
  my::redblack_tree<int, int> t;
  t.insert({10, 10});
  t.insert({5, 20});
  t.insert({1, 30});

  EXPECT_EQ(t.root->data.first, 5);
  EXPECT_EQ(t.root->left->data.first, 1);
  EXPECT_EQ(t.root->right->data.first, 10);

  EXPECT_EQ(t.root->color, my::node_colors::BLACK);
  EXPECT_EQ(t.root->left->color, my::node_colors::RED);
  EXPECT_EQ(t.root->right->color, my::node_colors::RED);
}

TEST(RedBlackTreeTest, BalanceFromBook) {
  my::redblack_tree<int, int> t;
  t.insert({7, 10});
  t.insert({3, 10});
  t.insert({18, 10});
  t.insert({10, 10});
  t.insert({22, 10});
  t.insert({8, 10});
  t.insert({11, 10});
  t.insert({26, 10});

  EXPECT_EQ(t.root->data.first, 7);
  EXPECT_EQ(t.root->left->data.first, 3);
  EXPECT_EQ(t.root->right->data.first, 18);

  EXPECT_EQ(t.root->color, my::node_colors::BLACK);
  EXPECT_EQ(t.root->left->color, my::node_colors::BLACK);
  EXPECT_EQ(t.root->right->color, my::node_colors::RED);

  EXPECT_EQ(t.root->right->left->data.first, 10);
  EXPECT_EQ(t.root->right->right->data.first, 22);

  EXPECT_EQ(t.root->right->left->color, my::node_colors::BLACK);
  EXPECT_EQ(t.root->right->right->color, my::node_colors::BLACK);

  EXPECT_EQ(t.root->right->left->left->data.first, 8);
  EXPECT_EQ(t.root->right->left->right->data.first, 11);

  EXPECT_EQ(t.root->right->left->left->color, my::node_colors::RED);
  EXPECT_EQ(t.root->right->left->right->color, my::node_colors::RED);

  EXPECT_EQ(t.root->right->right->right->data.first, 26);
  EXPECT_EQ(t.root->right->right->right->color, my::node_colors::RED);
}

}  // namespace my::testing
