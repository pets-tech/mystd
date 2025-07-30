#pragma once

#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace my::heapbased {

/// @brief doubly linked list on linked nodes
template <typename T>
class list {
 public:
  using value_type = T;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using size_type = std::size_t;

 private:
  struct Node {
    value_type data{};
    Node* prev = nullptr;
    Node* next = nullptr;
  };

  Node* head_ = nullptr;
  Node* tail_ = nullptr;
  size_type size_ = 0;

  // iterator implementation
  template <bool IsConst>
  class iterator_basic {
   public:
    using value_type = T;
    using node_type = std::conditional_t<IsConst, const Node, Node>;
    using list_type = std::conditional_t<IsConst, const list, list>;
    using reference = std::conditional_t<IsConst, const T&, T&>;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

    iterator_basic() = default;
    explicit iterator_basic(list_type* owner, node_type* ptr = nullptr) : owner_(owner), ptr_(ptr) {}

    node_type* node() const { return ptr_; }

    reference operator*() const {
      if (!ptr_) throw std::out_of_range("Dereferencing end() iterator");
      return ptr_->data;
    }

    pointer operator->() const {
      if (!ptr_) throw std::out_of_range("Accessing through end() iterator");
      return &ptr_->data;
    }

    iterator_basic& operator++() {
      if (ptr_) ptr_ = ptr_->next;
      return *this;
    }

    iterator_basic& operator--() {
      if (!ptr_) {
        if (!owner_ || !owner_->tail_) throw std::out_of_range("Cannot decrement end() of empty list");
        ptr_ = owner_->tail_;
      } else {
        ptr_ = ptr_->prev;
      }
      return *this;
    }

    iterator_basic operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }
    iterator_basic operator--(int) {
      auto tmp = *this;
      --(*this);
      return tmp;
    }

    bool operator==(const iterator_basic& rhs) const { return ptr_ == rhs.ptr_; }
    bool operator!=(const iterator_basic& rhs) const { return ptr_ != rhs.ptr_; }

   private:
    list_type* owner_ = nullptr;
    node_type* ptr_ = nullptr;
  };

  void free_nodes(Node* node) {
    while (node) {
      Node* next = node->next;
      delete node;
      node = next;
    }
    head_ = tail_ = nullptr;
    size_ = 0;
  }

 public:
  using iterator = iterator_basic<false>;
  using const_iterator = iterator_basic<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  // ctor
  list() = default;

  list(std::initializer_list<value_type> init) {
    for (const auto& val : init) {
      push_back(val);
    }
  }

  // copy
  list(const list& other) {
    for (const auto& val : other) {
      push_back(val);
    }
  }

  list& operator=(const list& other) {
    if (this != &other) {
      list tmp(other);
      swap(tmp);
    }
    return *this;
  }

  // move
  list(list&& other) noexcept
      : head_(std::exchange(other.head_, nullptr)),
        tail_(std::exchange(other.tail_, nullptr)),
        size_(std::exchange(other.size_, 0)) {}

  list& operator=(list&& other) noexcept {
    if (this != &other) {
      clear();
      head_ = std::exchange(other.head_, nullptr);
      tail_ = std::exchange(other.tail_, nullptr);
      size_ = std::exchange(other.size_, 0);
    }
    return *this;
  }

  ~list() { clear(); }

  // element access
  reference front() {
    if (!head_) throw std::out_of_range("front() on empty list");
    return head_->data;
  }

  const_reference front() const {
    if (!head_) throw std::out_of_range("front() on empty list");
    return head_->data;
  }

  reference back() {
    if (!tail_) throw std::out_of_range("back() on empty list");
    return tail_->data;
  }

  const_reference back() const {
    if (!tail_) throw std::out_of_range("back() on empty list");
    return tail_->data;
  }

  // iterator access
  iterator begin() noexcept { return iterator(this, head_); }
  iterator end() noexcept { return iterator(this, nullptr); }

  const_iterator begin() const noexcept { return const_iterator(this, head_); }
  const_iterator end() const noexcept { return const_iterator(this, nullptr); }

  const_iterator cbegin() const noexcept { return const_iterator(this, head_); }
  const_iterator cend() const noexcept { return const_iterator(this, nullptr); }

  reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
  reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

  const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
  const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }

  const_reverse_iterator crbegin() noexcept { return const_reverse_iterator(end()); }
  const_reverse_iterator crend() noexcept { return const_reverse_iterator(begin()); }

  // capacity
  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }

  // modifiers
  iterator insert(iterator pos, const_reference value) {
    Node* curr = pos.node();
    Node* new_node = new Node{value};

    if (!curr) {  // insert at end
      new_node->prev = tail_;
      if (tail_) tail_->next = new_node;
      tail_ = new_node;
      if (!head_) head_ = new_node;
    } else {
      new_node->next = curr;
      new_node->prev = curr->prev;
      if (curr->prev)
        curr->prev->next = new_node;
      else
        head_ = new_node;
      curr->prev = new_node;
    }

    ++size_;
    return iterator(this, new_node);
  }

  iterator insert(const_iterator pos, const_reference value) {
    return insert(iterator(const_cast<Node*>(pos.node())), value);
  }

  iterator erase(iterator pos) {
    Node* node = pos.node();
    if (!node) throw std::out_of_range("Cannot erase end() iterator");

    Node* prev = node->prev;
    Node* next = node->next;

    if (prev)
      prev->next = next;
    else
      head_ = next;

    if (next)
      next->prev = prev;
    else
      tail_ = prev;

    delete node;
    --size_;

    return iterator(this, next);
  }

  void push_front(const_reference value) { insert(begin(), value); }

  void push_back(const_reference value) { insert(end(), value); }

  void pop_front() {
    if (empty()) throw std::out_of_range("pop_front() on empty list");
    erase(begin());
  }

  void pop_back() {
    if (empty()) throw std::out_of_range("pop_back() on empty list");
    iterator tmp = end();
    --tmp;
    erase(tmp);
  }

  void clear() { free_nodes(head_); }

  void swap(list& other) noexcept {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(size_, other.size_);
  }
};

}  // namespace my::heapbased
