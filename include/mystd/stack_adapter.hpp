#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <type_traits>
#include <utility>

namespace my {

/// @brief The stack is an container adapter that provides a Last-In-First-Out (LIFO) interface.
///
/// The stack has limited interface includes:
/// - push O(1),
/// - pop  O(1),
/// - top O(1),
/// - empty O(1),
/// - size O(1).
///
/// The Container must implements method above with standard semantics.
///
template <typename T, typename Container = std::deque<T>>
class stack {
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
  stack() noexcept(std::is_nothrow_default_constructible_v<Container>) = default;

  explicit stack(const Container& other) noexcept(std::is_nothrow_copy_constructible_v<Container>) : c(other) {}

  explicit stack(Container&& other) noexcept(std::is_nothrow_move_constructible_v<Container>) : c(std::move(other)) {}

  // capacity

  bool empty() const noexcept { return c.empty(); }

  size_type size() const noexcept { return c.size(); }

  // observers

  [[nodiscard]] reference top() {
    assert(!c.empty() && "stack::top() called on empty stack");
    return c.back();
  }

  [[nodiscard]] const_reference top() const {
    assert(!c.empty() && "stack::top() const called on empty stack");
    return c.back();
  }

  // modifiers

  void push(const value_type& value) { c.push_back(value); }

  void push(value_type&& value) { c.push_back(std::move(value)); }

  void pop() {
    assert(!c.empty() && "stack::pop() called on empty stack");
    c.pop_back();
  }

  void swap(stack& s) noexcept(noexcept(std::swap(c, s.c))) { std::swap(c, s.c); }
};

}  // namespace my
