#include <gtest/gtest.h>

#include <list>
#include <queue>
#include <string>

#include "mystd/deque_blocks_based.hpp"
#include "mystd/deque_cyclicbuffer_based.hpp"
#include "mystd/list_fixed_array.hpp"
#include "mystd/list_linked_nodes.hpp"
#include "mystd/queue_adapter.hpp"

namespace my::testing {

TEST(QueueAdapterTest, Initialization) {
  std::list<int> l = {1, 2, 3};
  my::queue<int, std::list<int>> q1(l);
  std::queue<int, std::list<int>> q2(l);

  EXPECT_EQ(q1.front(), 1);
  EXPECT_EQ(q1.back(), 3);

  EXPECT_EQ(q2.front(), 1);
  EXPECT_EQ(q2.back(), 3);

  EXPECT_EQ(l.front(), 1);
  EXPECT_EQ(l.back(), 3);
}

TEST(QueueAdapterTest, Interface) {
  my::queue<int> q;
  EXPECT_TRUE(q.empty());

  q.enqueue(1);
  q.enqueue(2);
  EXPECT_FALSE(q.empty());
  EXPECT_EQ(q.size(), 2);

  EXPECT_EQ(q.front(), 1);
  EXPECT_EQ(q.back(), 2);

  q.dequeue();
  EXPECT_EQ(q.front(), 2);
  EXPECT_EQ(q.back(), 2);

  q.dequeue();
  EXPECT_EQ(q.size(), 0);
  EXPECT_TRUE(q.empty());
}

TEST(QueueAdapterTest, CopyAndMove) {
my:
  queue<int> q;
  EXPECT_TRUE(q.empty());

  q.enqueue(1);
  q.enqueue(2);
  EXPECT_FALSE(q.empty());

  // copy
  my::queue<int> q1;
  q1 = q;
  EXPECT_EQ(q.front(), q1.front());
  EXPECT_EQ(q.back(), q1.back());
  EXPECT_EQ(q.size(), q1.size());

  // move
  my::queue<int> q2;
  q2 = std::move(q);
  EXPECT_TRUE(q.empty());
  EXPECT_EQ(q2.front(), q1.front());
  EXPECT_EQ(q2.back(), q1.back());
  EXPECT_EQ(q2.size(), q1.size());
}

struct MyType {
  int a = 42;
};

TEST(QueueAdapterTest, CustomTypes) {
  my::queue<MyType> s;
  s.enqueue(MyType());
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s.front().a, 42);
  s.dequeue();
  EXPECT_TRUE(s.empty());
}

TEST(QueueAdapterTest, MyStdImplementations) {
  my::queue<int, my::arraybased::list<int>> s2;
  s2.enqueue(1);
  s2.enqueue(2);
  EXPECT_EQ(s2.front(), 1);
  s2.dequeue();
  s2.dequeue();
  EXPECT_TRUE(s2.empty());

  my::queue<int, my::heapbased::list<int>> s3;
  s3.enqueue(1);
  s3.enqueue(2);
  EXPECT_EQ(s3.front(), 1);
  s3.dequeue();
  s3.dequeue();
  EXPECT_TRUE(s3.empty());

  my::queue<int, my::cyclicbufferbased::deque<int, 8>> s4;
  EXPECT_TRUE(s4.empty());
  for (size_t i = 0; i < 100; ++i) {
    s4.enqueue(i);
    EXPECT_EQ(s4.front(), 0);
  }
  for (size_t i = 0; i < 100; ++i) {
    s4.dequeue();
  }
  EXPECT_TRUE(s4.empty());

  my::queue<int, my::blocksbased::deque<int, 1>> s5;
  EXPECT_TRUE(s5.empty());
  for (size_t i = 0; i < 100; ++i) {
    s5.enqueue(i);
    EXPECT_EQ(s5.front(), 0);
  }
  for (size_t i = 0; i < 100; ++i) {
    s5.dequeue();
  }
  EXPECT_TRUE(s5.empty());
}

}  // namespace my::testing
