#pragma once
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace my::cyclicbufferbased {

template <typename T>
class deque {
 public:
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using size_type = std::size_t;

 private:
  pointer buffer_ = nullptr;
  size_type capacity_ = 0;
  size_type size_ = 0;
  size_type head_ = 0;

  static constexpr size_type INITIAL_CAPACITY = 8;

  size_type index(size_type offset) const noexcept { return (head_ + offset) % capacity_; }

  void reallocate() {
    size_type new_capacity = capacity_ ? capacity_ * 2 : INITIAL_CAPACITY;
    pointer new_buffer = new value_type[new_capacity];

    for (size_type i = 0; i < size_; ++i) {
      new_buffer[i] = std::move(buffer_[index(i)]);
    }

    delete[] buffer_;
    buffer_ = new_buffer;
    capacity_ = new_capacity;
    head_ = 0;
  }

  template <bool IsConst>
  class iterator_basic {
   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = std::conditional_t<IsConst, const T, T>;
    using difference_type = std::ptrdiff_t;
    using pointer = std::conditional_t<IsConst, const T*, T*>;
    using reference = std::conditional_t<IsConst, const T&, T&>;

    iterator_basic(deque* d = nullptr, size_type pos = 0) : d_(d), pos_(pos) {}

    reference operator*() const { return (*d_)[pos_]; }
    pointer operator->() const { return &(**this); }

    iterator_basic& operator++() {
      ++pos_;
      return *this;
    }
    iterator_basic operator++(int) {
      auto tmp = *this;
      ++(*this);
      return tmp;
    }
    iterator_basic& operator--() {
      --pos_;
      return *this;
    }
    iterator_basic operator--(int) {
      auto tmp = *this;
      --(*this);
      return tmp;
    }

    iterator_basic& operator+=(difference_type n) {
      pos_ += n;
      return *this;
    }
    iterator_basic& operator-=(difference_type n) {
      pos_ -= n;
      return *this;
    }

    reference operator[](difference_type n) const { return (*d_)[pos_ + n]; }

    bool operator==(const iterator_basic& other) const { return pos_ == other.pos_ && d_ == other.d_; }
    bool operator!=(const iterator_basic& other) const { return !(*this == other); }

   private:
    deque* d_;
    size_type pos_;
  };

 public:
  using iterator = iterator_basic<false>;
  using const_iterator = iterator_basic<true>;

  // Constructors
  deque() = default;

  explicit deque(size_type count, const_reference value = value_type()) {
    capacity_ = std::max(count, INITIAL_CAPACITY);
    buffer_ = new value_type[capacity_];
    for (size_type i = 0; i < count; ++i) {
      buffer_[i] = value;
    }
    size_ = count;
    head_ = 0;
  }

  // Copy constructor
  deque(const deque& other) {
    capacity_ = other.capacity_;
    size_ = other.size_;
    head_ = other.head_;
    buffer_ = new value_type[capacity_];
    for (size_type i = 0; i < size_; ++i) {
      buffer_[index(i)] = other.buffer_[other.index(i)];
    }
  }

  // Copy assignment
  deque& operator=(const deque& other) {
    if (this != &other) {
      deque tmp(other);
      swap(tmp);
    }
    return *this;
  }

  // Move constructor
  deque(deque&& other) noexcept
      : buffer_(std::exchange(other.buffer_, nullptr)),
        capacity_(std::exchange(other.capacity_, 0)),
        size_(std::exchange(other.size_, 0)),
        head_(std::exchange(other.head_, 0)) {}

  // Move assignment
  deque& operator=(deque&& other) noexcept {
    if (this != &other) {
      clear();
      buffer_ = std::exchange(other.buffer_, nullptr);
      capacity_ = std::exchange(other.capacity_, 0);
      size_ = std::exchange(other.size_, 0);
      head_ = std::exchange(other.head_, 0);
    }
    return *this;
  }

  ~deque() { clear(); }

  // Capacity
  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }
  size_type capacity() const noexcept { return capacity_; }

  // Element access
  reference operator[](size_type pos) { return buffer_[index(pos)]; }

  const_reference operator[](size_type pos) const { return buffer_[index(pos)]; }

  reference at(size_type pos) {
    if (pos >= size_) throw std::out_of_range("deque::at: out of range");
    return (*this)[pos];
  }

  const_reference at(size_type pos) const {
    if (pos >= size_) throw std::out_of_range("deque::at: out of range");
    return (*this)[pos];
  }

  reference front() {
    if (empty()) throw std::out_of_range("deque::front: empty");
    return buffer_[head_];
  }

  const_reference front() const {
    if (empty()) throw std::out_of_range("deque::front: empty");
    return buffer_[head_];
  }

  reference back() {
    if (empty()) throw std::out_of_range("deque::back: empty");
    return buffer_[index(size_ - 1)];
  }

  const_reference back() const {
    if (empty()) throw std::out_of_range("deque::back: empty");
    return buffer_[index(size_ - 1)];
  }

  // Modifiers
  void push_back(const_reference value) {
    if (size_ == capacity_) reallocate();
    buffer_[index(size_)] = value;
    ++size_;
  }

  void push_front(const_reference value) {
    if (size_ == capacity_) reallocate();
    head_ = (head_ == 0) ? capacity_ - 1 : head_ - 1;
    buffer_[head_] = value;
    ++size_;
  }

  void pop_back() {
    if (empty()) throw std::out_of_range("deque::pop_back: empty");
    --size_;
  }

  void pop_front() {
    if (empty()) throw std::out_of_range("deque::pop_front: empty");
    head_ = (head_ + 1) % capacity_;
    --size_;
  }

  void swap(deque& other) noexcept {
    std::swap(buffer_, other.buffer_);
    std::swap(capacity_, other.capacity_);
    std::swap(size_, other.size_);
    std::swap(head_, other.head_);
  }

  void clear() {
    delete[] buffer_;
    buffer_ = nullptr;
    capacity_ = 0;
    size_ = 0;
    head_ = 0;
  }

  // Iterators
  iterator begin() { return iterator(this, 0); }
  iterator end() { return iterator(this, size_); }
  const_iterator begin() const { return const_iterator(const_cast<deque*>(this), 0); }
  const_iterator end() const { return const_iterator(const_cast<deque*>(this), size_); }
};

}  // namespace my::cyclicbufferbased
