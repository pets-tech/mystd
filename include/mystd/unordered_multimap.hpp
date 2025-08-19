#pragma once

#include <initializer_list>

#include "mystd/hashtable.hpp"

namespace my {

template <class Key, class Value, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>>
class unordered_multimap {
 private:
  using ValueType = std::pair<Key, Value>;

  struct KeyOfValue {
    const Key& operator()(const ValueType& v) const noexcept { return v.first; }
  };

  using Base = hashtable<ValueType, Key, KeyOfValue, Hash, KeyEqual, InsertPolicy::AllowDuplicates>;
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
  unordered_multimap(const size_type bucket_count = 8) : table(bucket_count) {};

  // initializer_list ctor
  unordered_multimap(const std::initializer_list<std::pair<Key, Value>>& init) : unordered_multimap() {
    for (const auto& kv : init) {
      insert(kv.first, kv.second);
    }
  }

  // lookup
  size_type count(const key_type& k) const { return table.count(k); }

  // modifiers

  iterator insert(const key_type& k, const_reference v) { return table.insert({k, v}).first; }

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
