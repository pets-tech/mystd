#pragma once

#include <iterator>
#include <type_traits>

namespace my {

// traversal policies

template <typename NodePtr>
struct TreeTraversalPolicy {
  static NodePtr next(NodePtr node) {
    if (!node) return nullptr;
    if (node->right) {
      node = node->right;
      while (node->left) node = node->left;
      return node;
    }
    NodePtr parent = node->parent;
    while (parent && node == parent->right) {
      node = parent;
      parent = parent->parent;
    }
    return parent;
  }

  static NodePtr prev(NodePtr node) {
    if (!node) return nullptr;
    if (node->left) {
      node = node->left;
      while (node->right) node = node->right;
      return node;
    }
    NodePtr parent = node->parent;
    while (parent && node == parent->left) {
      node = parent;
      parent = parent->parent;
    }
    return parent;
  }
};

template <typename NodePtr>
struct ListTraversalPolicy {
  static NodePtr next(NodePtr node) { return node ? node->next : nullptr; }
  static NodePtr prev(NodePtr node) { return node ? node->prev : nullptr; }
};

template <typename NodePtr>
struct ArrayTraversalPolicy {
  static NodePtr next(NodePtr node) { return node ? node + 1 : nullptr; }
  static NodePtr prev(NodePtr node) { return node ? node - 1 : nullptr; }
};

// value extractors

template <typename T>
struct DefaultValueExtractor {
  T& operator()(auto* node) const { return node->value; }
  const T& operator()(const auto* node) const { return node->value; }
};

template <typename T>
struct ArrayValueExtractor {
  T& operator()(T* node) const { return *node; }
  const T& operator()(const T* node) const { return *node; }
};

/// forward iterator
template <typename ValueType, typename NodePtr, bool IsConst = false, typename Category = std::forward_iterator_tag,
          typename TraversalPolicy = TreeTraversalPolicy<NodePtr>,
          typename ValueExtractor = DefaultValueExtractor<ValueType>>
class iterator {
 public:
  using iterator_category = Category;
  using value_type = ValueType;
  using difference_type = std::ptrdiff_t;
  using pointer = std::conditional_t<IsConst, const ValueType*, ValueType*>;
  using reference = std::conditional_t<IsConst, const ValueType&, ValueType&>;

  explicit iterator(NodePtr p = nullptr) : node_(p) {}

  template <bool OtherConst = IsConst, typename = std::enable_if_t<!OtherConst && IsConst>>
  iterator(const iterator<ValueType, NodePtr, false, Category, TraversalPolicy, ValueExtractor>& other)
      : node_(other.base()) {}

  NodePtr base() const { return node_; }

  reference operator*() const { return ValueExtractor()(node_); }
  pointer operator->() const { return std::addressof(ValueExtractor()(node_)); }

  iterator& operator++() {
    node_ = TraversalPolicy::next(node_);
    return *this;
  }

  iterator operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const iterator& other) const { return node_ == other.node_; }
  bool operator!=(const iterator& other) const { return node_ != other.node_; }

 protected:
  NodePtr node_;
};

// bidirectional iterator
template <typename ValueType, typename NodePtr, bool IsConst = false,
          typename Category = std::bidirectional_iterator_tag, typename TraversalPolicy = TreeTraversalPolicy<NodePtr>,
          typename ValueExtractor = DefaultValueExtractor<ValueType>>
class bidirectional_iterator : public iterator<ValueType, NodePtr, IsConst, Category, TraversalPolicy, ValueExtractor> {
  using Base = iterator<ValueType, NodePtr, IsConst, Category, TraversalPolicy, ValueExtractor>;

 public:
  using iterator_category = typename Base::iterator_category;
  using value_type = typename Base::value_type;
  using difference_type = typename Base::difference_type;
  using pointer = typename Base::pointer;
  using reference = typename Base::reference;

  using Base::Base;

  bidirectional_iterator& operator--() {
    this->node_ = TraversalPolicy::prev(this->node_);
    return *this;
  }

  bidirectional_iterator operator--(int) {
    bidirectional_iterator tmp = *this;
    --(*this);
    return tmp;
  }
};

// random access iterator
template <typename ValueType, typename NodePtr, bool IsConst = false,
          typename Category = std::random_access_iterator_tag, typename TraversalPolicy = TreeTraversalPolicy<NodePtr>,
          typename ValueExtractor = DefaultValueExtractor<ValueType>>
class random_access_iterator
    : public bidirectional_iterator<ValueType, NodePtr, IsConst, Category, TraversalPolicy, ValueExtractor> {
  using Base = bidirectional_iterator<ValueType, NodePtr, IsConst, Category, TraversalPolicy, ValueExtractor>;
  using difference_type = typename Base::difference_type;
  using reference = typename Base::reference;

 public:
  using iterator_category = typename Base::iterator_category;
  using value_type = typename Base::value_type;
  using pointer = typename Base::pointer;

  using Base::Base;

  random_access_iterator& operator+=(difference_type n) {
    while (n > 0) {
      this->node_ = TraversalPolicy::next(this->node_);
      --n;
    }
    while (n < 0) {
      this->node_ = TraversalPolicy::prev(this->node_);
      ++n;
    }
    return *this;
  }

  random_access_iterator& operator-=(difference_type n) { return *this += -n; }

  random_access_iterator operator+(difference_type n) const {
    random_access_iterator tmp = *this;
    tmp += n;
    return tmp;
  }

  random_access_iterator operator-(difference_type n) const {
    random_access_iterator tmp = *this;
    tmp -= n;
    return tmp;
  }

  difference_type operator-(const random_access_iterator& other) const {
    difference_type count = 0;
    auto tmp = other.node_;
    while (tmp != this->node_) {
      tmp = TraversalPolicy::next(tmp);
      ++count;
    }
    return count;
  }

  bool operator<(const random_access_iterator& other) const {
    auto tmp = this->node_;
    while (tmp && tmp != other.node_) {
      tmp = TraversalPolicy::next(tmp);
    }
    return tmp != other.node_;
  }

  bool operator>(const random_access_iterator& other) const { return other < *this; }
  bool operator<=(const random_access_iterator& other) const { return !(*this > other); }
  bool operator>=(const random_access_iterator& other) const { return !(*this < other); }

  reference operator[](difference_type n) const {
    random_access_iterator tmp = *this;
    tmp += n;
    return *tmp;
  }
};

}  // namespace my
