#include <gtest/gtest.h>

#include <iostream>
#include <random>
#include <vector>

#include "mystd/some_trees/redblack_tree.hpp"

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
/// Delete
///
/// case 1
///   no left
///     erase(10) leads to
///
///             40
///         /       \
///       20        60R
///      /  \      /    \
///    15R  30R   50     70
///              /     /   \
///            45R   65R   75R
///
///   no both
///     erase(30)
///
///             40
///         /       \
///       20        60R
///      /         /    \
///    15R       50     70
///              /     /   \
///            45R   65R   75R
///
/// case 2
///   no right
///   erase(50)
///             40
///         /       \
///       20        60R
///      /         /    \
///    15R       45     70
///                    /   \
///                  65R   75R
/// case 3
///   both
///   erase(70)
///             40
///         /       \
///       20        60R
///      /         /    \
///    15R       45     75
///                    /
///                  65R
///
///   erase(40)
///             45
///         /       \
///       20        60R
///      /             \
///    15R              75
///                    /
///                  65R
///

template <typename Key, typename Value>
my::redblack_tree<Key, Value> make_tree() {
  my::redblack_tree<Key, Value> t;
  t.insert({40, 40});
  t.insert({20, 20});
  t.insert({60, 60});
  t.insert({10, 10});
  t.insert({30, 30});
  t.insert({50, 50});
  t.insert({70, 70});
  t.insert({15, 15});
  t.insert({45, 45});
  t.insert({65, 65});
  t.insert({75, 75});
  return t;
}

