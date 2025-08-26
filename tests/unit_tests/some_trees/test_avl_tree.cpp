#include <gtest/gtest.h>

#include <cmath>
#include <iostream>

#include "mystd/some_trees/avl_tree.hpp"

namespace my::testing {

TEST(AVLTreeTest, InsertAndContains) {
  avl_tree<int, std::string> tree;

  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});
  tree.insert({30, "thirty"});

  EXPECT_TRUE(tree.contains(10));
  EXPECT_TRUE(tree.contains(20));
  EXPECT_TRUE(tree.contains(30));
  EXPECT_FALSE(tree.contains(40));
}

TEST(AVLTreeTest, Erase) {
  avl_tree<int, std::string> tree;

  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});
  tree.insert({30, "thirty"});
  tree.insert({40, "forty"});

  EXPECT_TRUE(tree.contains(20));
  tree.erase(20);
  EXPECT_FALSE(tree.contains(20));

  EXPECT_TRUE(tree.contains(30));
  tree.erase(30);
  EXPECT_FALSE(tree.contains(30));
}

TEST(AVLTreeTest, RebalanceAfterInsert) {
  avl_tree<int, std::string> tree;

  tree.insert({30, "thirty"});
  tree.insert({20, "twenty"});
  tree.insert({10, "ten"});

  EXPECT_TRUE(tree.contains(10));
  EXPECT_TRUE(tree.contains(20));
  EXPECT_TRUE(tree.contains(30));
}

TEST(AVLTreeTest, PrintTreeToConsole) {
  avl_tree<int, std::string> tree;

  tree.insert({10, "ten"});
  tree.insert({20, "twenty"});
  tree.insert({30, "thirty"});
  tree.insert({25, "twenty five"});
  tree.insert({5, "five"});

  tree.printTree();
}

TEST(AVLTreeTest, VizualTest) {
  avl_tree<int, int> tree;

  const int N = 20;
  for (int i = 1; i <= N; i++) {
    tree.insert({i, i});
  }

  // int height = tree.root->height;
  // int logN = static_cast<int>(std::log2(N)) + 2;
  // ASSERT_LE(height, logN);

  tree.printTree();

  tree.erase(4);
  tree.erase(8);
  tree.erase(16);
  tree.erase(20);
  tree.erase(9);
  tree.erase(3);
  tree.erase(2);
  tree.erase(1);
  tree.erase(19);
  tree.erase(18);

  std::cout << "**\n";
  tree.printTree();
}

}  // namespace my::testing
