#pragma once

#include <iterator>
#include <stdexcept>
#include <utility>

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

  // it is o(n) now
  // TODO save pointers to blocks and got O(1)
  reference element_access(size_type pos) const {
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
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<IsConst, const T, T>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using reference = std::conditional_t<IsConst, const T&, T&>;

    iterator_basic(deque* d = nullptr, std::size_t index = 0) : d_(d), index_(index) {};

    reference operator*() const { return (*d_)[index_]; }

    pointer operator->() const { return &(**this); }

    iterator_basic& operator++() {
      ++index_;
      return *this;
    }

    iterator_basic operator++(int) {
      deque tmp = *this;
      ++(*this);
      return tmp;
    }

    iterator_basic& operator--() {
      --index_;
      return *this;
    }

    iterator_basic operator--(int) {
      auto tmp = *this;
      --index_;
      return tmp;
    }

    iterator_basic& operator+=(difference_type n) {
      index_ += n;
      return *this;
    }

    iterator_basic operator+(difference_type n) const {
      auto tmp = *this;
      return tmp += n;
    }

    iterator_basic& operator-=(difference_type n) {
      index_ -= n;
      return *this;
    }

    iterator_basic operator-(difference_type n) const {
      auto tmp = *this;
      return tmp -= n;
    }

    // TODO test it very well!
    difference_type operator-(const iterator_basic& other) const {
      return static_cast<difference_type>(index_) - static_cast<difference_type>(other.index_);
    }

    bool operator==(const iterator_basic& other) const { return index_ == other.index_ && d_ == other.d_; }

    bool operator!=(const iterator_basic& other) const { return !(*this == other); }

    bool operator<(const iterator_basic& other) const { return index_ < other.index_; }

    bool operator>(const iterator_basic& other) const { return other < *this; }

    bool operator<=(const iterator_basic& other) const { return !(other < *this); }

    bool operator>=(const iterator_basic& other) const { return !(*this < other); }

   private:
    deque* d_;
    std::size_t index_;
  };

 public:
  using iterator = iterator_basic<false>;
  using const_iterator = iterator_basic<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

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
    if (pos >= size_) {
      throw std::out_of_range("deque::at() calls for out of range element");
    }
    return (*this)[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= size_) {
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
      if (head_) {
        head_->prev = nullptr;
      } else {
        tail_ = nullptr;  // makes deque consistant (if delete last element then head_ == tail_)
      }
      head_index_ = 0;
    }
    --size_;
  }

  void pop_back() {
    --tail_index_;
    if (tail_index_ == 0) {
      auto prev = tail_->prev;
      delete tail_;
      tail_ = prev;
      if (tail_) {
        tail_->next = nullptr;
      } else {
        head_ = nullptr;  // makes deque consistant (if delete last element then head_ == tail_)
      }
      tail_index_ = BlockCapacity;
    }
    --size_;
  }

  // iterators

  iterator begin() { return iterator(this, 0); }
  iterator end() { return iterator(this, size()); }

  const_iterator begin() const { return const_iterator(this, 0); }
  const_iterator end() const { return const_iterator(this, size()); }

  const_iterator cbegin() const { return const_iterator(this, 0); }
  const_iterator cend() const { return const_iterator(this, size()); }

  reverse_iterator rbegin() { return reverse_iterator(end()); }
  reverse_iterator rend() { return reverse_iterator(begin()); }

  const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }

  const_reverse_iterator crbegin() const { return const_reverse_iterator(end()); }
  const_reverse_iterator crend() const { return const_reverse_iterator(begin()); }

  // others

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
