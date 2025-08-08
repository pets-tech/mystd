#pragma once
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace my::cyclicbufferbased {

template <typename T>
class deque {
 public:
  using value_type = T;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using pointer = value_type*;
  using const_pointer = const value_type*;
  using difference_type = std::ptrdiff_t;

 private:
  pointer buffer_ = nullptr;
  size_type cap_ = 0;
  size_type size_ = 0;
  size_type head_ = 0;

  static constexpr size_type min_capacity = 8;

  size_type phys_index(size_type logical) const noexcept { return (head_ + logical) % cap_; }

  void grow() {
    size_type new_cap = cap_ ? cap_ * 2 : min_capacity;
    pointer new_buf = new value_type[new_cap];

    for (size_type i = 0; i < size_; ++i) {
      new_buf[i] = std::move(buffer_[phys_index(i)]);
    }

    delete[] buffer_;
    buffer_ = new_buf;
    cap_ = new_cap;
    head_ = 0;
  }

 public:
  deque() = default;

  explicit deque(size_type count, const T& value = T()) {
    cap_ = std::max(count, min_capacity);
    buffer_ = new value_type[cap_];
    for (size_type i = 0; i < count; ++i) {
      buffer_[i] = value;
    }
    size_ = count;
  }

  deque(const deque& other) {
    cap_ = other.cap_;
    size_ = other.size_;
    head_ = other.head_;
    buffer_ = new value_type[cap_];
    for (size_type i = 0; i < cap_; ++i) {
      buffer_[i] = other.buffer_[i];
    }
  }

  deque(deque&& other) noexcept : buffer_(other.buffer_), cap_(other.cap_), size_(other.size_), head_(other.head_) {
    other.buffer_ = nullptr;
    other.cap_ = other.size_ = other.head_ = 0;
  }

  deque& operator=(deque other) noexcept {
    swap(other);
    return *this;
  }

  ~deque() { delete[] buffer_; }

  void swap(deque& other) noexcept {
    std::swap(buffer_, other.buffer_);
    std::swap(cap_, other.cap_);
    std::swap(size_, other.size_);
    std::swap(head_, other.head_);
  }

  reference operator[](size_type idx) { return buffer_[phys_index(idx)]; }

  const_reference operator[](size_type idx) const { return buffer_[phys_index(idx)]; }

  reference at(size_type idx) {
    if (idx >= size_) throw std::out_of_range("deque::at");
    return (*this)[idx];
  }

  const_reference at(size_type idx) const {
    if (idx >= size_) throw std::out_of_range("deque::at");
    return (*this)[idx];
  }

  reference front() { return (*this)[0]; }
  const_reference front() const { return (*this)[0]; }

  reference back() { return (*this)[size_ - 1]; }
  const_reference back() const { return (*this)[size_ - 1]; }

  bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }
  size_type capacity() const noexcept { return cap_; }

  void push_back(const T& value) {
    if (size_ == cap_) grow();
    buffer_[phys_index(size_)] = value;
    ++size_;
  }

  void push_back(T&& value) {
    if (size_ == cap_) grow();
    buffer_[phys_index(size_)] = std::move(value);
    ++size_;
  }

  void push_front(const T& value) {
    if (size_ == cap_) grow();
    head_ = (head_ == 0) ? cap_ - 1 : head_ - 1;
    buffer_[head_] = value;
    ++size_;
  }

  void push_front(T&& value) {
    if (size_ == cap_) grow();
    head_ = (head_ == 0) ? cap_ - 1 : head_ - 1;
    buffer_[head_] = std::move(value);
    ++size_;
  }

  void pop_back() {
    if (empty()) throw std::out_of_range("pop_back from empty deque");
    --size_;
  }

  void pop_front() {
    if (empty()) throw std::out_of_range("pop_front from empty deque");
    head_ = (head_ + 1) % cap_;
    --size_;
  }

  void clear() noexcept {
    size_ = 0;
    head_ = 0;
  }

  class iterator {
    deque* d_;
    size_type pos_;

   public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    iterator(deque* d = nullptr, size_type pos = 0) : d_(d), pos_(pos) {}

    reference operator*() const { return (*d_)[pos_]; }
    pointer operator->() const { return &(*d_)[pos_]; }

    iterator& operator++() {
      ++pos_;
      return *this;
    }
    iterator operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }
    iterator& operator--() {
      --pos_;
      return *this;
    }
    iterator operator--(int) {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    iterator& operator+=(difference_type n) {
      pos_ += n;
      return *this;
    }
    iterator& operator-=(difference_type n) {
      pos_ -= n;
      return *this;
    }

    friend iterator operator+(iterator it, difference_type n) {
      it += n;
      return it;
    }
    friend iterator operator-(iterator it, difference_type n) {
      it -= n;
      return it;
    }
    friend difference_type operator-(const iterator& a, const iterator& b) {
      return static_cast<difference_type>(a.pos_) - static_cast<difference_type>(b.pos_);
    }

    reference operator[](difference_type n) const { return (*d_)[pos_ + n]; }

    friend bool operator==(const iterator& a, const iterator& b) { return a.d_ == b.d_ && a.pos_ == b.pos_; }
    friend bool operator!=(const iterator& a, const iterator& b) { return !(a == b); }
    friend bool operator<(const iterator& a, const iterator& b) { return a.pos_ < b.pos_; }
    friend bool operator>(const iterator& a, const iterator& b) { return b < a; }
    friend bool operator<=(const iterator& a, const iterator& b) { return !(b < a); }
    friend bool operator>=(const iterator& a, const iterator& b) { return !(a < b); }
  };

  iterator begin() { return iterator(this, 0); }
  iterator end() { return iterator(this, size_); }
};

}  // namespace my::cyclicbufferbased
