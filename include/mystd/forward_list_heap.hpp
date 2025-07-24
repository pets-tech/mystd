#pragma once

#include <initializer_list>
#include <type_traits>
#include <utility>
#include <stdexcept>
#include <iterator>

namespace my::heapbased {

/// @brief Heap based forward list.
template <typename T>
class forward_list {
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
        Node* next;

        Node(const value_type& val, Node* next = nullptr)
            : data(val), next(next) {}
    };

    Node* head_ = nullptr;

    template <bool IsConst>
    class iterator_basic {
        using node_type = std::conditional_t<IsConst, const Node, Node>;

        node_type* ptr_ = nullptr;

    public:
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference  = std::conditional_t<IsConst, const T&, T&>;
        using pointer    = std::conditional_t<IsConst, const T*, T*>;

        explicit iterator_basic(node_type* ptr = nullptr) : ptr_(ptr) {}
        iterator_basic(const iterator_basic&) = default;

        node_type* node() const {
            return ptr_;
        }

        reference operator*() const { return ptr_->data; }
        pointer operator->() const { return &ptr_->data; }

        iterator_basic& operator++() {
            if (ptr_) ptr_ = ptr_->next;
            return *this;
        }

        iterator_basic operator++(int) {
            iterator_basic tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const iterator_basic& rhs) const { return ptr_ == rhs.ptr_; }
        bool operator!=(const iterator_basic& rhs) const { return ptr_ != rhs.ptr_; }
    };

public:
    using iterator_category = std::forward_iterator_tag;
    using iterator  = iterator_basic<false>;
    using const_iterator = iterator_basic<true>;

    forward_list() = default;

    forward_list(std::initializer_list<value_type> init) {
        for (auto it = std::rbegin(init); it != std::rend(init); ++it) {
            push_front(*it);
        }
    }

    forward_list(const forward_list& other) {
        if (!other.head_) return;

        head_ = new Node(other.head_->data);
        Node* curr = head_;
        Node* other_node = other.head_->next;
        while (other_node) {
            curr->next = new Node(other_node->data);
            other_node = other_node->next;
            curr = curr->next;
        }
    }

    forward_list& operator=(const forward_list& other) {
        if (this != &other) {
            forward_list tmp(other);
            swap(tmp);
        }
        return *this;
    }

    forward_list(forward_list&& other) noexcept
        : head_(std::exchange(other.head_, nullptr)) {}

    forward_list& operator=(forward_list&& other) noexcept {
        if (this != &other) {
            clear();
            head_ = std::exchange(other.head_, nullptr);
        }
        return *this;
    }

    ~forward_list() noexcept {
        clear();
    }

    // element access
    const_reference front() const {
        if (!head_) {
            throw std::runtime_error("list is empty");
        }
        return head_->data;
    }

    // capacity
    bool empty() const noexcept {
        return head_ == nullptr;
    }

    // modifiers
    void clear() noexcept {
        free_nodes(head_);
        head_ = nullptr;
    }

    iterator erase(iterator it) {
        if (it == end()) {
            return end();
        }

        if (it == begin()) {
            pop_front();
            return begin();
        }
                                            
        iterator prev = begin();
        while (prev != end() && std::next(prev) != it) {
            ++prev;
        }

        if (prev != end()) {
            Node* to_delete = prev.node()->next;
            prev.node()->next = to_delete->next;
            delete to_delete;
            return iterator(prev.node()->next);
        }
        return end();
    }

    void push_front(const value_type& value) {
        head_ = new Node(value, head_);
    }

    void pop_front() {
        if (!head_) {
            throw std::runtime_error("the list is empty");
        };
        Node* tmp = head_;
        head_ = head_->next;
        delete tmp;
    }

    void reverse() noexcept {
        Node* prev = nullptr;
        Node* curr = head_;
        while (curr) {
            Node* next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }
        head_ = prev;
    }

    void swap(forward_list& other) noexcept {
        std::swap(head_, other.head_);
    }

    // iterators
    iterator begin() noexcept { return iterator(head_); }
    iterator end() noexcept   { return iterator(nullptr); }

    const_iterator begin() const noexcept { return const_iterator(head_); }
    const_iterator end() const noexcept   { return const_iterator(nullptr); }

    const_iterator cbegin() const noexcept { return const_iterator(head_); }
    const_iterator cend() const noexcept   { return const_iterator(nullptr); }

private:
    void free_nodes(Node* node) noexcept {
        while (node) {
            Node* next = node->next;
            delete node;
            node = next;
        }
    }
};

} // namespace my::heapbased
