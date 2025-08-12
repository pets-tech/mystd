#include <gtest/gtest.h>

#include "mystd/deque_cyclicbuffer_based.hpp"

namespace my::cyclicbufferbased {

TEST(DequeTest, PushBackAndFront) {
  deque<int> d;
  d.push_back(1);
  d.push_back(2);
  EXPECT_EQ(d.front(), 1);
  EXPECT_EQ(d.back(), 2);
}

TEST(DequeTest, PushFront) {
  deque<int> d;
  d.push_front(1);
  d.push_front(2);
  EXPECT_EQ(d.front(), 2);
  EXPECT_EQ(d.back(), 1);
}

TEST(DequeTest, PopBack) {
  deque<int> d;
  d.push_back(1);
  d.push_back(2);
  d.pop_back();
  EXPECT_EQ(d.back(), 1);
}

TEST(DequeTest, PopFront) {
  deque<int> d;
  d.push_back(1);
  d.push_back(2);
  d.pop_front();
  EXPECT_EQ(d.front(), 2);
}

TEST(DequeTest, AtOperator) {
  deque<int> d;
  d.push_back(10);
  d.push_back(20);
  EXPECT_EQ(d.at(1), 20);
  EXPECT_THROW(d.at(2), std::out_of_range);
}

TEST(DequeTest, IteratorLoop) {
  deque<int> d;
  for (int i = 0; i < 5; ++i) d.push_back(i);
  int sum = 0;
  for (auto v : d) sum += v;
  EXPECT_EQ(sum, 10);
}

TEST(DequeTest, ReallocateWorks) {
  deque<int> d;
  for (int i = 0; i < 20; ++i) d.push_back(i);
  EXPECT_EQ(d.size(), 20u);
  EXPECT_EQ(d.front(), 0);
  EXPECT_EQ(d.back(), 19);
}

TEST(DequeTest, CopyConstructor) {
  deque<int> d1;
  for (int i = 0; i < 5; ++i) d1.push_back(i);
  deque<int> d2(d1);
  EXPECT_EQ(d2.size(), d1.size());
  for (size_t i = 0; i < d1.size(); ++i) {
    EXPECT_EQ(d1[i], d2[i]);
  }
}

TEST(DequeTest, CopyAssignment) {
  deque<int> d1;
  for (int i = 0; i < 5; ++i) d1.push_back(i);
  deque<int> d2;
  d2 = d1;
  EXPECT_EQ(d2.size(), d1.size());
  for (size_t i = 0; i < d1.size(); ++i) {
    EXPECT_EQ(d1[i], d2[i]);
  }
}

TEST(DequeTest, MoveConstructor) {
  deque<int> d1;
  for (int i = 0; i < 5; ++i) d1.push_back(i);
  deque<int> d2(std::move(d1));
  EXPECT_EQ(d2.size(), 5u);
  EXPECT_TRUE(d1.empty());
}

TEST(DequeTest, MoveAssignment) {
  deque<int> d1;
  for (int i = 0; i < 5; ++i) d1.push_back(i);
  deque<int> d2;
  d2 = std::move(d1);
  EXPECT_EQ(d2.size(), 5u);
  EXPECT_TRUE(d1.empty());
}

TEST(DequeTest, WrapAroundPushPop) {
  deque<int> d;
  for (int i = 0; i < 8; ++i) d.push_back(i);
  for (int i = 0; i < 3; ++i) d.pop_front();
  for (int i = 8; i < 11; ++i) d.push_back(i);
  std::vector<int> result;
  for (auto v : d) result.push_back(v);
  EXPECT_EQ((std::vector<int>{3, 4, 5, 6, 7, 8, 9, 10}), result);
}

TEST(DequeTest, MixedPushFrontBack) {
  deque<int> d;
  d.push_back(1);
  d.push_front(0);
  d.push_back(2);
  d.push_front(-1);
  EXPECT_EQ(d.front(), -1);
  EXPECT_EQ(d.back(), 2);
  std::vector<int> expected{-1, 0, 1, 2};
  EXPECT_TRUE(std::equal(d.begin(), d.end(), expected.begin()));
}

TEST(DequeTest, ReallocateKeepsOrderWithWrap) {
  deque<int> d;
  for (int i = 0; i < 8; ++i) d.push_back(i);
  for (int i = 0; i < 5; ++i) d.pop_front();
  for (int i = 8; i < 20; ++i) d.push_back(i);
  std::vector<int> expected{5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
  EXPECT_TRUE(std::equal(d.begin(), d.end(), expected.begin()));
}

TEST(DequeTest, IteratorValidityAfterCopy) {
  deque<int> d1;
  for (int i = 0; i < 5; ++i) d1.push_back(i);
  auto it = d1.begin();
  deque<int> d2(d1);
  EXPECT_EQ(*it, 0);
  EXPECT_EQ(*d2.begin(), 0);
}

TEST(DequeTest, EmptyOperationsThrow) {
  deque<int> d;
  EXPECT_THROW(d.front(), std::out_of_range);
  EXPECT_THROW(d.back(), std::out_of_range);
  EXPECT_THROW(d.pop_front(), std::out_of_range);
  EXPECT_THROW(d.pop_back(), std::out_of_range);
}

}  // namespace my::cyclicbufferbased
