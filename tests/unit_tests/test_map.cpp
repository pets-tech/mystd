#include <gtest/gtest.h>

#include <exception>
#include <string>

#include "mystd/map.hpp"

namespace my::testing {

TEST(MapTest, ConstructAndCopy) {
  my::map<int, int> m;
  EXPECT_EQ(m.size(), 0u);
  EXPECT_TRUE(m.empty());

  my::map<int, int> m0(m);
  EXPECT_EQ(m0.size(), 0u);
  EXPECT_TRUE(m0.empty());

  m = m;
  EXPECT_EQ(m.size(), 0u);
  EXPECT_TRUE(m.empty());

  my::map<int, int> m1 = {{1, 10}, {1, 10}, {2, 20}, {3, 30}};
  EXPECT_EQ(m1.size(), 3u);
  EXPECT_FALSE(m1.empty());
  EXPECT_TRUE(m1.contains(1));

  // copy
  my::map<int, int> m2(m1);
  EXPECT_EQ(m2.size(), 3u);
  EXPECT_FALSE(m2.empty());
  EXPECT_TRUE(m2.contains(1));

  EXPECT_EQ(m1.size(), 3u);
  EXPECT_FALSE(m1.empty());
  EXPECT_TRUE(m1.contains(1));

  my::map<int, int> m3;
  m3 = m1;
  EXPECT_EQ(m3.size(), 3u);
  EXPECT_FALSE(m3.empty());
  EXPECT_TRUE(m3.contains(1));

  EXPECT_EQ(m1.size(), 3u);
  EXPECT_FALSE(m1.empty());
  EXPECT_TRUE(m1.contains(1));

  m3.clear();
  m3.clear();
  m3.clear();
}

TEST(MapTest, ConstructAndMove) {
  my::map<int, int> m1 = {{1, 10}, {2, 20}, {3, 30}};

  // move
  my::map<int, int> m2(std::move(m1));
  EXPECT_EQ(m2.size(), 3u);
  EXPECT_FALSE(m2.empty());
  EXPECT_TRUE(m2.contains(1));

  EXPECT_EQ(m1.size(), 0u);
  EXPECT_TRUE(m1.empty());
  EXPECT_FALSE(m1.contains(1));

  my::map<int, int> m3;
  m3 = std::move(m2);
  EXPECT_EQ(m3.size(), 3u);
  EXPECT_FALSE(m3.empty());
  EXPECT_TRUE(m3.contains(1));

  EXPECT_EQ(m2.size(), 0u);
  EXPECT_TRUE(m2.empty());
  EXPECT_FALSE(m2.contains(1));
}

TEST(MapTest, BasicOperations) {
  my::map<int, int> m;

  EXPECT_TRUE(m.empty());

  m.insert({1, 10});
  m.insert({2, 20});

  EXPECT_EQ(m.count(3), 0);

  m.insert({3, 30});
  EXPECT_EQ(m.count(3), 1);

  // add dublicate
  m.insert({3, 30});
  EXPECT_EQ(m.count(3), 1);

  EXPECT_EQ(m.size(), 3);
  EXPECT_FALSE(m.empty());

  m.insert({4, 40});
  EXPECT_EQ(m.size(), 4);

  EXPECT_TRUE(m.contains(1));
  EXPECT_FALSE(m.contains(-1));
  EXPECT_FALSE(m.contains(5));

  auto it = m.find(2);
  EXPECT_EQ(it->second, 20);
  EXPECT_EQ(m[2], 20);
  EXPECT_EQ(m.at(2), 20);
  EXPECT_THROW(m.at(42), std::out_of_range);

  it = m.find(42);
  EXPECT_TRUE(it == m.end());

  m.erase(-1);
  EXPECT_EQ(m.size(), 4);

  m.erase(2);
  EXPECT_EQ(m.size(), 3);
  EXPECT_FALSE(m.contains(2));

  m.clear();
  EXPECT_EQ(m.size(), 0);
  EXPECT_TRUE(m.empty());
}

TEST(MapTest, Iterators) {
  my::map<int, int> m;

  EXPECT_TRUE(m.begin() == m.end());

  my::map<int, int> m1 = {{1, 2}, {2, 3}, {3, 4}};

  int i = 1;
  for (const auto& [k, v] : m1) {
    EXPECT_EQ(k, i++);
    EXPECT_EQ(v, i);
  }
}

TEST(MapTest, String) {
  my::map<int, std::string> source = {{1, "one"}, {2, "two"}};
  EXPECT_EQ(source.size(), 2);

  EXPECT_EQ(source[1], "one");
  EXPECT_EQ(source[2], "two");

  source[3] = "three";
  EXPECT_EQ(source[3], "three");

  auto it = source.find(2);
  EXPECT_EQ(it->second, "two");
}

TEST(MapTest, Highload) {
  int n = 100000;
  my::map<int, double> m;

  for (size_t i = 0; i < n; ++i) {
    m.insert({i, i * 10});
  }
  EXPECT_EQ(m.size(), n);
}

}  // namespace my::testing
