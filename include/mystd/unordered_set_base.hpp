#pragma once

#include <initializer_list>

#include "mystd/hashtable.hpp"

namespace my {

template <class Value, class Hash = std::hash<Value>, class KeyEqual = std::equal_to<Value>,
          InsertPolicy Policy = InsertPolicy::UniqueKeys, class Allocator = std::allocator<Value>>
class unordered_set_base {
 private:
  struct Identity {
    const Value& operator()(const Value& v) const noexcept { return v; }
  };

  using Base = hashtable<Value, Value, Identity, Hash, KeyEqual, Policy, Allocator>;
  Base table;

 public:
  using value_type = Value;
  using reference = Value&;
  using const_reference = const Value&;
  using size_type = size_t;
  using iterator = typename Base::iterator;
  using const_iterator = typename Base::const_iterator;

  // ctor
  unordered_set_base(const size_type bucket_count = 8) : table(bucket_count) {};

  // initializer_list ctor
  unordered_set_base(const std::initializer_list<Value>& init) : unordered_set_base() {
    for (const auto& v : init) {
      insert(v);
    }
  }

  // lookup
  size_type count(const value_type& v) const { return table.count(v); }

  // modifiers

  iterator insert(const value_type& v) {
    auto it_flag = table.insert(v);
    return it_flag.first;
  }

  iterator find(const value_type& v) { return table.find(v); }

  iterator erase(const value_type& v) { return table.erase(v); }

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
