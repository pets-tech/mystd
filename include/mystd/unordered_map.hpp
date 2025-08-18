#pragma once

#include <forward_list>
#include <functional>
#include <initializer_list>
#include <vector>

namespace my {

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class unordered_map {
 public:
  using key_type = Key;
  using value_type = Value;
  using reference = Value&;
  using const_reference = const Value&;

  using size_type = size_t;

 private:
  std::vector<std::forward_list<std::pair<Key, Value>>> buckets;
  Hash hasher;
  size_type num_elements = 0;
  size_type max_load_factor = 0.75;
  size_type reallocation_factor = 2;

  size_type hash(key_type key) { return hasher(key); }

  size_type get_index(key_type key) {
    size_type sh = hash(key);
    return sh % buckets.size();
  }

  void rehash() {
    size_type new_capacity = buckets.size() * reallocation_factor;
    std::vector<std::forward_list<std::pair<Key, Value>>> new_buckets{new_capacity};

    for (const auto& bucket : buckets) {
      for (const auto& kv : bucket) {
        size_type new_sh = hash(kv.first) % new_buckets.size();
        new_buckets[new_sh].push_front(kv);
      }
    }
    buckets.swap(new_buckets);
  }

 public:
  // ctor

  unordered_map(size_type capacity = 8) : buckets(capacity) {}

  unordered_map(const std::initializer_list<std::pair<Key, Value>>& init) : unordered_map() {
    for (const auto& kv : init) {
      insert(kv.first, kv.second);
    }
  }

  // capacity

  size_type size() const noexcept { return num_elements; }

  bool empty() const noexcept { return num_elements == 0; }

  // observers

  reference operator[](const key_type& key) {
    size_type idx = get_index(key);

    for (auto& kv : buckets[idx]) {
      if (kv.first == key) {
        return kv.second;
      }
    }

    buckets[idx].push_front({key, value_type{}});
    ++num_elements;

    size_type load_factor = num_elements / buckets.size();
    if (load_factor > max_load_factor) {
      rehash();
      idx = get_index(key);
    }

    for (auto& kv : buckets[idx]) {
      if (kv.first == key) {
        return kv.second;
      }
    }
  }

  // modifiers

  void insert(const key_type& key, const_reference value) {
    size_type idx = get_index(key);

    for (auto& kv : buckets[idx]) {
      if (kv.first == key) {
        kv.second = value;
      }
    }

    buckets[idx].push_front({key, value});
    ++num_elements;

    size_type load_factor = num_elements / buckets.size();
    if (load_factor > max_load_factor) {
      rehash();
    }
  }

  bool find(const key_type& key) {
    for (const auto& bucket : buckets) {
      for (const auto& kv : bucket) {
        if (kv.first == key) {
          return true;
        }
      }
    }
    return false;
  }

  void erase(const key_type& key) {
    size_type idx = get_index(key);

    auto& bucket = buckets[idx];

    if (bucket.front().first == key) {
      bucket.pop_front();
      --num_elements;
      return;
    }

    auto prev_it = bucket.before_begin();
    for (auto it = bucket.begin(), ite = bucket.end(); it != ite; ++it) {
      if (it->first == key) {
        bucket.erase_after(prev_it);
        --num_elements;
        return;
      }
      ++prev_it;
    }
  }
};

}  // namespace my
