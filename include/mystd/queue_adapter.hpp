#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <type_traits>
#include <utility>

namespace my {

/// @brief The queue is a container adapter that provides a First-In-First-Out (FIFO) interface;
///
/// The queue has limited (relative container which is based) interface includes:
/// - enqueue O(1)
/// - dequeue O(1)
/// - search O(n)
///
/// The Container must implements front, back, push_back, pop_front, size, empty with standard semantics.
///
template <typename T, typename Container = std::deque<T>>
class queue {
 public:
  using value_type = typename Container::value_type;
  using reference = typename Container::reference;
  using const_reference = typename Container::const_reference;
  using size_type = typename Container::size_type;
  using container_type = Container;

 private:
  Container c;

 public:
  // ctors
  queue() noexcept(std::is_nothrow_default_constructible_v<Container>) = default;

  explicit queue(const Container& other) noexcept(std::is_nothrow_copy_constructible_v<Container>) : c(other) {}

  explicit queue(Container&& other) noexcept(std::is_nothrow_move_constructible_v<Container>) : c(std::move(other)) {}

  // capacity

  bool empty() const noexcept { return c.empty(); }

  size_type size() const noexcept { return c.size(); }

  // observers

  [[nodiscard]] reference front() {
    assert(!c.empty() && "queue::front called on empty queue");
    return c.front();
  }

  [[nodiscard]] const_reference front() const {
    assert(!c.empty() && "queue::front const called on empty queue");
    return c.front();
  }

  [[nodiscard]] reference back() {
    assert(!c.empty() && "queue::back called on empty queue");
    return c.back();
  }

  [[nodiscard]] const_reference top() const {
    assert(!c.empty() && "queue::back const called on empty queue");
    return c.back();
  }

  // modifiers

  void enqueue(const_reference value) { c.push_back(value); }

  void dequeue() { c.pop_front(); }

  void swap(queue& q) noexcept { std::swap(c, q.c); }
};

}  // namespace my
