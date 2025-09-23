#include <gtest/gtest.h>

#include <exception>
#include <string>

#include "mystd/multimap.hpp"

namespace my::testing {

TEST(MultimapTest, ConstructAndCopy) {
  my::multimap<int, int> m;
  EXPECT_EQ(m.size(), 0u);
  EXPECT_TRUE(m.empty());

  my::multimap<int, int> m0(m);
  EXPECT_EQ(m0.size(), 0u);
  EXPECT_TRUE(m0.empty());

  m = m;
  EXPECT_EQ(m.size(), 0u);
  EXPECT_TRUE(m.empty());

  my::multimap<int, int> m1 = {{1, 10}, {1, 10}, {2, 20}, {3, 30}};
  EXPECT_EQ(m1.size(), 4u);
  EXPECT_FALSE(m1.empty());
  EXPECT_TRUE(m1.contains(1));

  my::multimap<int, int> m2(m1);
  EXPECT_EQ(m2.size(), 4u);
  EXPECT_FALSE(m2.empty());
  EXPECT_TRUE(m2.contains(1));

  EXPECT_EQ(m1.size(), 4u);
  EXPECT_FALSE(m1.empty());
  EXPECT_TRUE(m1.contains(1));

  my::multimap<int, int> m3;
  m3 = m1;
  EXPECT_EQ(m3.size(), 4u);
  EXPECT_FALSE(m3.empty());
  EXPECT_TRUE(m3.contains(1));

  EXPECT_EQ(m1.size(), 4u);
  EXPECT_FALSE(m1.empty());
  EXPECT_TRUE(m1.contains(1));

  m3.clear();
  m3.clear();
  m3.clear();
}

TEST(MultimapTest, ConstructAndMove) {
  my::multimap<int, int> m1 = {{1, 10}, {2, 20}, {3, 30}};

  my::multimap<int, int> m2(std::move(m1));
  EXPECT_EQ(m2.size(), 3u);
  EXPECT_FALSE(m2.empty());
  EXPECT_TRUE(m2.contains(1));

  EXPECT_EQ(m1.size(), 0u);
  EXPECT_TRUE(m1.empty());
  EXPECT_FALSE(m1.contains(1));

  my::multimap<int, int> m3;
  m3 = std::move(m2);
  EXPECT_EQ(m3.size(), 3u);
  EXPECT_FALSE(m3.empty());
  EXPECT_TRUE(m3.contains(1));

  EXPECT_EQ(m2.size(), 0u);
  EXPECT_TRUE(m2.empty());
  EXPECT_FALSE(m2.contains(1));
}

TEST(MultimapTest, BasicOperations) {
  my::multimap<int, int> m;

  EXPECT_TRUE(m.empty());

  m.insert({1, 10});
  m.insert({2, 20});

  EXPECT_EQ(m.count(3), 0);

  m.insert({3, 30});
  EXPECT_EQ(m.count(3), 1);

  m.insert({3, 30});
  EXPECT_EQ(m.count(3), 2);

  EXPECT_EQ(m.size(), 4);
  EXPECT_FALSE(m.empty());

  m.insert({4, 40});
  EXPECT_EQ(m.size(), 5);

  EXPECT_TRUE(m.contains(1));
  EXPECT_FALSE(m.contains(-1));
  EXPECT_FALSE(m.contains(5));

  auto it = m.find(2);
  EXPECT_EQ(it->second, 20);

  it = m.find(42);
  EXPECT_TRUE(it == m.end());

  m.erase(-1);
  EXPECT_EQ(m.size(), 5);

  m.erase(2);
  EXPECT_EQ(m.size(), 4);
  EXPECT_FALSE(m.contains(2));

  m.clear();
  EXPECT_EQ(m.size(), 0);
  EXPECT_TRUE(m.empty());
}

TEST(MultimapTest, Iterators) {
  my::multimap<int, int> m;
  EXPECT_TRUE(m.begin() == m.end());

  my::multimap<int, int> m1 = {{1, 2}, {2, 3}, {3, 4}};
  int i = 1;
  for (const auto& [k, v] : m1) {
    EXPECT_EQ(k, i);
    EXPECT_EQ(v, i + 1);
    ++i;
  }
}

TEST(MultimapTest, String) {
  my::multimap<int, std::string> source = {{1, "one"}, {2, "two"}};
  EXPECT_EQ(source.size(), 2);

  for (auto i = 0; i < 20; ++i) {
    source.insert({3, "three"});
    EXPECT_EQ(source.count(3), i + 1);
  }

  source.erase(3);
  EXPECT_EQ(source.count(3), 19);

  auto it = source.find(2);
  EXPECT_EQ(it->second, "two");
}

TEST(MultimapTest, Highload) {
  int n = 100000;
  my::multimap<int, double> m;

  for (size_t i = 0; i < n; ++i) {
    m.insert({i, i * 10});
  }
  EXPECT_EQ(m.size(), n);
}

}  // namespace my::testing
