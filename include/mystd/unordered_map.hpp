#pragma once

#include <forward_list>
#include <functional>
#include <initializer_list>
#include <utility>
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

  static constexpr size_type max_load_factor = 0.75;
  static constexpr size_type reallocation_factor = 2;

 private:
  using Bucket = std::forward_list<std::pair<Key, Value>>;

  std::vector<Bucket> buckets;
  Hash hasher;
  size_type num_elements = 0;

  size_type hash(key_type key) { return hasher(key); }

  size_type get_index(key_type key) {
    size_type sh = hash(key);
    return sh % buckets.size();
  }

  void rehash() {
    size_type new_capacity = buckets.size() * reallocation_factor;
    std::vector<Bucket> new_buckets{new_capacity};

    for (const auto& bucket : buckets) {
      for (const auto& kv : bucket) {
        size_type new_sh = hash(kv.first) % new_buckets.size();
        new_buckets[new_sh].push_front(kv);
      }
    }
    buckets.swap(new_buckets);
  }

  template <bool IsConst>
  class iterator_basic {
   private:
    using buckets_iter = typename std::vector<Bucket>::iterator;
    using bucket_iter = typename Bucket::iterator;

    using reference = std::conditional_t<IsConst, const std::pair<Key, Value>&, std::pair<Key, Value>&>;
    using pointer = std::conditional_t<IsConst, const std::pair<Key, Value>*, std::pair<Key, Value>*>;

    buckets_iter bucket_it;
    buckets_iter bucket_end;
    bucket_iter elem_it;

    void go_to_not_empty() {
      while (bucket_it != bucket_end && elem_it == bucket_it->end()) {
        ++bucket_it;
        if (bucket_it != bucket_end) {
          elem_it = bucket_it->begin();
        }
      }
    }

   public:
    iterator_basic(buckets_iter start, buckets_iter end) : bucket_it(start), bucket_end(end) {
      if (bucket_it != bucket_end) {
        elem_it = bucket_it->begin();
      }
      go_to_not_empty();
    }

    reference operator*() { return *elem_it; }

    pointer operator->() { return &(*elem_it); }

    iterator_basic& operator++() {
      ++elem_it;
      go_to_not_empty();
      return *this;
    }

    bool operator==(const iterator_basic& other) {
      if (bucket_it == bucket_end && other.bucket_it == other.bucket_end) {
        return true;
      }
      return bucket_it == other.bucket_it && elem_it == other.elem_it;
    }

    bool operator!=(const iterator_basic& other) { return !(*this == other); }
  };

 public:
  using iterator = iterator_basic<false>;
  using const_iterator = iterator_basic<true>;

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

  // iterators

  iterator begin() { return iterator(buckets.begin(), buckets.end()); }

  iterator end() { return iterator(buckets.end(), buckets.end()); }

  const_iterator begin() const { return const_iterator(buckets.begin(), buckets.end()); }

  const_iterator end() const { return const_iterator(buckets.end(), buckets.end()); }

  const_iterator cbegin() const { return const_iterator(buckets.begin(), buckets.end()); }

  const_iterator cend() const { return const_iterator(buckets.end(), buckets.end()); }
};

}  // namespace my
