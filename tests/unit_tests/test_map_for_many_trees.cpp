#include <gtest/gtest.h>

#include <algorithm>
#include <exception>
#include <string>
#include <utility>

#include "mystd/map_base.hpp"
#include "mystd/some_trees/rb_tree.hpp"
#include "mystd/some_trees/treap.hpp"

namespace my {

template <typename Key, typename T, typename Tree = my::rb_tree<std::pair<Key, T>, KeyOfPair<Key, T>>>
using map_rbtree = map_base<Key, T, true, Tree>;

template <typename Key, typename T, typename Tree = my::treap<std::pair<Key, T>, KeyOfPair<Key, T>>>
using map_treap = map_base<Key, T, true, Tree>;

}  // namespace my

namespace my::testing {

TEST(MapForManyTreesTest, InsertAndSize) {
  // map rb tree based
  my::map_rbtree<int, std::string> m_rb;
  EXPECT_TRUE(m_rb.empty());
  EXPECT_EQ(m_rb.size(), 0);

  m_rb.insert({1, "one"});
  m_rb.insert({2, "two"});
  m_rb.insert({3, "three"});
  EXPECT_FALSE(m_rb.empty());
  EXPECT_EQ(m_rb.size(), 3);

  m_rb.insert({1, "one"});
  EXPECT_EQ(m_rb.size(), 3);
  EXPECT_EQ(m_rb[1], "one");

  // map treap based
  my::map_treap<int, std::string> m_treap;
  EXPECT_TRUE(m_treap.empty());
  EXPECT_EQ(m_treap.size(), 0);

  m_treap.insert({1, "one"});
  m_treap.insert({2, "two"});
  m_treap.insert({3, "three"});
  EXPECT_FALSE(m_treap.empty());
  EXPECT_EQ(m_treap.size(), 3);

  m_treap.insert({1, "one"});
  EXPECT_EQ(m_treap.size(), 3);
  EXPECT_EQ(m_treap[1], "one");
}

TEST(MapForManyTreesTest, FindAndAccess) {
  my::map_rbtree<int, std::string> m_rb;
  m_rb.insert({1, "one"});
  m_rb.insert({2, "two"});

  auto itrb = m_rb.find(1);
  ASSERT_NE(itrb, m_rb.end());
  EXPECT_EQ(itrb->second, "one");

  itrb = m_rb.find(3);
  EXPECT_EQ(itrb, m_rb.end());

  my::map_treap<int, std::string> m_treap;
  m_treap.insert({1, "one"});
  m_treap.insert({2, "two"});

  auto ittreap = m_treap.find(1);
  ASSERT_NE(ittreap, m_treap.end());
  EXPECT_EQ(ittreap->second, "one");

  ittreap = m_treap.find(3);
  EXPECT_EQ(ittreap, m_treap.end());
}

TEST(MapForManyTreesTest, Erase) {
  my::map_rbtree<int, std::string> m_rb;
  m_rb.insert({1, "one"});
  m_rb.insert({2, "two"});
  m_rb.insert({3, "three"});

  m_rb.erase(2);
  EXPECT_EQ(m_rb.size(), 2);
  EXPECT_EQ(m_rb.find(2), m_rb.end());

  m_rb.erase(4);
  EXPECT_EQ(m_rb.size(), 2);

  my::map_treap<int, std::string> m_treap;
  m_treap.insert({1, "one"});
  m_treap.insert({2, "two"});
  m_treap.insert({3, "three"});

  m_treap.erase(2);
  EXPECT_EQ(m_treap.size(), 2);
  EXPECT_EQ(m_treap.find(2), m_treap.end());

  m_treap.erase(4);
  EXPECT_EQ(m_treap.size(), 2);
}

TEST(MapForManyTreesTest, Iterator) {
  my::map_rbtree<int, std::string> m_rb;
  m_rb.insert({1, "one"});
  m_rb.insert({2, "two"});
  m_rb.insert({3, "three"});

  std::vector<std::pair<int, std::string>> result;
  for (const auto& pair : m_rb) {
    result.push_back(pair);
  }
  std::vector<std::pair<int, std::string>> expected = {{1, "one"}, {2, "two"}, {3, "three"}};
  EXPECT_EQ(result, expected);

  my::map_treap<int, std::string> m_treap;
  m_treap.insert({1, "one"});
  m_treap.insert({2, "two"});
  m_treap.insert({3, "three"});

  result.clear();
  for (const auto& pair : m_treap) {
    result.push_back(pair);
  }
  EXPECT_EQ(result, expected);
}

TEST(MapForManyTreesTest, Clear) {
  my::map_rbtree<int, std::string> m_rb;
  m_rb.insert({1, "one"});
  m_rb.insert({2, "two"});
  m_rb.clear();
  EXPECT_TRUE(m_rb.empty());
  EXPECT_EQ(m_rb.size(), 0);
  EXPECT_EQ(m_rb.find(1), m_rb.end());

  my::map_treap<int, std::string> m_treap;
  m_treap.insert({1, "one"});
  m_treap.insert({2, "two"});
  m_treap.clear();
  EXPECT_TRUE(m_rb.empty());
  EXPECT_EQ(m_treap.size(), 0);
  EXPECT_EQ(m_treap.find(1), m_treap.end());
}

TEST(MapForManyTreesTest, CopyAndAssignment) {
  my::map_rbtree<int, std::string> m_rb;
  m_rb.insert({1, "one"});
  m_rb.insert({2, "two"});

  my::map_rbtree<int, std::string> m_rb_copy(m_rb);
  EXPECT_EQ(m_rb_copy.size(), 2);
  EXPECT_EQ(m_rb_copy[1], "one");
  EXPECT_EQ(m_rb_copy[2], "two");

  my::map_rbtree<int, std::string> m_rb_assign;
  m_rb_assign = m_rb;
  EXPECT_EQ(m_rb_assign.size(), 2);
  EXPECT_EQ(m_rb_assign[1], "one");
  EXPECT_EQ(m_rb_assign[2], "two");

  my::map_treap<int, std::string> m_treap;
  m_treap.insert({1, "one"});
  m_treap.insert({2, "two"});

  my::map_treap<int, std::string> m_treap_copy(m_treap);
  EXPECT_EQ(m_treap_copy.size(), 2);
  EXPECT_EQ(m_treap_copy[1], "one");
  EXPECT_EQ(m_treap_copy[2], "two");

  my::map_treap<int, std::string> m_treap_assign;
  m_treap_assign = m_treap;
  EXPECT_EQ(m_treap_assign.size(), 2);
  EXPECT_EQ(m_treap_assign[1], "one");
  EXPECT_EQ(m_treap_assign[2], "two");
}

}  // namespace my::testing
