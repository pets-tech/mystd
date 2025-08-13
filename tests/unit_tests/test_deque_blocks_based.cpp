#include <gtest/gtest.h>

#include "mystd/deque_blocks_based.hpp"

namespace my::blocksbased::testing {

template <std::size_t BlockSize>
void runAtScenario() {
  deque<int, BlockSize> d;
  d.push_back(1);
  d.push_back(2);
  d.push_back(3);

  EXPECT_EQ(d.at(0), 1);
  EXPECT_EQ(d.at(2), 3);

  d.clear();

  d.push_front(1);
  d.push_front(2);
  d.push_front(3);

  EXPECT_EQ(d.at(0), 3);
  EXPECT_EQ(d.at(2), 1);
}

template <std::size_t BlockSize, std::size_t N>
void runPushPopScenario() {
  deque<int, BlockSize> d;

  for (size_t i = 0; i < N; ++i) {
    d.push_back(static_cast<int>(i));
  }
  EXPECT_EQ(d.size(), N);
  EXPECT_FALSE(d.empty());
  EXPECT_EQ(d.front(), 0);
  EXPECT_EQ(d.back(), static_cast<int>(N - 1));

  for (size_t i = 0; i < N; ++i) {
    d.push_front(-static_cast<int>(i));
  }
  EXPECT_EQ(d.size(), 2 * N);
  EXPECT_EQ(d.front(), -static_cast<int>(N - 1));
  EXPECT_EQ(d.back(), static_cast<int>(N - 1));

  for (size_t i = 0; i < 2 * N; ++i) {
    d.pop_front();
  }
  EXPECT_TRUE(d.empty());
  EXPECT_EQ(d.size(), 0);
}

TEST(DequeBlocksTest, Initialization) {
  deque<int> d;
  EXPECT_EQ(d.size(), 0);
  EXPECT_TRUE(d.empty());

  deque<int> d1 = {1, 2, 3};
  EXPECT_EQ(d1.size(), 3);
  EXPECT_EQ(d1.front(), 1);
  EXPECT_EQ(d1.back(), 3);
}

TEST(DequeBlocksTest, Const) {
  const deque<int> d1 = {1, 2, 3};
  EXPECT_EQ(d1.front(), 1);
  EXPECT_EQ(d1.back(), 3);
}

TEST(DequeBlocksTest, Copy) {
  deque<int> d1 = {1, 2, 3};
  deque<int> d2(d1);
  deque<int> d3;
  d3 = d2;

  EXPECT_EQ(d1.size(), d2.size());
  EXPECT_EQ(d1.size(), d3.size());

  EXPECT_EQ(d1.front(), d2.front());
  EXPECT_EQ(d1.front(), d3.front());

  EXPECT_EQ(d1.back(), d2.back());
  EXPECT_EQ(d1.back(), d3.back());
}

TEST(DequeBlocksTest, Move) {
  deque<int> d1 = {1, 2, 3};

  deque<int> d2(std::move(d1));
  EXPECT_TRUE(d1.empty());
  EXPECT_EQ(d2.size(), 3);
  EXPECT_EQ(d2.front(), 1);
  EXPECT_EQ(d2.back(), 3);

  deque<int> d3;
  d3 = std::move(d2);
  EXPECT_TRUE(d2.empty());
  EXPECT_EQ(d3.size(), 3);
  EXPECT_EQ(d3.front(), 1);
  EXPECT_EQ(d3.back(), 3);
}

TEST(DequeBlocksTest, PushPop) {
  runPushPopScenario<1, 3>();
  runPushPopScenario<2, 5>();
  runPushPopScenario<4, 20>();
  runPushPopScenario<32, 130>();
}

TEST(DequeBlocksTest, At) {
  runAtScenario<1>();
  runAtScenario<2>();
  runAtScenario<3>();
  runAtScenario<4>();
}

}  // namespace my::blocksbased::testing
