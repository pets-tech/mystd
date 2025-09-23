#pragma once

#include <cmath>
#include <utility>

#include "mystd/some_trees/rb_tree.hpp"

namespace my {

template <typename Key, typename T, bool Unique = true,
          typename Tree = my::rb_tree<std::pair<Key, T>, KeyOfPair<Key, T>>>
class map_base {
 public:
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const Key, T>;
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
  // construct/copy/move/destruct
  map_base() = default;

  map_base(std::initializer_list<value_type> init) {
    for (const auto& el : init) {
      insert(el);
    }
  }

  map_base(const map_base& other) {
    Tree tmp(other.tree);
    tree.swap(tmp);
  }

  map_base(map_base&& other) : tree(std::exchange(other.tree, Tree{})) {}

  ~map_base() { tree.clear(); }

  map_base& operator=(const map_base& other) {
    if (this != &other) {
      map_base tmp(other);
      swap(tmp);
    }
    return *this;
  }

  map_base& operator=(map_base&& other) {
    if (this != &other) {
      clear();
      tree = std::exchange(other.tree, Tree{});
    }
    return *this;
  }

 public:
  // capacity

  size_t size() const { return tree.size(); }

  bool empty() const { return tree.empty(); }

  // accessors
  mapped_type& operator[](const key_type& key)
    requires(Unique)
  {
    auto it = find(key);
    if (it == tree.end()) {
      tree.insert({key, mapped_type{}});
      it = find(key);
    }
    return it->second;
  }

  const mapped_type& operator[](const key_type& key) const
    requires(Unique)
  {
    auto it = find(key);
    if (it == tree.end()) {
      tree.insert({key, mapped_type{}});
      it = find(key);
    }
    return it->second;
  }

  mapped_type& at(const key_type& key)
    requires(Unique)
  {
    auto it = find(key);
    if (it == end()) {
      throw std::out_of_range("Key not found");
    }
    return it->second;
  }

  const mapped_type& at(const key_type& key) const
    requires(Unique)
  {
    auto it = find(key);
    if (it == end()) {
      throw std::out_of_range("Key not found");
    }
    return it->second;
  }

  // modifiers
  void clear() { tree.clear(); }

  // return nothing, because some of my trees have no iterators
  void insert(const value_type& value) {
    if constexpr (Unique) {
      if (!tree.contains(value.first)) {
        tree.insert(value);
      }
    } else {
      tree.insert(value);
    }
  }

  void erase(const key_type& key) { tree.erase(key); }

  void swap(map_base& other) { std::swap(tree, other.tree); }
  // merge()

  // lookup

  size_type count(const key_type& key) { return tree.count(key); }

  iterator find(const key_type& key) { return tree.find(key); }

  bool contains(const key_type& key) { return tree.contains(key); }

  // lower_bound()
  // upper_bound()

  /// iterators

  iterator begin() { return tree.begin(); }

  iterator end() { return tree.end(); }

  const_iterator begin() const { return tree.cbegin(); }

  const_iterator end() const { return tree.cend(); }
};

}  // namespace my
