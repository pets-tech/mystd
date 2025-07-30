#pragma once

#include <initializer_list>
#include <ostream>
#include <stdexcept>
#include <type_traits>

namespace my {

const size_t CAPACITY = 32;
const size_t REALLICATION_FACTOR = 1.5;

template <typename T, size_t Capacity = CAPACITY>
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
  size_type size_ = 0;
  size_type capacity_ = Capacity;
  size_type reallocation_factor_ = REALLICATION_FACTOR;

  void reallocate(size_type new_capacity) {
    value_type* new_data = new value_type[new_capacity];
    std::move(data_, data_ + size_, new_data);
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
  }

 public:
  // contruction and assignment

  vector() : data_(new value_type[Capacity]), size_(Capacity), capacity_(Capacity) {}

  // initializer list
  vector(std::initializer_list<value_type> init)
      : data_(new T[init.size()]), size_(init.size()), capacity_(init.size()) {
    std::copy(init.begin(), init.end(), data_);
  }

  ~vector() { delete[] data_; }

  // copy and copy assigment
  vector(const vector& other) {
    const size_t other_size = other.size();
    data_ = new T[other_size];
    for (size_t i = 0; i < other_size; ++i) {
      data_[i] = other.data_[i];
    }
    size_ = other_size;
  }

  vector& operator=(const vector& other) {
    if (this != &other) {
      const size_t other_size = other.size();
      delete[] data_;
      data_ = new T[other_size];
      for (size_t i = 0; i < other_size; ++i) {
        data_[i] = other.data_[i];
      }
      size_ = other_size;
    }
    return *this;
  }

  // move and move assignment
  vector(vector&& other) noexcept : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  vector& operator=(vector&& other) noexcept {
    if (this != &other) {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }
    return *this;
  }

  // capacity
  [[nodiscard]] bool empty() const noexcept { return size_ == 0; }
  size_type size() const noexcept { return size_; }
  size_type capacity() const noexcept { return capacity_; }

  // element access

  pointer data() { return data_; }
  const_pointer data() const { return data_; }

  reference operator[](size_type index) noexcept { return data_[index]; }
  const_reference operator[](size_type index) const noexcept { return data_[index]; }

  reference at(size_type index) {
    if (index >= size_) throw std::out_of_range("");
    return data_[index];
  }
  const_reference at(size_type index) const {
    if (index >= size_) throw std::out_of_range("");
    return data_[index];
  }

  // iterators

  iterator begin() { return data_; }

  const_iterator begin() const { return data_; }

  iterator end() { return data_ + size_; }

  const_iterator end() const { return data_ + size_; }

  void fill(value_type value) {
    for (size_t i = 0; i < size_; ++i) {
      data_[i] = value;
    }
  }

  void swap(vector& other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
  }
};

// non-member methods

template <typename T, size_t Capacity = CAPACITY>
bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
  return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <typename T, size_t Capacity = CAPACITY>
bool operator!=(const vector<T>& lhs, const vector<T> rhs) {
  return !(lhs == rhs);
}

template <typename T, size_t Capacity = CAPACITY>
bool operator<(const vector<T>& lhs, const vector<T> rhs) {
  for (size_t i = 0; i < lhs.size(); ++i) {
    if (lhs[i] >= rhs[i]) {
      return false;
    }
  }
  return true;
}

template <typename T, size_t Capacity = CAPACITY>
bool operator>=(const vector<T>& lhs, const vector<T> rhs) {
  return !(lhs < rhs);
}

template <typename T, size_t Capacity = CAPACITY>
bool operator>(const vector<T>& lhs, const vector<T> rhs) {
  return rhs < lhs;
}

template <typename T, size_t Capacity = CAPACITY>
bool operator<=(const vector<T>& lhs, const vector<T> rhs) {
  return !(lhs > rhs);
}

template <typename T, size_t Capacity = CAPACITY>
void swap(vector<T>& lhs, vector<T>& rhs) {
  lhs.swap(rhs);
}

template <typename T, size_t Capacity = CAPACITY>
std::ostream& operator<<(std::ostream& os, const vector<T>& arr) {
  const size_t N = arr.size();
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
