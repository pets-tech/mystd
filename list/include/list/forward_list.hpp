#pragma once
#include <array>
#include <type_traits>

namespace my {

/// @brief Array based forward list.
template<typename T, size_t Capacity = 10>
class forward_list {
    static_assert(std::is_default_constructible_v<T>, "T must be default constructible");

private:
    // custom nested iterator for forward list
    template<bool IsConst>
    class iterator_basic {
        using list_type = std::conditional_t<IsConst, const forward_list, forward_list>;

        list_type*  list_ptr = nullptr;
        size_t current_index = Capacity;
    
    public:
        using value_type = T;
        using reference = std::conditional_t<IsConst, const T&, T&>;
        using pointer = std::conditional_t<IsConst, const T*, T*>;

        iterator_basic() = default;
        iterator_basic(forward_list* list, size_t idx) : list_ptr(list), current_index(idx) {}

        reference operator*() const {
            return list_ptr->storage[current_index].data;
        }

        pointer operator->() const {
            return &(operator*());
        }

        iterator_basic& operator++() {
            if (current_index == Capacity) {
                throw std::out_of_range("The iterator out of range");
            }
            current_index = list_ptr->storage[current_index].next;
            return *this;
        }
        
        bool operator==(const iterator_basic& other) const = default;
    };

public:
    using value_type = T;
    using size_type = size_t;
    using reference = T&;
    using const_reference = const T&;
    using iterator = iterator_basic<false>;
    using const_iterator = iterator_basic<true>;

private:
    struct Node {
        value_type data{};
        size_type next{};
    };
    std::array<Node, Capacity> storage;
    size_t head{Capacity};
    size_t free_head{0};
    size_t size_{0};
  
public:

    // construction
    forward_list() {
        size_t i = 1;
        for (auto& el : storage) {
            el.next = i++;
        }
        storage[Capacity - 1].next = Capacity; // last element link!
    }

    ~forward_list() = default;

    // copy and copy sassignment
    forward_list(const forward_list& other) {
        if (other.size_ > Capacity) { 
            throw std::runtime_error("Capacity exeeded");
        }
        std::copy(other.storage.begin(), other.storage.end(), storage.begin());
        head = other.head;
        free_head = other.free_head;
        size_ = other.size_;
    }

    forward_list& operator=(const forward_list& other) {
        if (this != &other) {
            forward_list tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // move and move assignment
    forward_list(forward_list&& other) noexcept 
            : storage(std::move(other.storage)), 
            head(other.head), 
            free_head(other.free_head),
            size_(other.size_) {
        other.head = other.capacity();
        other.free_head = 0;
        other.size_ = 0;
    }

    forward_list& operator=(forward_list&& other) noexcept {
        if (this != &other) {
            storage = std::move(other.storage);
            head = other.head;
            free_head = other.free_head;
            size_ = other.size_;
            other.head = other.capacity();
            other.free_head = 0;
            other.size_ = 0;
        }
        return *this;
    }

    // element access
    void push_front(const value_type& data) {
        emplace_front(data);
    }

    void push_front(value_type&& data) {
        emplace_front(std::move(data));
    }
 
    template<typename... Args>
    void emplace_front(Args&&... args) {
        if (size_ >= Capacity) {
            throw std::runtime_error("the list capacity exeeded");
        }
        auto new_head = free_head;
        free_head = storage[free_head].next;

        storage[new_head].data = T(std::forward<Args>(args)...);
        storage[new_head].next = head;

        head = new_head;
        ++size_;
    }

    void pop_front() {
        if (empty()) {
            throw std::runtime_error("The list is empty");
        }
        auto old_head = head;
        head = storage[head].next;

        // save free element list
        storage[old_head].next = free_head;
        free_head = old_head;
        
        --size_;
    }

    // it is better implement erase(iterator) to avoid check for index in range
    void erase(size_type index) {
        if (empty() || index >= Capacity) {
            throw std::runtime_error("can not erase: invalid index");
        }
        if (index == head) {
            pop_front();
            return;
        }

        for (size_t i = 0; i < Capacity; ++i) {
            if (storage[i].next == index) {
                storage[i].next = storage[index].next;
                storage[index].next = free_head;
                free_head = index;
                --size_;
                return;
            }
        }
        throw std::runtime_error("Index not found in the list");
    }

    reference front() noexcept { return storage[head].data; }
    const_reference front() const noexcept { return storage[head].data; }

    // capacity
    size_type size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    static constexpr size_type capacity() noexcept { return Capacity; }

    void swap(forward_list& other) noexcept {
        std::swap(storage, other.storage);
        std::swap(head, other.head);
        std::swap(free_head, other.free_head);
        std::swap(size_, other.size_);
    }

    // iterators
    iterator begin() noexcept { return iterator(this, head); }
    iterator end() noexcept { return iterator(this, Capacity); }
    const_iterator begin() const noexcept { return const_iterator(this, head); }
    const_iterator end() const noexcept { return const_iterator(this, Capacity); }

};

}