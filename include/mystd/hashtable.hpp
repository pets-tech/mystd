#pragma once

#include <functional>
#include <memory>
#include <utility>
#include <vector>

namespace my {

/// @brief The policy for map/set multimap/multiset.
enum class InsertPolicy { UniqueKeys, AllowDuplicates };

/// @brief The core of others hash tables based data structures.
/// @tparam Value
/// @tparam Key
/// @tparam KeyOfValue -- rule how to extract Key
/// @tparam Hash -- rule how to compute hash of Key
/// @tparam KeyEqual -- rule how to equal to Key
/// @tparam Policy -- rule if Key is unique or not
template <class Value, class Key, class KeyOfValue, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<Key>,
          InsertPolicy Policy = InsertPolicy::UniqueKeys, class Allocator = std::allocator<Value>>
class hashtable {
 public:
  using key_type = Key;
  using value_type = Value;
  using reference = Value&;
  using const_reference = const Value&;
  using size_type = std::size_t;

  static constexpr float max_load_factor = 0.75f;
  static constexpr size_type reallocation_factor = 2;

 private:
  struct Node {
    Value value;
    size_type hash;
    Node* next;

    Node(const_reference v, size_type h, Node* n = nullptr) : value(v), hash(h), next(n) {}
  };

  using node_allocator_type = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;

  std::vector<Node*> buckets;
  size_type size_ = 0;
  Hash hasher;
  KeyEqual equal;
  KeyOfValue key_of_value;
  node_allocator_type alloc;

  size_type get_index_(const key_type& k, size_type bucket_count) const { return hasher(k) % bucket_count; }

  void rehash_() {
    if (load_factor() > max_load_factor) {
      rehash(buckets.size() * reallocation_factor);
    }
  }

 public:
  template <bool IsConst>
  class iterator_basic {
   public:
    using iterator_category = std::forward_iterator_tag;
    using value_type = std::conditional_t<IsConst, const Value, Value>;
    using pointer = std::conditional_t<IsConst, const Value*, Value*>;
    using reference = std::conditional_t<IsConst, const Value&, Value&>;
    using difference_type = std::ptrdiff_t;
    using BucketsVector = std::conditional_t<IsConst, const std::vector<Node*>, std::vector<Node*>>;

   private:
    Node* node;
    BucketsVector* buckets;
    size_type bucket_idx;

    void skip_empty() {
      while (!node && bucket_idx + 1 < buckets->size()) {
        node = (*buckets)[++bucket_idx];
      }
    }

   public:
    iterator_basic(Node* n, BucketsVector* b, size_type i) : node(n), buckets(b), bucket_idx(i) {
      if (!node) {
        skip_empty();
      }
    }

    reference operator*() const { return node->value; }

    pointer operator->() const { return &node->value; }

    iterator_basic& operator++() {
      if (node) {
        node = node->next;
        if (!node) {
          skip_empty();
          if (!node) {
            bucket_idx = buckets->size();
          }
        }
      }
      return *this;
    }

    bool operator==(const iterator_basic& other) const {
      return node == other.node && bucket_idx == other.bucket_idx && buckets == other.buckets;
    }
    bool operator!=(const iterator_basic& other) const { return !(*this == other); }
  };

  using iterator = iterator_basic<false>;
  using const_iterator = iterator_basic<true>;

  // ctor
  explicit hashtable(size_type bucket_count = 8)
      : buckets(bucket_count, nullptr), size_(0), hasher(), equal(), key_of_value(), alloc() {}

  // copy
  hashtable(const hashtable& other)
      : buckets(other.buckets.size(), nullptr),
        size_(0),
        hasher(other.hasher),
        equal(other.equal),
        key_of_value(other.key_of_value),
        alloc(other.alloc) {
    for (size_type i = 0; i < other.buckets.size(); ++i) {
      Node* src = other.buckets[i];
      Node** dst = &buckets[i];
      while (src) {
        Node* new_node = alloc.allocate(1);
        std::allocator_traits<node_allocator_type>::construct(alloc, new_node, src->value, src->hash);
        *dst = new_node;
        dst = &new_node->next;
        src = src->next;
        ++size_;
      }
    }
  }

  // copy assign
  hashtable& operator=(const hashtable& other) {
    if (this != &other) {
      hashtable tmp(other);
      swap(tmp);
    }
    return *this;
  }

  // move
  hashtable(hashtable&& other) noexcept {
    swap(other);  // very pretty! because new consistent
                  // empty object created (default ctor do it)
                  // and then swaped
  }

