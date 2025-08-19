#pragma once

#include <initializer_list>

#include "mystd/unordered_set_base.hpp"

namespace my {

template <class Value, class Hash = std::hash<Value>, class KeyEqual = std::equal_to<Value>,
          class Allocator = std::allocator<Value> >
using unordered_multiset = unordered_set_base<Value, Hash, KeyEqual, InsertPolicy::AllowDuplicates, Allocator>;

}  // namespace my
