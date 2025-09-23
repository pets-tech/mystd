#pragma once

#include "mystd/map_base.hpp"

namespace my {

template <typename Key, typename T, typename Tree = my::rb_tree<std::pair<Key, T>, KeyOfPair<Key, T>>>
using multimap = map_base<Key, T, false, Tree>;

}
