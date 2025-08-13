#pragma once

#include <stdexcept>
#include <utility>
#include <vector>

namespace my::blocksbased {

template <typename T, std::size_t BlockCapacity = 8>
class deque {
  static_assert(BlockCapacity > 0, "BLOCK_SIZE must be > 0");

 public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using size_type = size_t;

 private:
  struct Block {
    T* data;
    Block* prev;
    Block* next;

    Block() : data(new T[BlockCapacity]), prev(nullptr), next(nullptr) {}
    ~Block() { delete[] data; }
  };

  Block* head_ = nullptr;
  Block* tail_ = nullptr;
  size_type head_index_ = BlockCapacity / 2;
  size_type tail_index_ = BlockCapacity / 2;
  size_type size_ = 0;

  Block* make_block() { return new Block(); }

  reference element_access(size_type pos) const noexcept {
    size_type block_index = (pos + head_index_) / BlockCapacity;  // add offset for head_index_ in first block
    size_type room_index = (pos + head_index_) % BlockCapacity;

    Block* cur = head_;
    for (size_t i = 0; i < block_index; ++i) {
      cur = cur->next;
    }
    return cur->data[room_index];
  }

  template <bool IsConst>
  class iterator_basic {
 }

 public : using iterator = iterator_basic<false>;
  using const_iterator = iterator_basic<true>;

  // ctor

  deque() = default;

  deque(std::initializer_list<value_type> init) {
    for (const auto& value : init) {
      push_back(value);
    }
  }

  // copy

  deque(const deque& other) {
    for (size_t i = 0; i < other.size(); ++i) {
      push_back(other.at(i));
    }
  }

  deque& operator=(const deque& other) {
    if (this != &other) {
      this->clear();
      deque tmp(other);
      swap(tmp);
    }
    return *this;
  }

  // move

  deque(deque&& other)
      : head_(std::exchange(other.head_, nullptr)),
        tail_(std::exchange(other.tail_, nullptr)),
        head_index_(std::exchange(other.head_index_, 0)),
        tail_index_(std::exchange(other.tail_index_, 0)),
        size_(std::exchange(other.size_, 0)) {}

  deque& operator=(deque&& other) {
    if (this != &other) {
      deque tmp(std::move(other));
      swap(tmp);
    }
    return *this;
  }

  ~deque() { clear(); }

  // capacity

  bool empty() const noexcept { return size_ == 0; }

  size_type size() const noexcept { return size_; }

  // observers

  reference operator[](size_type pos) noexcept { return element_access(pos); }

  const_reference operator[](size_type pos) const noexcept { return element_access(pos); }

  reference at(size_type pos) {
    if (pos > size_) {
      throw std::out_of_range("deque::at() calls for out of range element");
    }
    return (*this)[pos];
  }

  const_reference at(size_type pos) const {
    if (pos > size_) {
      throw std::out_of_range("deque::at() calls for out of range element");
    }
    return (*this)[pos];
  }

  reference front() { return head_->data[head_index_]; }

  const_reference front() const { return head_->data[head_index_]; }

  reference back() { return tail_->data[tail_index_ - 1]; }

  const_reference back() const { return tail_->data[tail_index_ - 1]; }

  // modifiers

  void push_front(const_reference value) {
    if (!head_) {
      head_ = tail_ = make_block();
    }
    if (head_index_ == 0) {
      Block* block = make_block();
      block->next = head_;
      head_->prev = block;
      head_ = block;
      head_index_ = BlockCapacity;
    }
    --head_index_;
    head_->data[head_index_] = value;
    ++size_;
  }

  void push_back(const_reference value) {
    if (!tail_) {
      head_ = tail_ = make_block();
    }
    if (tail_index_ == BlockCapacity) {
      Block* block = make_block();
      block->prev = tail_;
      tail_->next = block;
      tail_ = block;
      tail_index_ = 0;
    }
    tail_->data[tail_index_] = value;
    ++tail_index_;
    ++size_;
  }

  void pop_front() {
    ++head_index_;
    if (head_index_ == BlockCapacity) {
      auto next = head_->next;
      delete head_;
      head_ = next;
      head_index_ = 0;
    }
    --size_;
  }

  void pop_back() {
    --tail_index_;
    if (tail_index_ = 0) {
      auto prev = tail_->prev;
      delete tail_;
      tail_ = prev;
      tail_index_ = BlockCapacity;
    }
    --size_;
  }

  void clear() {
    while (head_) {
      auto next = head_->next;
      delete head_;
      head_ = next;
    }
    size_ = 0;
    head_ = tail_ = nullptr;
  }

  void swap(deque& other) {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
    std::swap(head_index_, other.head_index_);
    std::swap(tail_index_, other.tail_index_);
    std::swap(size_, other.size_);
  }
};

}  // namespace my::blocksbased
