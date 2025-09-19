#include <gtest/gtest.h>

#include <algorithm>
#include <initializer_list>
#include <vector>

#include "mystd/iterator/iterator.hpp"

namespace my::testing {

// sorth specializations

template <typename ValueType, bool IsConst = false>
using forward_iterator = my::iterator<ValueType, std::conditional_t<IsConst, const ValueType*, ValueType*>, IsConst,
                                      std::forward_iterator_tag,
                                      ArrayTraversalPolicy<std::conditional_t<IsConst, const ValueType*, ValueType*>>,
                                      my::ArrayValueExtractor<ValueType>>;

template <typename ValueType, bool IsConst = false>
using bidirectional_iterator =
    my::bidirectional_iterator<ValueType, std::conditional_t<IsConst, const ValueType*, ValueType*>, IsConst,
                               std::bidirectional_iterator_tag,
                               ArrayTraversalPolicy<std::conditional_t<IsConst, const ValueType*, ValueType*>>,
                               my::ArrayValueExtractor<ValueType>>;

template <typename ValueType, bool IsConst = false>
using random_access_iterator =
    my::random_access_iterator<ValueType, std::conditional_t<IsConst, const ValueType*, ValueType*>, IsConst,
                               std::random_access_iterator_tag,
                               ArrayTraversalPolicy<std::conditional_t<IsConst, const ValueType*, ValueType*>>,
                               my::ArrayValueExtractor<ValueType>>;

// helper for tests

class MyContainer {
 public:
  std::vector<int> value;
  MyContainer(std::initializer_list<int> init) : value(init) {}

  forward_iterator<int> fbegin() { return forward_iterator<int>(value.data()); }
  forward_iterator<int> fend() { return forward_iterator<int>(value.data() + value.size()); }

  bidirectional_iterator<int> bbegin() { return bidirectional_iterator<int>(value.data()); }
  bidirectional_iterator<int> bend() { return bidirectional_iterator<int>(value.data() + value.size()); }

  random_access_iterator<int> rbegin() { return random_access_iterator<int>(value.data()); }
  random_access_iterator<int> rend() { return random_access_iterator<int>(value.data() + value.size()); }

  random_access_iterator<int, true> crbegin() const { return random_access_iterator<int, true>(value.data()); }
  random_access_iterator<int, true> crend() const {
    return random_access_iterator<int, true>(value.data() + value.size());
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

// TODO check random access iterator, don't work now
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

  // EXPECT_TRUE(it < ite);

  // auto it2 = vec.rbegin();

  // it += 3;
  // it2 += 3;
  // EXPECT_TRUE(it == it2);

  // it2 += 1;
  // EXPECT_TRUE(it < it2);
  // EXPECT_TRUE(it <= it2);

  // it2 -= 2;
  // EXPECT_TRUE(it > it2);
  // EXPECT_TRUE(it >= it2);

  // random_access_iterator<int, true> it3(vec.value.data());
  // // *it3 = 42; // CE

  // auto begin = vec.crbegin();
  // auto end = vec.crend();

  // auto it4 = std::find(begin, end, 2);
  // EXPECT_TRUE(it4 != vec.crend());
  // EXPECT_TRUE(*it4 == 2);

  // auto it5 = std::find(vec.rbegin(), vec.rend(), 3);
  // EXPECT_TRUE(it5 != vec.rend());
  // EXPECT_TRUE(*it5 == 3);
}

}  // namespace my::testing
