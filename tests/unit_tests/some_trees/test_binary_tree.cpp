#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "mystd/some_trees/binary_tree.hpp"

namespace my::testing {

my::binary_tree<int> create_tree() {
  //       8
  //    /     \
    //   6       9
  //  / \       \
    // 5   7       11
  //            /  \
    //           10  12

  my::binary_tree<int> t;
  t.insert(8);
  t.insert(6);
  t.insert(7);
  t.insert(5);
  t.insert(9);
  t.insert(11);
  t.insert(10);
  t.insert(12);
  return t;
}

TEST(BinaryTreeTest, CopyMove) {
  my::binary_tree<int> t = create_tree();

  // copy
  my::binary_tree<int> tmp1(t);
  my::binary_tree<int> tmp2;
  tmp2 = t;

  // move
  my::binary_tree<int> tmp3(std::move(t));
  my::binary_tree<int> tmp4;
  tmp4 = std::move(tmp3);
}

TEST(BinaryTreeTest, Inorder) {
  my::binary_tree<int> t = create_tree();

  auto vec = t.traversal(my::TraversalType::INORDER);

  std::vector<int> answer = {5, 6, 7, 8, 9, 10, 11, 12};
  for (size_t i = 0; answer.size() < 8; ++i) {
    EXPECT_EQ(vec[i], answer[i]);
  }

  t.insert(4);   // left child
  t.insert(13);  // right child
  auto vec_ext = t.traversal(my::TraversalType::INORDER);

  std::vector<int> answer_ext = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
  for (size_t i = 0; i < answer_ext.size(); ++i) {
    EXPECT_EQ(vec_ext[i], answer_ext[i]);
  }
}

TEST(BinaryTreeTest, Preorder) {
  my::binary_tree<int> t = create_tree();

  auto vec = t.traversal(my::TraversalType::PREORDER);

  std::vector<int> answer = {8, 6, 5, 7, 9, 11, 10, 12};
  for (size_t i = 0; i < answer.size(); ++i) {
    EXPECT_EQ(vec[i], answer[i]);
  }
}

TEST(BinaryTreeTest, Postorder) {
  my::binary_tree<int> t = create_tree();

  auto vec = t.traversal(my::TraversalType::POSTORDER);

  std::vector<int> answer = {5, 7, 6, 10, 12, 11, 9, 8};
  for (size_t i = 0; i < answer.size(); ++i) {
    EXPECT_EQ(vec[i], answer[i]);
  }
}

TEST(BinaryTreeTest, Search) {
  my::binary_tree<int> t = create_tree();

  EXPECT_TRUE(t.search(5));
  EXPECT_TRUE(t.search(12));
  EXPECT_TRUE(t.search(8));
  EXPECT_FALSE(t.search(42));
}

TEST(BinaryTreeTest, FindMin) {
  my::binary_tree<int> t = create_tree();

  EXPECT_EQ(t.findmin(), 5);
}

TEST(BinaryTreeTest, Remove) {
  my::binary_tree<int> t = create_tree();

  t.insert(4);   // left child
  t.insert(13);  // right child

  // current tree is

  //              8
  //           /     \
    //          6       9
  //         / \       \
    //        5   7       11
  //      /            /  \
    //     4            10  12
  //                        \
    //                         13

  // case with no children
  t.remove(7);

  // case with left children
  t.remove(5);

  // case with right children
  t.remove(12);

  // case with both children
  t.remove(8);

  // new current tree

  //              9
  //           /     \
    //          6       11
  //         /       / \
    //        4       10 13
  //

  auto vec = t.traversal(my::TraversalType::INORDER);
  std::vector<int> answer = {4, 6, 9, 10, 11, 13};
  for (size_t i = 0; i < answer.size(); ++i) {
    EXPECT_EQ(vec[i], answer[i]);
  }
}

TEST(BinaryTreeTest, Edges) {
  // empty tree
  my::binary_tree<int> t1;
  EXPECT_FALSE(t1.search(42));
  EXPECT_EQ(t1.traversal(my::TraversalType::INORDER).size(), 0);
  t1.remove(42);
  EXPECT_EQ(t1.findmin(), std::nullopt);

  // root only
  my::binary_tree<int> t2;
  t2.insert(42);
  EXPECT_TRUE(t2.search(42));
  EXPECT_EQ(t2.traversal(my::TraversalType::INORDER), std::vector<int>({42}));
  t2.remove(42);
  EXPECT_EQ(t2.traversal(my::TraversalType::INORDER).size(), 0);
}

}  // namespace my::testing
