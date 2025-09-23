#pragma once

#include "mystd/set_base.hpp"

namespace my {

template <typename Key, typename Tree = my::rb_tree<Key, KeyOfIdentity<Key>>>
using multiset = set_base<Key, false, Tree>;

}
