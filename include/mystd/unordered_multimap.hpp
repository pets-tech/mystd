#pragma once

#include <initializer_list>

#include "mystd/unordered_map_base.hpp"

namespace my {

template <class Key, class Value, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>,
          class Allocator = std::allocator<std::pair<const Key, Value>>>
using unordered_multimap = unordered_map_base<Key, Value, Hash, KeyEqual, InsertPolicy::AllowDuplicates, Allocator>;

}  // namespace my
