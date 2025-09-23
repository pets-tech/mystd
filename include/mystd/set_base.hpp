#pragma once

#include <initializer_list>

#include "mystd/some_trees/rb_tree.hpp"

namespace my {

template <typename Key, bool Unique = true, typename Tree = my::rb_tree<Key, KeyOfIdentity<Key>>>
class set_base {
 public:
  using key_type = Key;
  using mapped_type = Key;
  using value_type = Key;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;

  using iterator = typename Tree::iterator;
  using const_iterator = typename Tree::const_iterator;

 private:
  Tree tree{};

 public:
  set_base() = default;

  set_base(std::initializer_list<value_type> init) {
    for (const auto& el : init) {
      insert(el);
    }
  }

  set_base(const set_base& other) {
    Tree tmp(other.tree);
    tree.swap(tmp);
  }

  set_base(set_base&& other) { tree = std::exchange(other.tree, Tree{}); }

  ~set_base() { clear(); }

  set_base& operator=(const set_base& other) {
    if (this != &other) {
      set_base tmp(other);
      swap(tmp);
    }
    return *this;
  }

  set_base& operator=(set_base&& other) {
    if (this != &other) {
      set_base tmp(std::move(other));
      swap(tmp);
    }
    return *this;
  }

  void clear() { tree.clear(); }

  void swap(set_base& other) { std::swap(tree, other.tree); }

  // merge()

  size_type size() const { return tree.size(); }

  bool empty() const { return tree.empty(); }

  size_type count(const key_type& key) { return tree.count(key); }

  bool contains(const key_type& key) { return tree.contains(key); }

  iterator find(const key_type& key) {
    auto it = tree.find(key);
    return it;
  }

  // process to insert unique values
  void insert(const value_type& value) {
    if constexpr (Unique) {
      if (!tree.contains(value)) {
        tree.insert(value);
      }
    } else {
      tree.insert(value);
    }
  }

  void erase(const value_type& value) { tree.erase(value); }

  // lower_bound()
  // upper_bound()

  iterator begin() { return tree.begin(); }

  iterator end() { return tree.end(); }

  const_iterator begin() const { return tree.cbegin(); }

  const_iterator end() const { return tree.cend(); }
};

}  // namespace my
