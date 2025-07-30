#include <gtest/gtest.h>

#include "mystd/list_linked_nodes.hpp"

namespace my::heapbased::testing {

TEST(ListTest, DefaultInitialization) {
  list<int> a;
  EXPECT_TRUE(a.empty());
  EXPECT_EQ(a.size(), 0);
}

TEST(ListTest, InitializerListConstruction) {
  list<int> a = {10, 20, 30};
  EXPECT_FALSE(a.empty());
  EXPECT_EQ(a.size(), 3);
  EXPECT_EQ(a.front(), 10);
  EXPECT_EQ(a.back(), 30);
}

TEST(ListTest, PushFrontBackAndPopFrontBack) {
  list<int> a;
  a.push_back(1);
  a.push_back(2);
  a.push_front(0);
  EXPECT_EQ(a.front(), 0);
  EXPECT_EQ(a.back(), 2);

  a.pop_front();
  EXPECT_EQ(a.front(), 1);

  a.pop_back();
  EXPECT_EQ(a.back(), 1);

  a.pop_back();
  EXPECT_TRUE(a.empty());
}

TEST(ListTest, ForwardIteratorTraversal) {
  list<int> a = {1, 2, 3, 4};
  int expected = 1;
  for (auto it = a.begin(), ite = a.end(); it != ite; ++it) {
    EXPECT_EQ(*it, expected++);
  }

  auto it = a.end();
  expected = 4;
  while (it != a.begin()) {
    --it;
    EXPECT_EQ(*it, expected--);
  }
}

TEST(ListTest, ReverseIteratorTraversal) {
  list<int> a = {1, 2, 3, 4};
  int expected = 4;
  for (auto it = a.rbegin(), ite = a.rend(); it != ite; ++it) {
    EXPECT_EQ(*it, expected--);
  }
}

TEST(ListTest, InsertBeginMiddleEnd) {
  list<int> a = {1, 3};

  a.insert(a.begin(), 0);  // {0, 1, 3}
  auto it = a.begin();
  ++it;  // it -> 1
  ++it;
  a.insert(it, 2);       // {0, 1, 2, 3} inser before 3
  a.insert(a.end(), 4);  // {0, 1, 2, 3, 4}

  int expected = 0;
  for (auto& val : a) {
    EXPECT_EQ(val, expected++);
  }
}

TEST(ListTest, EraseBeginMiddleEnd) {
  list<int> a = {0, 1, 2, 3, 4};

  a.erase(a.begin());  // erase 0
  auto it = a.begin();
  ++it;         // it -> 2
  a.erase(it);  // erase 2
  it = a.end();
  --it;         // it -> 4
  a.erase(it);  // erase 4

  std::vector<int> expected = {1, 3};
  auto exp_it = expected.begin();
  for (auto& val : a) {
    EXPECT_EQ(val, *exp_it++);
  }
}

TEST(ListTest, CopyConstructorAndAssignment) {
  list<int> a = {1, 2, 3};
  list<int> b = a;  // copy constructor
  list<int> c;
  c = a;  // copy assignment

  EXPECT_EQ(a.front(), b.front());
  EXPECT_EQ(a.front(), c.front());
  EXPECT_EQ(a.back(), b.back());
  EXPECT_EQ(a.back(), c.back());
}

TEST(ListTest, MoveConstructorAndAssignment) {
  list<int> a = {1, 2, 3};
  list<int> b = std::move(a);
  EXPECT_EQ(b.size(), 3);
  EXPECT_TRUE(a.empty());

  list<int> c;
  c = std::move(b);
  EXPECT_EQ(c.size(), 3);
  EXPECT_TRUE(b.empty());
}

TEST(ListTest, ConstIterator) {
  const list<int> a = {5, 6, 7};
  auto it = a.begin();
  EXPECT_EQ(*it, 5);
  ++it;
  EXPECT_EQ(*it, 6);

  auto rit = a.rbegin();
  EXPECT_EQ(*rit, 7);
  ++rit;
  EXPECT_EQ(*rit, 6);
}

}  // namespace my::heapbased::testing
