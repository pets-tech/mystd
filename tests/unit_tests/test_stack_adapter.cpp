#include <gtest/gtest.h>

#include <list>
#include <string>
#include <vector>

#include "mystd/list_fixed_array.hpp"
#include "mystd/list_linked_nodes.hpp"
#include "mystd/stack_adapter.hpp"
#include "mystd/vector.hpp"

namespace my::testing {

TEST(StackAdapterTest, CopyAndMove) {
  my::stack<int> s1;
  s1.push(1);
  s1.push(2);

  auto s2 = s1;
  ASSERT_EQ(s2.top(), 2);
  s2.pop();
  ASSERT_EQ(s2.top(), 1);
  ASSERT_EQ(s1.top(), 2);

  auto s3 = std::move(s1);
  ASSERT_TRUE(s1.empty());
  ASSERT_EQ(s3.top(), 2);
}

TEST(StackAdapterTest, BasicOperations) {
  stack<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);

  s.push(1);
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s.top(), 1);

  s.push(2);
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.top(), 2);

  s.pop();
  EXPECT_EQ(s.top(), 1);
  EXPECT_EQ(s.size(), 1);
}

TEST(StackAdapterTest, EdgeCases) {
  stack<std::string> s;
  s.push("test");
  s.push(std::string("move"));

  EXPECT_EQ(s.top(), "move");
  s.pop();
  EXPECT_EQ(s.top(), "test");
}

TEST(StackAdapterTest, ContainerTypes) {
  stack<int, std::vector<int>> s;
  s.push(1);
  EXPECT_EQ(s.top(), 1);
  s.pop();
  EXPECT_TRUE(s.empty());

  stack<int, std::list<int>> slist;
  slist.push(1);
  EXPECT_EQ(slist.top(), 1);
  slist.pop();
  EXPECT_TRUE(slist.empty());
}

struct MyType {
  int a = 42;
};

TEST(StackAdapterTest, CustomTypes) {
  stack<MyType> s;
  s.push(MyType());
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s.top().a, 42);
  s.pop();
  EXPECT_TRUE(s.empty());
}

TEST(StackAdapterTest, MyStdImplementations) {
  stack<int, my::vector<int>> s1;
  s1.push(1);
  EXPECT_EQ(s1.top(), 1);
  s1.pop();
  EXPECT_TRUE(s1.empty());

  stack<int, my::arraybased::list<int>> s2;
  s2.push(1);
  EXPECT_EQ(s2.top(), 1);
  s2.pop();
  EXPECT_TRUE(s2.empty());

  stack<int, my::heapbased::list<int>> s3;
  s3.push(1);
  EXPECT_EQ(s3.top(), 1);
  s3.pop();
  EXPECT_TRUE(s3.empty());
}

}  // namespace my::testing
