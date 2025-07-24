#pragma once

#include <array>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace my::arraybased {

/// @brief Fixed-capacity doubly linked lisk on static array.
template <typename T, std::size_t Capacity = 1000>
class list {
public:
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = std::size_t;
    using iterator_category = std::forward_iterator_tag;

private:
    static constexpr size_type npos = Capacity;

    struct Node {
        value_type data{};
        size_type prev{npos};
        size_type next{npos};
    };

    std::array<Node, Capacity> storage_;
    size_type head_ = npos;
    size_type tail_ = npos;
    size_type head_free_ = 0;
    size_type size_ = 0;

    template <bool IsConst>
    class iterator_basic {
    public:
        using list_type = std::conditional_t<IsConst, const list, list>;
        using node_type = std::conditional_t<IsConst, const Node, Node>;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using reference  = std::conditional_t<IsConst, const T&, T&>;
        using pointer    = std::conditional_t<IsConst, const T*, T*>;
        using iterator_category = std::forward_iterator_tag;

        iterator_basic() = default;
        explicit iterator_basic(list_type* lst, size_type idx) : list_(lst), current_(idx) {};
        
        reference operator*() const { return list_->storage_[current_].data; }
        pointer operator->() const { return &(**this); }

        iterator_basic& operator++() {
            current_ = list_->storage_[current_].next;
            return *this;
        }

        reference operator++(int) {
            iterator_basic tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator_basic& operator--() {
            current_ = list_->storage_[current_].prev;
            return *this;
        }
        reference operator--(int) {
            iterator_basic tmp = *this;
            --(*this);
            return tmp;
        }


        bool operator==(const iterator_basic& rhs) const { 
            return current_ == rhs.current_;
        }
        bool operator!=(const iterator_basic& rhs) const {
            return !(*this == rhs);
        }
        
        size_type index() const { return current_; }

    private:
        list_type* list_ = nullptr;
        size_type current_ = npos;
    };

public:
    using iterator = iterator_basic<false>;
    using const_iterator = iterator_basic<true>;

    list() {
        for (size_type i = 0; i < Capacity; ++i) {
            storage_[i].prev = i - 1;
            storage_[i].next = i + 1;
        }
        storage_[0].prev = npos;
        storage_[Capacity - 1].next = npos;
    };

    list(const std::initializer_list<value_type>& init) : list() {
        for (const auto& val : init) {
            push_back(val);
        }
    }

    // copy
    list(const list& other) = default;
    list& operator=(const list& other) {
        if (this != &other) {
            list tmp(other);
            swap(tmp);
        }
        return *this;
    }

    // move
    list(list&& other) noexcept 
        : storage_(std::move(other.storage_)),
        head_(other.head_), 
        tail_(other.tail_), 
        size_(other.size_) 
    {
        other.head_ = npos;
        other.tail_ = npos;
        other.size_ = 0;
    }
    list& operator=(list&& other) noexcept {
        if (this != &other) {
            list tmp(std::move(other));
            swap(tmp);
            other.head_ = npos;
            other.tail_ = npos;
            other.size_ = 0;            
        }
        return *this;
    }

    ~list() = default;

    // element access
    reference front() { return storage_[head_].data; }
    reference back() { return storage_[tail_].data; }
    const_reference front() const { return storage_[head_].data; }
    const_reference back() const { return storage_[tail_].data; }

    // capacity
    bool empty() const noexcept { return size_ == 0; }
    size_type size() const noexcept { return size_; }
    size_type max_size() const noexcept { return Capacity; };

    // modifiers
    void clear() {
        head_ = tail_ = npos;
        head_free_ = 0;
        size_ = 0;

        for (size_type i = 0; i < Capacity; ++i) {
            storage_[i].prev = i - 1;
            storage_[i].next = i + 1;
        }

        storage_[0].prev = Capacity;
        storage_[Capacity - 1].next = Capacity;
    }

    
    // 0 1 2 3 4 5 npos
    // ^head_free_
    //             ^head_
    //             ^tail_
    iterator insert(iterator pos, const_reference value) {
        if (head_free_ == npos) {
            throw std::overflow_error("List is full");
        }

        // allocate place in array
        size_type new_idx = head_free_;
        head_free_ = storage_[head_free_].next;

        // add data
        storage_[new_idx].data = value;

        // update links
        if (pos == begin()) { // if head
            storage_[new_idx].prev = npos;
            storage_[new_idx].next = head_;
            if (head_ != npos) storage_[head_].prev = new_idx;
            head_ = new_idx;
            if (tail_ == npos) tail_ = new_idx;
        } else if (pos == end()) { // if tail
            storage_[new_idx].prev = tail_;
            storage_[new_idx].next = npos;
            if (tail_ != npos) storage_[tail_].next = new_idx;
            tail_ = new_idx;
            if (head_ == npos) head_ = new_idx;
        } else { // if middle
            size_type next_idx = pos.index();
            size_type prev_idx = storage_[next_idx].prev;

            // new node
            storage_[new_idx].prev = prev_idx;
            storage_[new_idx].next = next_idx;

            // update links for new node
            storage_[prev_idx].next = new_idx;
            storage_[next_idx].prev = new_idx;
        }

        ++size_;
        return iterator(this, new_idx);
    }

    iterator erase(iterator pos) {
        if (pos == end()) pos = --end();
        size_type idx = pos.index();

        if (idx == head_) head_ = storage_[idx].next;
        if (idx == tail_) tail_ = storage_[idx].prev;

        if (storage_[idx].prev != npos) storage_[storage_[idx].prev].next = storage_[idx].next;
        if (storage_[idx].next != npos) storage_[storage_[idx].next].prev = storage_[idx].prev;


        // deallocate
        storage_[idx].data = 0;
        storage_[idx].next = head_free_;
        head_free_ = idx;

        --size_;
        return iterator(this, storage_[idx].next);
    }

    void push_front(const_reference val) { insert(begin(), val); }
    void push_back(const_reference val)  { insert(end(), val); }
    void pop_front() { erase(begin()); }
    void pop_back()  { erase(--end()); }

    void swap(list& other) noexcept {
        std::swap(storage_, other.storage_);
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(head_free_, other.head_free_);
        std::swap(size_, other.size_);
    }

    // iterators
    iterator begin() noexcept { return iterator(this, head_); }
    iterator end() noexcept { return iterator(this, npos); }
    const_iterator begin() const noexcept { return const_iterator(this, head_); }
    const_iterator end() const noexcept { return const_iterator(this, npos); }
    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }
};

} // namespace my::arraybased