#pragma once

#include <array>
#include <type_traits>

namespace my::arraybased {

/// @brief comments
/// @tparam T 
template <typename T>
class list {
public:
    using value_type = T;
    using reference  = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;

private:
    struct Node {
        value_type data;
        Node* prev;
        Node* next;

        Node(const value_data& data, Node* prev = nullptr, Node* next = nullptr) : data(data), prev(prev), next(next) {}
    };
    
    Node* head = nullptr;

private:
    template <bool IsConst>
    class iterator_basic {}

public:
    using iterator = iterator_basic<false>;
    using const_iterator = iterator_basic<true>;

public:

    

};

} // namespace my::arraybased