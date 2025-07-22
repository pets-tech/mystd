#pragma once

#include <initializer_list>
#include <stdexcept>
#include <type_traits>
#include <ostream>

namespace my {
namespace stack {

template<typename T, size_t N>
class array {
    static_assert(std::is_default_constructible<T>(), "Array elements must be default-contructoble");

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
    value_type data_[N]{};

public:
    // construction and assignment
    constexpr array() noexcept = default;
    
    constexpr array(const array&) noexcept(std::is_nothrow_copy_constructible_v<value_type>) = default;
    constexpr array& operator=(const array&) noexcept(std::is_nothrow_copy_assignable_v<value_type>) = default;

    array(array&&) = delete;
    array& operator=(array&&) = delete;

    constexpr array(std::initializer_list<value_type> init) {
        if (init.size() > N) throw std::out_of_range("Initializer list exeeds array capacity");

        std::copy(init.begin(), init.end(), data_);
        std::fill(data_ + init.size(), data_ + N, T{});
    }

    // element access

    reference at(size_type pos) {
        if (pos >= N) throw std::out_of_range("pos exeeds array ranges");
        return data_[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= N) throw std::out_of_range("pos exeeds array ranges");
        return data_[pos];        
    }

    constexpr reference operator[](size_type pos) noexcept {
        return data_[pos];
    }

    constexpr const_reference operator[](size_type pos) const noexcept {
        return data_[pos];
    }

    constexpr reference front() noexcept {
        return data_[0];
    }

    constexpr const_reference front() const noexcept {
        return data_[0];
    }

    constexpr reference back() noexcept {
        return data_[N - 1];
    }

    constexpr const_reference back() const noexcept {
        return data_[N - 1];
    }

    constexpr pointer data() noexcept {
        return data_;
    }

    constexpr const_pointer data() const noexcept {
        return data_;
    }

    // iterators
    constexpr iterator begin() noexcept { return data_; }
    constexpr const_iterator begin() const noexcept { return data_; }

    constexpr iterator end() noexcept { return data_ + N; }
    constexpr const_iterator end() const noexcept { return data_ + N; }

    // capacity
    [[nodiscard]] constexpr bool empty() const noexcept { return N == 0; }
    constexpr size_type size() const noexcept { return N;}

    // operations
    constexpr void fill(const_reference value) {
        std::fill_n(data_, N, value);
    }

    constexpr void swap(array& other) {
        std::swap_ranges(data_, data_ + N, other.data_);
    }
};

// non-memers functions

template<typename T, size_t N>
constexpr bool operator==(const array<T, N> lhs, const array<T, N> rhs) {
    return std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T, size_t N>
constexpr bool operator!=(const array<T, N> lhs, const array<T, N> rhs) {
    return !(lhs == rhs);
}

template<typename T, size_t N>
constexpr bool operator<(const array<T, N> lhs, const array<T, N> rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename T, size_t N>
constexpr bool operator>(const array<T, N> lhs, const array<T, N> rhs) {
    return rhs < lhs;
}

template<typename T, size_t N>
constexpr bool operator<=(const array<T, N> lhs, const array<T, N> rhs) {
    return !(rhs < lhs);
}

template<typename T, size_t N>
constexpr bool operator>=(const array<T, N> lhs, const array<T, N> rhs) {
    return !(lhs < rhs);
}

template<typename T, size_t N>
constexpr void swap(const array<T, N> lhs, const array<T, N> rhs) {
    lhs.swap(rhs);
}

template<typename T, size_t N>
std::ostream& operator<< (std::ostream& os, const array<T, N> arr) noexcept{
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
} // end of namespace my