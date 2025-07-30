#pragma once

#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <type_traits>

namespace my {

template <typename T, size_t N>
class vector {
  static_assert(std::is_default_constructible<T>(), "Type must be default-constructoble");

 public:
  using value_type = T;
  using size_type = size_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = T*;
  using const_pointer = const T*;
  using iterator = T*;
  using const_iterator = const T*;

 private:
  pointer data_;

 public:
  // contruction and assignment

  vector() : data_(new T[N]) {}  // no initial values

  // initializer list
  vector(std::initializer_list<value_type> init) {
    if (init.size() > N) throw std::out_of_range("Initializer list exeeds vector capacity");
    data_ = new T[N];
    std::copy(init.begin(), init.end(), data_);
    std::fill(data_ + init.size(), data_ + N, value_type{});
  }

  ~vector() { delete[] data_; }

  // copy and copy assigment
  vector(const vector& other) {
    data_ = new T[N];
    for (size_t i = 0; i < N; ++i) {
      data_[i] = other.data_[i];
    }
  }

  vector& operator=(const vector& other) {
    if (this != &other) {
      delete[] data_;
      data_ = new T[N];
      for (size_t i = 0; i < N; ++i) {
        data_[i] = other.data_[i];
      }
    }
    return *this;
  }

  // move and move assignment
  vector(vector&& other) = delete;
  vector& operator[](vector&& other) = delete;

  // capacity
  [[nodiscard]] bool empty() const noexcept { return N == 0; }
  size_type size() const noexcept { return N; }

  // element access

  pointer data() { return data_; }
  const_pointer data() const { return data_; }

  reference operator[](size_type index) noexcept { return data_[index]; }
  const_reference operator[](size_type index) const noexcept { return data_[index]; }

  reference at(size_type index) {
    if (index >= N) throw std::out_of_range("");
    return data_[index];
  }
  const_reference at(size_type index) const {
    if (index >= N) throw std::out_of_range("");
    return data_[index];
  }

  // iterators

  iterator begin() { return data_; }

  const_iterator begin() const { return data_; }

  iterator end() { return data_ + N; }

  const_iterator end() const { return data_ + N; }

  void fill(value_type value) {
    for (size_t i = 0; i < N; ++i) {
      data_[i] = value;
    }
  }

  void swap(vector& other) { std::swap_ranges(data_, data_ + N, other.data_); }
};

// non-member methods

template <typename T, size_t N>
bool operator==(const vector<T, N>& lhs, const vector<T, N> rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, size_t N>
bool operator!=(const vector<T, N>& lhs, const vector<T, N> rhs) {
  return !(lhs == rhs);
}

template <typename T, size_t N>
bool operator<(const vector<T, N>& lhs, const vector<T, N> rhs) {
  for (size_t i = 0; i < lhs.size(); ++i) {
    if (lhs[i] >= rhs[i]) {
      return false;
    }
  }
  return true;
}

template <typename T, size_t N>
bool operator>=(const vector<T, N>& lhs, const vector<T, N> rhs) {
  return !(lhs < rhs);
}

template <typename T, size_t N>
bool operator>(const vector<T, N>& lhs, const vector<T, N> rhs) {
  return rhs < lhs;
}

template <typename T, size_t N>
bool operator<=(const vector<T, N>& lhs, const vector<T, N> rhs) {
  return !(lhs > rhs);
}

template <typename T, size_t N>
void swap(const vector<T, N>& lhs, const vector<T, N>& rhs) {
  lhs.swap(rhs);
}

template <typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const vector<T, N>& arr) {
  for (size_t i = 0; i < N; ++i) {
    os << arr[i];
    if (i != N - 1) {
      os << " ";
    }
  }
  os << "\n";
  return os;
}

}  // namespace my
