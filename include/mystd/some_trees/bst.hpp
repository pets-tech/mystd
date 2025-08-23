#pragma once
#include <cassert>
#include <functional>
#include <utility>

namespace my {

template <class Key, class Value>
struct KeyOfValueImpl {
  using ValueType = std::pair<Key, Value>;
  const Key& operator()(const ValueType& v) const noexcept { return v.first; }
};

template <class Key, class Value, class KeyOfValue = KeyOfValueImpl<Key, Value>, class Compare = std::less<Key>>
class binary_search_tree {
  using ValueType = std::pair<Key, Value>;

 private:
  struct Node {
    ValueType data;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    explicit Node(const ValueType& v) : data(v) {}
  };

  Node* root = nullptr;
  KeyOfValue key_of_value{};
  Compare comp{};

  static bool eq_keys(const Compare& c, const Key& a, const Key& b) noexcept { return !c(a, b) && !c(b, a); }

  Node* search_recursive(Node* x, const ValueType& v) const {
    const Key& key = key_of_value(v);
    while (x && !eq_keys(comp, key_of_value(x->data), key)) {
      x = comp(key, key_of_value(x->data)) ? x->left : x->right;
    }
    return x;
  }

  Node* search_iterative(Node* x, const ValueType& v) const {
    const Key& key = key_of_value(v);
    Node* cur = x;
    while (cur && !eq_keys(comp, key_of_value(cur->data), key)) {
      cur = comp(key, key_of_value(cur->data)) ? cur->left : cur->right;
    }
    return cur;
  }

  Node* search_min(Node* x) const noexcept {
    if (!x) return nullptr;
    while (x->left) x = x->left;
    return x;
  }

  Node* search_max(Node* x) const noexcept {
    if (!x) return nullptr;
    while (x->right) x = x->right;
    return x;
  }

  Node* successor(Node* x) const noexcept {
    if (!x) return nullptr;
    if (x->right) return search_min(x->right);
    Node* y = x->parent;
    while (y && x == y->right) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  Node* predecessor(Node* x) const noexcept {
    if (!x) return nullptr;
    if (x->left) return search_max(x->left);
    Node* y = x->parent;
    while (y && x == y->left) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  Node* insert_recursive(Node* x, const ValueType& v, Node* parent = nullptr) {
    if (!x) {
      Node* n = new Node(v);
      n->parent = parent;
      return n;
    }
    const Key& key = key_of_value(v);
    const Key& cur = key_of_value(x->data);

    if (eq_keys(comp, key, cur)) {
      x->data = v;
      return x;
    }

    if (comp(key, cur)) {
      x->left = insert_recursive(x->left, v, x);
    } else {
      x->right = insert_recursive(x->right, v, x);
    }
    return x;
  }

  Node* insert_iterative(Node* x, const ValueType& v) {
    if (!x) {
      root = new Node(v);
      return root;
    }
    const Key& key = key_of_value(v);
    Node* prev = nullptr;
    Node* curr = x;
    while (curr) {
      prev = curr;
      const Key& cur = key_of_value(curr->data);
      if (eq_keys(comp, key, cur)) {
        curr->data = v;
        return curr;
      }
      curr = comp(key, cur) ? curr->left : curr->right;
    }
    Node* node = new Node(v);
    node->parent = prev;
    if (comp(key, key_of_value(prev->data)))
      prev->left = node;
    else
      prev->right = node;
    return node;
  }

  Node* delete_recursive(Node* x, const ValueType& v) {
    if (!x) return nullptr;
    const Key& k = key_of_value(v);
    const Key& cur = key_of_value(x->data);

    if (comp(k, cur)) {
      x->left = delete_recursive(x->left, v);
      if (x->left) x->left->parent = x;
    } else if (comp(cur, k)) {
      x->right = delete_recursive(x->right, v);
      if (x->right) x->right->parent = x;
    } else {
      if (!x->left) {
        Node* r = x->right;
        if (r) r->parent = x->parent;
        delete x;
        return r;
      }
      if (!x->right) {
        Node* l = x->left;
        if (l) l->parent = x->parent;
        delete x;
        return l;
      }

      Node* m = search_min(x->right);
      x->data = m->data;
      x->right = delete_recursive(x->right, m->data);
      if (x->right) x->right->parent = x;
    }
    return x;
  }

  Node* delete_iterative(Node* x, const ValueType& v) {
    assert(false && "[delete_iterative] not implemented.");
    return x;
  }

  static void clear(Node* x) noexcept {
    if (!x) return;
    clear(x->left);
    clear(x->right);
    delete x;
  }

 public:
  binary_search_tree() noexcept = default;
  ~binary_search_tree() { clear(root); }

  // just because i'm lazy now
  binary_search_tree(const binary_search_tree&) = delete;
  binary_search_tree& operator=(const binary_search_tree&) = delete;
  binary_search_tree(binary_search_tree&&) noexcept = default;
  binary_search_tree& operator=(binary_search_tree&&) noexcept = default;

  void clear() noexcept {
    clear(root);
    root = nullptr;
  }

  Node* insert(const ValueType& v) {
    root = insert_recursive(root, v);
    return search_recursive(root, v);
  }
  Node* search(const ValueType& v) const { return search_recursive(root, v); }
  void erase(const ValueType& v) { root = delete_recursive(root, v); }

  Node* inserti(const ValueType& v) { return insert_iterative(root, v); }
  Node* searchi(const ValueType& v) const { return search_iterative(root, v); }
  void erasei(const ValueType& v) { root = delete_iterative(root, v); }
};

}  // namespace my