TEST(RedBlackTreeTest, TestTestingTree) {
  auto t = make_tree<int, int>();
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

// trivial tests

TEST(RedBlackTreeTest, Empty) {
  my::redblack_tree<int, int> t;
  EXPECT_EQ(t.size(), 0);
  EXPECT_TRUE(t.empty());
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, RootOnly) {
  my::redblack_tree<int, int> t;
  t.insert({1, 1});
  EXPECT_EQ(t.size(), 1);
  EXPECT_FALSE(t.empty());
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, Inc1234) {
  my::redblack_tree<int, int> t;
  t.insert({1, 1});
  t.insert({2, 2});
  t.insert({3, 3});
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
  t.insert({4, 4});
  EXPECT_EQ(t.size(), 4);
  EXPECT_FALSE(t.empty());
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, Dec4321) {
  my::redblack_tree<int, int> t;
  t.insert({4, 4});
  t.insert({3, 3});
  t.insert({2, 2});
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
  t.insert({1, 1});
  EXPECT_EQ(t.size(), 4);
  EXPECT_FALSE(t.empty());
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, EraseRedLeaf) {
  my::redblack_tree<int, int> t;
  t.insert({40, 40});
  t.insert({20, 20});
  t.insert({60, 60});
  t.insert({10, 10});
  t.erase(10);
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, EraseBlackLeaf) {
  my::redblack_tree<int, int> t;
  t.insert({40, 40});
  t.insert({20, 20});
  t.insert({60, 60});
  t.insert({10, 10});
  t.erase(60);
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

// erase cases tests

TEST(RedBlackTreeTest, EraseBlackNodeWithRightChild) {
  auto t = make_tree<int, int>();
  t.erase(10);
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, EraseBlackNodeWithNoChildren) {
  auto t = make_tree<int, int>();
  t.erase(30);
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, EraseBlackNodeWithLeftChild) {
  auto t = make_tree<int, int>();
  t.erase(50);
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, EraseBlackNodeWithBothChildren) {
  auto t = make_tree<int, int>();
  t.erase(70);
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, EraseRoot) {
  auto t = make_tree<int, int>();
  t.erase(40);
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, EraseNoExistElement) {
  auto t = make_tree<int, int>();
  t.erase(42);
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

// TEST(RedBlackTreeTest, TestBalancingInc) {
//   my::redblack_tree<int, int> t;

//   for (size_t i = 0; i < 6; ++i) {
//     t.insert({i, i});
//   }
//   EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());

//   for (size_t i = 0; i < 3; ++i) {
//     t.erase(i);
//     EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
//   }
// }

// TEST(RedBlackTreeTest, TestBalancingDec) {
//   my::redblack_tree<int, int> t;

//   for (size_t i = 6; i > 0; --i) {
//     t.insert({i, i});
//   }
//   EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());

//   for (size_t i = 3; i > 0; --i) {
//     t.erase(i);
//     std::
//     EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
//   }
// }

TEST(RedBlackTreeTest, InsertAndEraseHalf) {
  my::redblack_tree<int, int> t;

  size_t n = 100;
  for (size_t i = 0; i < n; ++i) {
    t.insert({i, i});
  }
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());

  for (size_t i = 0; i < n / 2; ++i) {
    t.erase(i);
    EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
  }
}

TEST(RedBlackTreeTest, EraseAllElements) {
  my::redblack_tree<int, int> t;

  // 100 unique random numbers between -100 and 100
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
    EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
  }
}

TEST(RedBlackTreeTest, RandomInsert) {
  int n = 1000;

  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> dist(-1 * n, n);

  my::redblack_tree<int, int> t;

  for (size_t i = 0; i < n; ++i) {
    int x = dist(gen);
    t.insert({x, x * 10});
  }

  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, RandomErase) {
  int n = 1000;

  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> dist(-1 * n, n);

  my::redblack_tree<int, int> t;

  for (size_t i = 0; i < n; ++i) {
    t.insert({i, i * 10});
  }

  for (size_t i = 0; i < n; ++i) {
    int x = dist(gen);
    t.erase(x);
  }

  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

TEST(RedBlackTreeTest, CommontRBTest) {
  // rb tree invariants:
  // 1. node -- red || black
  // 2. root -- black
  // 3. leafs -- black
  // 4. red node can't have red child
  // 5. black-height eq to any leafs

  my::redblack_tree<int, int> t;

  t.insert({10, 10});  //  invariant 2 -> recolor
  t.insert({20, 20});  //
  t.insert({30, 30});  //  invariant 4 -> uncle black -> line case -> rotate grandparent + recolor
  t.insert({15, 15});  //  invariant 4 -> uncle red -> recolor
  t.insert({16, 16});  //  invariant 4 -> uncle black ->
                       //          -> triangle (zig-zag) case ->
                       //          -> rotate parent -> line case ->
                       //          -> rotate grandparent + recolor
  t.insert({17, 17});  // invariant 4 -> uncle red -> recolor

  t.insert({31, 31});  //
  t.insert({32, 32});  // invariant 4 -> uncle black -> rotate grandpa + recolor
  t.insert({33, 33});  // invariant 4 -> uncle red -> recolor
  t.insert({34, 34});  // invariant 4 -> uncle black -> rotate grandpa + recolor
  t.insert({35, 35});  // invariant 4 -> uncle red -> many recolor up to root

  // symmetry cases
  t.insert({25, 25});  //
  t.insert({22, 22});  // invariant 4 -> uncle black -> rotate grandpa + recolor
  t.insert({24, 24});  // invariant 4 -> uncle black -> zigzeg case ->  rotate parent
                       // -> rotate grandpa + recolor

  //              20(B)
  //        /               \
  //     15(B)              31(B)
  //    /    \          /          \
  //  10(B) 16(B)      25(R)       33(R)
  //          \       /   \        /   \
  //          17(R) 22(B) 30(B)  32(B) 34(B)
  //                  \                   \
  //                   24(R)             35(R)
  //

  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());

  t.erase(10);  // node has no children ->  black sibling -> distant nephew red -> rotate parent + recolor
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());

  t.erase(22);  //  right child -> recolor
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());

  t.erase(33);  //  node is red and has both child
                // -> transplant min from right subtree
                // -> black sibling with red distant nephew
                // -> rotate parent + recolor

  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());

  t.erase(20);  //  erase root
  EXPECT_TRUE(t.is_binary_tree() && t.is_rb_tree());
}

}  // namespace my::testing
