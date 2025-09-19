#pragma once

#include <iterator>
#include <type_traits>

namespace my {

/// base stupid interface, because may be used instead
/// more low iterator category (input, output, etc.)
template <typename T, bool IsConst, typename Category>
struct iterator {
  using iterator_category = Category;
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using reference = std::conditional_t<IsConst, const T&, T&>;
  using pointer = std::conditional_t<IsConst, const T*, T*>;

  pointer base() const;
};

// forward (++ only)
template <typename T, bool IsConst = false, typename Category = std::forward_iterator_tag>
class forward_iterator : public iterator<T, IsConst, Category> {
 protected:
  using Base = iterator<T, IsConst, Category>;

 public:
  using iterator_category = Category;
  using value_type = typename Base::value_type;
  using difference_type = typename Base::difference_type;
  using reference = typename Base::reference;
  using pointer = typename Base::pointer;

  pointer ptr;

 public:
  explicit forward_iterator(pointer p = nullptr) : ptr(p) {}
  forward_iterator(const forward_iterator<T, false, Category>& other) : ptr(other.base()) {}

  pointer base() const { return this->ptr; }

  reference operator*() const { return *ptr; }
  pointer operator->() const { return ptr; }

  forward_iterator& operator++() {
    ++ptr;
    return *this;
  }

  forward_iterator operator++(int) {
    forward_iterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const forward_iterator& other) const { return ptr == other.ptr; }
  bool operator!=(const forward_iterator& other) const { return ptr != other.ptr; }
};

// bidirectional iterator, add --
template <typename T, bool IsConst = false, typename Category = std::bidirectional_iterator_tag>
class bidirectional_iterator : public forward_iterator<T, IsConst, Category> {
 protected:
  using Base = forward_iterator<T, IsConst, Category>;

 public:
  using iterator_category = Category;
  using value_type = typename Base::value_type;
  using difference_type = typename Base::difference_type;
  using reference = typename Base::reference;
  using pointer = typename Base::pointer;

 public:
  explicit bidirectional_iterator(pointer p = nullptr) : Base(p) {}
  bidirectional_iterator(const bidirectional_iterator<T, false, Category>& other) : Base(other.base()) {}

  bidirectional_iterator& operator--() {
    --this->ptr;
    return *this;
  }

  bidirectional_iterator operator--(int) {
    bidirectional_iterator tmp = *this;
    --(*this);
    return tmp;
  }
};

// random access iterator, add +=, -=, +, -, [], <,>,<=,>=
template <typename T, bool IsConst = false, typename Category = std::random_access_iterator_tag>
class random_access_iterator : public bidirectional_iterator<T, IsConst, Category> {
 protected:
  using Base = bidirectional_iterator<T, IsConst, Category>;

 public:
  using iterator_category = Category;
  using value_type = typename Base::value_type;
  using difference_type = typename Base::difference_type;
  using reference = typename Base::reference;
  using pointer = typename Base::pointer;

 public:
  explicit random_access_iterator(pointer p = nullptr) : Base(p) {}
  random_access_iterator(const random_access_iterator<T, false, Category>& other) : Base(other.base()) {}

  random_access_iterator& operator+=(difference_type n) {
    this->ptr += n;
    return *this;
  }

  random_access_iterator& operator-=(difference_type n) {
    this->ptr -= n;
    return *this;
  }

  random_access_iterator operator+(difference_type n) const { return random_access_iterator(this->ptr + n); }

  random_access_iterator operator-(difference_type n) const { return random_access_iterator(this->ptr - n); }

  difference_type operator-(const random_access_iterator& other) const { return this->ptr - other.ptr; }

  bool operator<(const random_access_iterator& other) const { return this->ptr < other.ptr; }

  bool operator>(const random_access_iterator& other) const { return this->ptr > other.ptr; }

  bool operator<=(const random_access_iterator& other) const { return this->ptr <= other.ptr; }

  bool operator>=(const random_access_iterator& other) const { return this->ptr >= other.ptr; }
};

}  // namespace my