  hashtable& operator=(hashtable&& other) {
    if (this != &other) {
      clear();
      swap(other);
    }
    return *this;
  }

  ~hashtable() { clear(); }

  void clear() {
    for (Node* node : buckets) {
      while (node) {
        Node* tmp = node->next;
        std::allocator_traits<node_allocator_type>::destroy(alloc, node);
        alloc.deallocate(node, 1);
        node = tmp;
      }
    }
    // leave the object in consistent state
    buckets.assign(buckets.size(), nullptr);
    size_ = 0;
  }

  size_type count(const key_type& k) const {
    size_type h = hasher(k);
    size_type idx = h % buckets.size();

    auto cur = buckets[idx];

    size_type count = 0;
    while (cur) {
      if (cur->hash == h && equal(key_of_value(cur->value), k)) {
        ++count;
      }
      cur = cur->next;
    }
    return count;
  }

  std::pair<iterator, bool> insert(const_reference v) {
    const key_type& k = key_of_value(v);
    size_type h = hasher(k);
    size_type idx = h % buckets.size();

    if constexpr (Policy == InsertPolicy::UniqueKeys) {
      for (Node* node = buckets[idx]; node; node = node->next) {
        if (node->hash == h && equal(key_of_value(node->value), k)) {
          return {iterator(node, &buckets, idx), false};
        }
      }
    }

    Node* new_node = alloc.allocate(1);
    std::allocator_traits<node_allocator_type>::construct(alloc, new_node, v, h, buckets[idx]);
    buckets[idx] = new_node;
    ++size_;

    rehash_();

    return {iterator(new_node, &buckets, idx), true};
  }

  iterator find(const key_type& k) {
    size_type h = hasher(k);
    size_type idx = h % buckets.size();
    for (Node* node = buckets[idx]; node; node = node->next) {
      if (node->hash == h && equal(key_of_value(node->value), k)) {
        return iterator(node, &buckets, idx);
      }
    }
    return end();
  }

  /// @return iterator to after removed element
  iterator erase(const key_type& k) {
    size_type h = hasher(k);
    size_type idx = h % buckets.size();

    Node* cur = buckets[idx];
    Node* prev = nullptr;

    while (cur) {
      if (cur->hash == h && equal(key_of_value(cur->value), k)) {
        Node* next = cur->next;
        if (prev) {
          prev->next = next;
        } else {  // first element case
          buckets[idx] = next;
        }

        std::allocator_traits<node_allocator_type>::destroy(alloc, cur);
        alloc.deallocate(cur, 1);
        --size_;

        if (next) {
          return iterator(next, &buckets, idx);
        } else {  // if bucket become empty find from start
          return iterator(nullptr, &buckets, idx);
        }
      }
      prev = cur;
      cur = cur->next;
    }

    return end();
  }

  // iterators
  iterator begin() { return iterator(nullptr, &buckets, 0); }
  iterator end() { return iterator(nullptr, &buckets, buckets.size()); }

  const_iterator begin() const { return const_iterator(nullptr, &buckets, 0); }
  const_iterator end() const { return const_iterator(nullptr, &buckets, buckets.size()); }

  const_iterator cbegin() const { return const_iterator(nullptr, &buckets, 0); }
  const_iterator cend() const { return const_iterator(nullptr, &buckets, buckets.size()); }

  // capacity and others

  size_type size() const { return size_; }
  bool empty() const { return size_ == 0; }
  size_type bucket_count() const { return buckets.size(); }

  float load_factor() const { return static_cast<float>(size_) / buckets.size(); }

  void rehash(size_type new_count) {
    std::vector<Node*> new_buckets(new_count, nullptr);

    for (Node* node : buckets) {
      while (node) {
        Node* next = node->next;
        size_type idx = node->hash % new_count;
        node->next = new_buckets[idx];
        new_buckets[idx] = node;
        node = next;
      }
    }

    buckets.swap(new_buckets);
  }

  void swap(hashtable& other) noexcept {
    std::swap(buckets, other.buckets);
    std::swap(size_, other.size_);
    std::swap(hasher, other.hasher);
    std::swap(equal, other.equal);
    std::swap(key_of_value, other.key_of_value);
    std::swap(alloc, other.alloc);
  }

  // ADL
  friend void swap(hashtable& a, hashtable& b) noexcept { a.swap(b); }
};

}  // namespace my
