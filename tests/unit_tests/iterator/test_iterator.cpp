#include <gtest/gtest.h>

#include <algorithm>
#include <initializer_list>
#include <vector>

#include "mystd/iterator/iterator.hpp"

namespace my::testing {

class MyContainer {
 public:
  std::vector<int> data;
  MyContainer(std::initializer_list<int> init) : data(init) {}

  forward_iterator<int> fbegin() { return forward_iterator<int>(data.data()); }
  forward_iterator<int> fend() { return forward_iterator<int>(data.data() + data.size()); }

  bidirectional_iterator<int> bbegin() { return bidirectional_iterator<int>(data.data()); }
  bidirectional_iterator<int> bend() { return bidirectional_iterator<int>(data.data() + data.size()); }

  random_access_iterator<int> rbegin() { return random_access_iterator<int>(data.data()); }
  random_access_iterator<int> rend() { return random_access_iterator<int>(data.data() + data.size()); }

  random_access_iterator<int, true> crbegin() const { return random_access_iterator<int, true>(data.data()); }
  random_access_iterator<int, true> crend() const {
    return random_access_iterator<int, true>(data.data() + data.size());
  }
};

TEST(MyIterators, Forward) {
  MyContainer vec = {1, 2, 3, 4, 5};

  auto it = vec.fbegin();
  EXPECT_EQ(*it, 1);

  ++it;
  EXPECT_EQ(*it, 2);

  it++;
  EXPECT_EQ(*it, 3);

  int i = 1;
  for (auto it = vec.fbegin(), ite = vec.fend(); it != ite; ++it) {
    EXPECT_EQ(*it, i++);
  }
  EXPECT_EQ(i, 6);
}

TEST(MyIterators, Bidirectional) {
  MyContainer vec = {1, 2, 3, 4, 5};

  auto it = vec.bbegin();
  EXPECT_EQ(*it, 1);

  ++it;
  EXPECT_EQ(*it, 2);

  it++;
  EXPECT_EQ(*it, 3);

  --it;
  EXPECT_EQ(*it, 2);

  it--;
  EXPECT_EQ(*it, 1);

  int i = 1;
  for (auto it = vec.bbegin(), ite = vec.bend(); it != ite; ++it) {
    EXPECT_EQ(*it, i++);
  }
  EXPECT_EQ(i, 6);
}

TEST(MyIterators, RandomAccess) {
  MyContainer vec = {1, 2, 3, 4, 5};

  auto it = vec.rbegin();
  EXPECT_EQ(*it, 1);

  it += 4;
  EXPECT_EQ(*it, 5);

  it -= 4;
  EXPECT_EQ(*it, 1);

  EXPECT_EQ(*it + 1, 2);
  EXPECT_EQ(*it - 1, 0);

  it = vec.rbegin();
  auto ite = vec.rend();

  EXPECT_TRUE(it < ite);

  auto it2 = vec.rbegin();

  it += 3;
  it2 += 3;
  EXPECT_TRUE(it == it2);

  it2 += 1;
  EXPECT_TRUE(it < it2);
  EXPECT_TRUE(it <= it2);

  it2 -= 2;
  EXPECT_TRUE(it > it2);
  EXPECT_TRUE(it >= it2);

  // const test
  using cit = random_access_iterator<int, true>;
  cit it3(vec.data.data());
  // *it3 = 42; // CE

  cit begin(vec.data.data()), end(vec.data.data() + vec.data.size());

  auto it4 = std::find(begin, end, 2);
  EXPECT_TRUE(it4 != vec.rend());
  EXPECT_TRUE(*it4 == 2);

  auto it5 = std::find(vec.rbegin(), vec.rend(), 3);
  EXPECT_TRUE(it5 != vec.rend());
  EXPECT_TRUE(*it5 == 3);
}

}  // namespace my::testing
