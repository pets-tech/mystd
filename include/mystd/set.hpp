#pragma once

#include "mystd/set_base.hpp"

namespace my {

template <typename Key, typename Tree = my::rb_tree<Key, KeyOfIdentity<Key>>>
using set = set_base<Key, true, Tree>;

}
