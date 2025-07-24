#pragma once

#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <ostream>

namespace my {
namespace heapbased {

template<typename T, size_t N>
class array {
    static_assert(std::is_default_constructible<T>(), "Type must be default-constructoble" );

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

    pointer  data_;

public:

    // contruction and assignment

    array() : data_(new T[N]) {} // no initial values
    
    // initializer list
    array(std::initializer_list<value_type> init) {
        if (init.size() > N) throw std::out_of_range("Initializer list exeeds array capacity");
        data_ = new T[N];
        std::copy(init.begin(), init.end(), data_);
        std::fill(data_ + init.size(), data_ + N, value_type{});
    }

    ~array() {
        delete[] data_;
    }

    // copy and copy assigment
    array(const array& other) {
            data_ = new T[N];
            for (size_t i = 0; i < N; ++i) {
                data_[i] = other.data_[i];
            }
    }

    array& operator=(const array& other) {
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
    array(array&& other) = delete;
    array& operator[](array&& other) = delete;

    // capacity
    [[nodiscard]] bool empty() const noexcept { return N == 0; }
    size_type size() const noexcept { return N; }


    // element access

    pointer data() {
        return data_;
    }
    const_pointer data() const {
        return data_;
    }

    reference operator[](size_type index) noexcept {
        return data_[index];
    }
    const_reference operator[](size_type index) const noexcept {
        return data_[index];
    }

    reference at(size_type index) {
        if (index >= N) throw std::out_of_range("");
        return data_[index];
    }
    const_reference at(size_type index) const {
        if (index >= N) throw std::out_of_range("");
        return data_[index];
    }


    // iterators

    iterator begin() {
        return data_;
    }

    const_iterator begin() const {
        return data_;
    }

    iterator end() {
        return data_ + N;
    }

    const_iterator end() const {
        return data_ + N;
    }

    void fill(value_type value) {
        for (size_t i = 0; i < N; ++i) {
            data_[i] = value;
        }
    }

    void swap(array& other) {
        std::swap_ranges(data_, data_ + N, other.data_);
    }
    
};

// non-member methods

template<typename T, size_t N>
bool operator==(const array<T, N>& lhs, const array<T, N> rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, size_t N>
bool operator!=(const array<T, N>& lhs, const array<T, N> rhs) {
    return !(lhs == rhs);
}

template<typename T, size_t N>
bool operator<(const array<T, N>& lhs, const array<T, N> rhs) {
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] >= rhs[i]) {
            return false;
        }
    }
    return true;
}

template<typename T, size_t N>
bool operator>=(const array<T, N>& lhs, const array<T, N> rhs) {
    return !(lhs < rhs);
}

template<typename T, size_t N>
bool operator>(const array<T, N>& lhs, const array<T, N> rhs) {
    return rhs < lhs;
}

template<typename T, size_t N>
bool operator<=(const array<T, N>& lhs, const array<T, N> rhs) {
    return !(lhs > rhs);
}

template<typename T, size_t N>
void swap(const array<T, N>& lhs, const array<T, N>& rhs) {
    lhs.swap(rhs);
}

template<typename T, size_t N>
std::ostream& operator<<(std::ostream& os, const array<T, N>& arr) {
    for (size_t i = 0; i < N; ++i) {
        os << arr[i];
        if (i != N - 1) {
            os << " ";
        }
    }
    os << "\n";
    return os;
} 

}
}