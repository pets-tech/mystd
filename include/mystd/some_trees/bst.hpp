#pragma once

#include <functional>

namespace my {

template <class Key, class Value>
struct KeyOfValueImpl {
  using ValueType = std::pair<Key, Value>;
  const Key &operator()(const ValueType &v) const noexcept { return v.first; }
};

template <class Key, class Value, class KeyOfValue = KeyOfValueImpl<Key, Value>, class KeyEqual = std::equal_to<Key>,
          class Compare = std::less<Key> >
class binary_search_tree {
  using ValueType = std::pair<Key, Value>;

 private:
  struct Node {
    ValueType data;
    Node *left;
    Node *right;
    Node *parent;
    Node(ValueType v) : data(v), left(nullptr), right(nullptr), parent(nullptr) {}
  };

  Node *root;
  KeyOfValue key_of_value;
  KeyEqual equal;
  Compare comp;

  Node *search_recursive(Node *x, ValueType v) {
    auto key = key_of_value(v);

    if (x == nullptr || equal(key_of_value(x->data), key)) {
      return x;
    }

    if (comp(key, key_of_equal(x->data))) {
      return search_recursive(x->left, v);
    } else {
      return search_recursive(x->right, v);
    }
  }

  Node *search_iterative(Node *x, ValueType v) {
    auto key = key_of_value(v);

    Node *cur = x;
    while (cur && !equa(key_of_value(cur->data), key)) {
      if (comp(key, key_of_value(cur->data))) {
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }
    return cur;
  }

  Node *serach_max(Node *x) noexcept {
    if (x == nullptr) {
      return nullptr;
    }

    Node *current = x;
    while (current->right != nullptr) {
      current = current->right;
    }
    return current;
  }

  Node *search_min(Node *x) noexcept {
    if (x == nullptr) {
      return nullptr;
    }

    Node *current = x;
    while (current->left != nullptr) {
      current = current->left;
    }
    return current;
  }

  Node *successor(Node *x) {
    if (x == nullptr) {
      return nullptr;
    }

    Node *y = nullptr;
    if (x->right) {
      return search_min(x->right);
    } else {
      y = x->parent;
      while (y && x == y->right) {
        x = y;
        y = y->parent;
      }
    }
    return y;
  }

  Node *predecessor(Node *x) {
    if (x == nullptr) {
      return nullptr;
    }

    Node *y = nullptr;
    if (x->left) {
      return search_max(x->left);
    } else {
      y = x->parent;
      while (y && x == y->right) {
        x = y;
        y = y->parent;
      }
    }
    return y;
  }

  Node *insert_recursive(Node *x, ValueType v) {
    if (x == nullptr) {
      return new Node(v);
    }

    auto key = key_of_value(v);

    Node *current = x;
    if (comp(key, key_of_value(current->data))) {
      current->left = insert_recursive(current->left, v);
    } else {
      current->right = insert_recursive(current->right, v);
    }
    return current;
  }

  Node *insert_iterative(Node *x, ValueType v) {
    auto key = key_of_value(v);

    Node *prev = nullptr;
    Node *curr = x;
    while (curr) {
      prev = curr;
      if (comp(key, key_of_value(curr->data))) {
        curr = curr->left;
      } else {
        curr = curr->right;
      }
    }

    Node *node = new Node(v);
    if (comp(key, key_of_value(prev->data))) {
      prev->left = node;
    } else {
      prev->right = node;
    }
    return node;
  }

  void delete_iterative(Node *x) {}

 public:
  binary_search_tree() = default;

  Node *insert(ValueType v) { return insert_recursive(root, v); }
  Node *search(ValueType v) { return search_recursive(root, v); }

  Node *inserti(ValueType v) { return insert_iterative(root, v); }
  Node *searchi(ValueType v) { return search_iterative(root, v); }

  // erase()

  int test() const { return 42; }
};

}  // namespace my
