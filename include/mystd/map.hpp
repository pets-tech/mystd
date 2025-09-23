#pragma once

#include "mystd/map_base.hpp"

namespace my {

template <typename Key, typename T, typename Tree = my::rb_tree<std::pair<Key, T>, KeyOfPair<Key, T>>>
using map = map_base<Key, T, true, Tree>;

}
