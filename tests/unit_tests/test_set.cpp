#include <gtest/gtest.h>

#include <string>

#include "mystd/set.hpp"

namespace my::testing {

TEST(SetTest, ConstructCopy) {
  my::set<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0u);

  my::set<int> s0 = {1, 1, 1, 2, 3, 3};
  EXPECT_EQ(s0.size(), 3u);

  my::set<int> s_(s);
  EXPECT_TRUE(s_.empty());
  EXPECT_EQ(s_.size(), 0u);

  s_ = s_;
  EXPECT_TRUE(s_.empty());
  EXPECT_EQ(s_.size(), 0u);

  my::set<int> s1 = {1, 2, 3};
  EXPECT_FALSE(s1.empty());
  EXPECT_EQ(s1.size(), 3u);

  // copy
  my::set<int> s2(s1);
  EXPECT_EQ(s2.size(), 3u);
  EXPECT_TRUE(s2.contains(1));

  my::set<int> s3;
  s3 = s1;
  EXPECT_EQ(s3.size(), 3u);
  EXPECT_TRUE(s3.contains(1));

  s3.clear();
  s3.clear();
  s3.clear();
}

TEST(SetTest, ConstructMove) {
  my::set<int> s1 = {1, 2, 3};

  // move
  my::set<int> s2(std::move(s1));
  EXPECT_EQ(s2.size(), 3u);
  EXPECT_TRUE(s2.contains(1));

  EXPECT_TRUE(s1.empty());
  EXPECT_EQ(s1.size(), 0u);

  my::set<int> s3;
  s3 = std::move(s2);
  EXPECT_EQ(s3.size(), 3u);
  EXPECT_TRUE(s3.contains(1));

  EXPECT_TRUE(s2.empty());
  EXPECT_EQ(s2.size(), 0u);

  s3 = std::move(s3);
  EXPECT_EQ(s3.size(), 3u);
  EXPECT_TRUE(s3.contains(1));
}

TEST(SetTest, BasicOperations) {
  my::set<int> s;
  EXPECT_TRUE(s.empty());

  // insert
  s.insert(1);
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 1);

  s.insert(2);
  EXPECT_EQ(s.size(), 2);

  for (size_t i = 0; i < 100; ++i) {
    s.insert(3);
  }
  EXPECT_EQ(s.size(), 3);

  // find
  auto it = s.find(2);
  EXPECT_EQ(*it, 2);

  it = s.find(0);
  EXPECT_TRUE(it == s.end());

  // containts
  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(0));

  // erase
  s.erase(3);
  EXPECT_EQ(s.size(), 2);

  s.erase(0);
  EXPECT_EQ(s.size(), 2);

  s.erase(2);
  s.erase(1);
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(SetTest, Iteratos) {
  my::set<int> s = {1, 2, 3};

  auto it = s.begin();
  auto ite = s.end();

  EXPECT_TRUE(it != ite);
  EXPECT_EQ(*it, 1);

  it++;
  EXPECT_EQ(*it, 2);

  ++it;
  EXPECT_EQ(*it, 3);

  ++it;
  EXPECT_EQ(it, s.end());

  // ! this dose not work. don't impelented end()-- feature :)
  // it--;
  // EXPECT_EQ(*it, 3);

  it = s.begin();
  it++;
  it++;
  EXPECT_EQ(*it, 3);

  it--;
  EXPECT_EQ(*it, 2);

  --it;
  EXPECT_EQ(*it, 1);
}

TEST(SetTest, String) {
  my::set<std::string> s = {"one", "one", "two"};
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.count("one"), 1);

  auto it = s.find("two");
  EXPECT_EQ(*it, "two");
}

TEST(SetTest, Highload) {
  int n = 100000;
  my::set<int> s;

  for (size_t i = 0; i < n; ++i) {
    s.insert(i);
  }
  EXPECT_EQ(s.size(), n);
}

}  // namespace my::testing
