#include <gtest/gtest.h>

#include <iterator>

#include "mystd/list_fixed_array.hpp"

namespace my::arraybased {
namespace testings {

TEST(ListTest, Initialization) {
  list<int> a;
  EXPECT_TRUE(a.empty());

  list<float> b = {1.0, 2.0, 3.0};
  EXPECT_FALSE(b.empty());
  EXPECT_EQ(b.front(), 1.0);
  EXPECT_EQ(b.back(), 3.0);
};

TEST(ListTest, ListInterfaceConsistancy) {
  my::arraybased::list<int> l1 = {1, 2, 3};
  //                              ^head
  //                                  ^tail

  EXPECT_EQ(l1.front(), 1);
  EXPECT_EQ(l1.back(), 3);
}

TEST(ListTest, PushAndPop) {
  list<float> a;
  a.push_back(1.0);
  a.push_front(2.0);

  EXPECT_EQ(a.back(), 1.0);
  EXPECT_EQ(a.front(), 2.0);

  a.pop_back();
  EXPECT_EQ(a.back(), 2.0);
  EXPECT_EQ(a.front(), 2.0);

  a.push_back(3.0);
  a.pop_front();
  EXPECT_EQ(a.back(), 3.0);
  EXPECT_EQ(a.front(), 3.0);

  a.pop_front();
  EXPECT_TRUE(a.empty());
}

TEST(ListTest, InsertAndErase) {
  list<int> a;
  a.insert(a.begin(), 1);
  EXPECT_EQ(a.front(), 1);

  a.insert(a.begin(), 2);
  EXPECT_EQ(a.front(), 2);

  a.insert(a.end(), 3);
  EXPECT_EQ(a.back(), 3);

  a.insert(a.end(), 4);
  EXPECT_EQ(a.back(), 4);

  a.insert(++a.begin(), 42);
  EXPECT_EQ(a.front(), 2);
  a.pop_front();
  EXPECT_EQ(a.front(), 42);

  a.insert(--a.end(), 42);
  EXPECT_EQ(a.back(), 4);
  a.pop_back();
  EXPECT_EQ(a.back(), 42);

  EXPECT_EQ(a.size(), 4);

  a.erase(++a.begin());
  EXPECT_EQ(a.size(), 3);

  EXPECT_EQ(a.front(), 42);
  a.erase(a.begin());
  EXPECT_EQ(a.front(), 3);

  EXPECT_EQ(a.back(), 42);
  a.erase(a.end());
  EXPECT_EQ(a.back(), 3);

  EXPECT_EQ(a.size(), 1);
}

TEST(ListTest, Copy) {
  list<int> a = {1, 2, 3};
  list<int> b(a);
  list<int> c = a;

  EXPECT_EQ(a.size(), b.size());
  EXPECT_EQ(a.size(), c.size());

  EXPECT_EQ(a.front(), b.front());
  EXPECT_EQ(a.front(), c.front());

  EXPECT_EQ(a.back(), b.back());
  EXPECT_EQ(a.back(), c.back());
}

TEST(ListTest, Move) {
  list<int> a = {1, 2, 3};
  list<int> b(std::move(a));

  EXPECT_TRUE(a.empty());
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b.front(), 1);
  EXPECT_EQ(b.back(), 3);

  list<int> c = std::move(b);

  EXPECT_TRUE(b.empty());
  EXPECT_EQ(c.size(), 3);
  EXPECT_EQ(c.front(), 1);
  EXPECT_EQ(c.back(), 3);
}

TEST(ListTest, Iterators) {
  list<int> a = {1, 2, 3};
  int i = 1;
  for (auto it = a.begin(); it != a.end(); ++it) {
    EXPECT_EQ(*it, i++);
  }

  i = 1;
  for (auto& el : a) {
    EXPECT_EQ(el, i++);
  }

  i = 1;
  for (const auto& el : a) {
    EXPECT_EQ(el, i++);
  }

  // i = 3;
  // for (auto it = std::rbegin(a); it != std::rend(a); ++it) {
  //     EXPECT_EQ(*it, i--);
  // }
}

TEST(ListTest, CustomType) {
  struct Point {
    int x, y;
    bool operator==(const Point& rhs) const { return ((x == rhs.x) && (y == rhs.y)); }
  };
  list<Point> p;
  p.push_front(Point(1, 2));
  p.push_front(Point(3, 4));
  p.push_back(Point(0, 0));

  EXPECT_EQ(p.size(), 3);
  EXPECT_EQ(p.front(), Point(3, 4));
  EXPECT_EQ(p.back(), Point(0, 0));
}

}  // namespace testings
}  // namespace my::arraybased
