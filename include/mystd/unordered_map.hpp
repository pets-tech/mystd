#pragma once

#include <initializer_list>

#include "mystd/hashtable.hpp"

namespace my {

template <class Key, class Value, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>>
class unordered_map {
 private:
  using ValueType = std::pair<Key, Value>;

  struct KeyOfValue {
    const Key& operator()(const ValueType& v) const noexcept { return v.first; }
  };

  using Base = hashtable<ValueType, Key, KeyOfValue, Hash, KeyEqual, InsertPolicy::UniqueKeys>;
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
  unordered_map() = default;

  unordered_map(const size_type bucket_count) : table(bucket_count) {};

  // initializer_list ctor
  unordered_map(const std::initializer_list<std::pair<Key, Value>>& init) : unordered_map() {
    for (const auto& kv : init) {
      insert(kv.first, kv.second);
    }
  }

  // observers
  reference operator[](const key_type& k) {
    auto [it, inserted] = table.insert({k, value_type{}});
    return (*it).second;
  }

  // modifiers

  iterator insert(const key_type& k, const_reference v) {
    auto it_flag = table.insert({k, v});
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
