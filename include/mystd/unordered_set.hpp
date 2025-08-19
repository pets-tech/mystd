#pragma once

#include <initializer_list>

#include "mystd/hashtable.hpp"

namespace my {

template <class Key, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>>
class unordered_set {
 private:
  struct Identity {
    const Key& operator()(const Key& k) const noexcept { return k; }
  };

  using Base = hashtable<Key, Key, Identity, Hash, KeyEqual, InsertPolicy::UniqueKeys>;
  Base table;

 public:
  using key_type = Key;
  using value_type = Value;
  using reference = Value&;
  using const_reference = const Value&;
  using size_type = size_t;
  using iterator = typename Base::iterator;
  using const_iterator = typename Base::const_iterator;

  // ctor
  unordered_set(const size_type bucket_count = 8) : table(bucket_count) {};

  // initializer_list ctor
  unordered_set(const std::initializer_list<std::pair<Key, Value>>& init) : unordered_set() {
    for (const auto& kv : init) {
      insert(kv);
    }
  }

  // observers
  reference operator[](const key_type& k) {
    auto [it, inserted] = table.insert(k);
    return *it;
  }

  // modifiers

  iterator insert(const key_type& k) {
    auto it_flag = table.insert(k);
    return it_flag.first;
  }

  iterator find(const key_type& k) { return table.find(k); }

  iterator erase(const key_type& k) { return table.erase(k); }

  // capacity
  size_type size() const { return table.size(); }

  bool empty() const { return table.empty(); }

  size_type bucket_count() const { return table.bucket_count(); }

  // iterators

  iterator begin() { return table.begin(); }

  iterator end() { return table.end(); }

  const_iterator begin() const { return table.begin(); }

  const_iterator end() const { return table.end(); }

  const_iterator cbegin() const { return table.cbegin(); }

  const_iterator cend() const { return table.cend(); }
};

}  // namespace my
