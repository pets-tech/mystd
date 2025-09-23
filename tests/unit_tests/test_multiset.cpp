#include <gtest/gtest.h>

#include <string>

#include "mystd/multiset.hpp"

namespace my::testing {

TEST(MultisetTest, ConstructCopy) {
  my::multiset<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0u);

  my::multiset<int> s0 = {1, 1, 1, 2, 3, 3};
  EXPECT_EQ(s0.size(), 6u);
  EXPECT_EQ(s0.count(1), 3u);
  EXPECT_EQ(s0.count(3), 2u);

  my::multiset<int> s_(s);
  EXPECT_TRUE(s_.empty());
  EXPECT_EQ(s_.size(), 0u);

  s_ = s_;
  EXPECT_TRUE(s_.empty());
  EXPECT_EQ(s_.size(), 0u);

  my::multiset<int> s1 = {1, 2, 3};
  EXPECT_FALSE(s1.empty());
  EXPECT_EQ(s1.size(), 3u);

  my::multiset<int> s2(s1);
  EXPECT_EQ(s2.size(), 3u);
  EXPECT_TRUE(s2.contains(1));

  my::multiset<int> s3;
  s3 = s1;
  EXPECT_EQ(s3.size(), 3u);
  EXPECT_TRUE(s3.contains(1));

  s3.clear();
  EXPECT_TRUE(s3.empty());
  EXPECT_EQ(s3.size(), 0u);
}

TEST(MultisetTest, ConstructMove) {
  my::multiset<int> s1 = {1, 2, 3, 3};

  my::multiset<int> s2(std::move(s1));
  EXPECT_EQ(s2.size(), 4u);
  EXPECT_EQ(s2.count(3), 2u);

  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.size(), 0u);

  my::multiset<int> s3;
  s3 = std::move(s2);
  EXPECT_EQ(s3.size(), 4u);
  EXPECT_TRUE(s3.contains(3));

  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.size(), 0u);

  s3 = std::move(s3);
  EXPECT_EQ(s3.size(), 4u);
  EXPECT_EQ(s3.count(3), 2u);
}

TEST(MultisetTest, BasicOperations) {
  my::multiset<int> s;
  EXPECT_TRUE(s.empty());

  s.insert(1);
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 1);

  s.insert(2);
  EXPECT_EQ(s.size(), 2);

  for (size_t i = 0; i < 5; ++i) {
    s.insert(3);
  }
  EXPECT_EQ(s.size(), 7);
  EXPECT_EQ(s.count(3), 5);

  auto it = s.find(2);
  EXPECT_EQ(*it, 2);

  it = s.find(0);
  EXPECT_TRUE(it == s.end());

  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(0));

  s.erase(3);
  EXPECT_EQ(s.count(3), 4u);
  EXPECT_EQ(s.size(), 6);

  s.erase(0);
  EXPECT_EQ(s.size(), 6);

  s.clear();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(MultisetTest, Iterators) {
  my::multiset<int> s = {1, 2, 2, 3};

  auto it = s.begin();
  auto ite = s.end();
  EXPECT_TRUE(it != ite);
  EXPECT_EQ(*it, 1);

  ++it;
  EXPECT_EQ(*it, 2);

  ++it;
  EXPECT_EQ(*it, 2);

  ++it;
  EXPECT_EQ(*it, 3);

  ++it;
  EXPECT_EQ(it, s.end());

  it = s.begin();
  it++;
  it++;
  it++;
  EXPECT_EQ(*it, 3);

  --it;
  EXPECT_EQ(*it, 2);

  --it;
  EXPECT_EQ(*it, 2);

  --it;
  EXPECT_EQ(*it, 1);
}

TEST(MultisetTest, String) {
  my::multiset<std::string> s = {"one", "one", "two"};
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s.count("one"), 2);

  auto it = s.find("two");
  EXPECT_EQ(*it, "two");

  s.insert("three");
  s.insert("three");
  EXPECT_EQ(s.count("three"), 2u);
}

TEST(MultisetTest, Highload) {
  int n = 50000;
  my::multiset<int> s;

  for (size_t i = 0; i < n; ++i) {
    s.insert(i % 100);
  }
  EXPECT_EQ(s.size(), n);
  EXPECT_EQ(s.count(42), n / 100);
}

}  // namespace my::testing
