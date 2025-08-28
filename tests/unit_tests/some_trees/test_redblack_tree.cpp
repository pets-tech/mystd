#include <gtest/gtest.h>

#include <iostream>
#include <vector>

#include "mystd/some_trees/redblack_tree.hpp"

namespace my::testing {

TEST(RedBlackTreeTest, InsertEraseContains) {
  my::redblack_tree<int, int> t;

  for (size_t i = 1; i <= 10; ++i) {
    t.insert({i, i * 10});
  }

  // for (auto& [k, v] : t) {
  //     std::cout << k << "\n";
  // }

  t.print();
}

}  // namespace my::testing
