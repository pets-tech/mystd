#pragma once

#include <cassert>
#include <functional>
#include <stack>
#include <utility>

namespace my {

/*
- balancing, avl or redblack
- in delete_recursive -- x->data = m->data it is copy!!!
- iterator
- exeption safety in insert_*
- implement delete_iterative
- copy/move ctor and assignments
- do API with just const Key& insteed value_type
- eq_key -> equal and localy may be better
- check public API bacause Node* is private
- insert or assign

*/

template <class Key, class Value>
struct KeyOfValueImlp {
  using value_type = std::pair<Key, Value>;
  const Key& operator()(const value_type& v) const noexcept { return v.first; }
};

template <class Key, class Value, class KeyOfValue = KeyOfValueImlp<Key, Value>, class Compare = std::less<Key>>
class binary_search_tree {
 public:
  using value_type = std::pair<Key, Value>;

 private:
  struct Node {
    value_type data;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    explicit Node(const value_type& v) : data(v) {}
  };

  Node* root = nullptr;
  Compare comp{};
  KeyOfValue key_of_value{};

  static void clear(Node* x) noexcept {
    if (!x) return;
    clear(x->left);
    clear(x->right);
    delete x;
  }

  /// @brief Compare keys.
  bool equal(const Key& a, const Key& b) const noexcept(noexcept(comp(a, b))) { return !comp(a, b) && !comp(b, a); }

  Node* search_recursive(Node* x, const Key& key) const {
    if (!x) {
      return nullptr;
    }

    if (equal(key_of_value(x->data), key)) {
      return x;
    }

    if (comp(key, key_of_value(x->data))) {
      return search_recursive(x->left, key);
    } else {
      return search_recursive(x->right, key);
    }
  }

  Node* search_iterative(Node* x, const Key& key) const {
    Node* cur = x;
    while (cur && !equal(key_of_value(cur->data), key)) {
      if (comp(key, key_of_value(cur->data))) {
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }
    return cur;
  }

  Node* search_min(Node* x) const noexcept {
    if (!x) {
      return nullptr;
    }

    while (x->left) {
      x = x->left;
    }
    return x;
  }

  Node* search_max(Node* x) const noexcept {
    if (!x) {
      return nullptr;
    }

    while (x->right) {
      x = x->right;
    }
    return x;
  }

  Node* successor(Node* x) const noexcept {
    if (!x) {
      return nullptr;
    }

    if (x->right) {
      return search_min(x->right);
    }

    Node* y = x->parent;
    while (y && x == y->right) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  Node* predecessor(Node* x) const noexcept {
    if (!x) {
      return nullptr;
    }

    if (x->left) {
      return search_max(x->left);
    }

    Node* y = x->parent;
    while (y && x == y->left) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  Node* insert_recursive(Node* node, const value_type& value) {
    if (!node) {
      return new Node(value);
    }

    const Key& key = key_of_value(value);
    const Key& node_key = key_of_value(node->data);

    if (equal(key, node_key)) {
      node->data = value;
      return node;
    }

    if (comp(key, node_key)) {
      node->left = insert_recursive(node->left, value);
      if (node->left) {
        node->left->parent = node;
      }
    } else {
      node->right = insert_recursive(node->right, value);
      if (node->right) {
        node->right->parent = node;
      }
    }
    return node;
  }

  Node* insert_iterative(Node* node, const value_type& value) {
    if (!node) {
      return new Node(value);
    }

    const Key& key = key_of_value(value);

    Node* parent = nullptr;
    Node* cur = node;
    while (cur) {
      parent = cur;
      if (equal(key, key_of_value(cur->data))) {
        cur->data = value;
        return node;
      }

      if (comp(key, key_of_value(cur->data))) {
        cur = cur->left;
      } else {
        cur = cur->right;
      }
    }

    Node* new_node = new Node(value);
    if (comp(key, key_of_value(parent->data))) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }
    new_node->parent = parent;
    return node;
  }

  Node* delete_recursive(Node* node, const Key& key) {
    if (!node) {
      return nullptr;
    }

    const Key& node_key = key_of_value(node->data);

    if (comp(key, node_key)) {
      node->left = delete_recursive(node->left, key);
      if (node->left) {
        node->left->parent = node;
      }
    } else if (comp(node_key, key)) {
      node->right = delete_recursive(node->right, key);
      if (node->right) {
        node->right->parent = node;
      }
    } else {
      // case 1 no left child
      if (!node->left) {
        Node* r = node->right;
        if (r) {
          r->parent = node->parent;
        }
        delete node;
        return r;
      }

      // case 2 no right child
      if (!node->right) {
        Node* l = node->left;
        if (l) {
          l->parent = node->parent;
        }
        delete node;
        return l;
      }

      // case 3 both childs

      Node* succ = search_min(node->right);  // smallest node in right subtree

      if (succ->parent != node) {
        succ->parent->left = succ->right;
        if (succ->right) {
          succ->right->parent = succ->parent;
        }
        succ->right = node->right;
        if (succ->right) {
          succ->right->parent = succ;
        }
      }

      succ->left = node->left;

      if (succ->left) {
        succ->left->parent = succ;
      }

      succ->parent = node->parent;
      delete node;

      return succ;
    }
    return node;
  }

  Node* delete_iterative(Node* root, const Key& key) {
    Node* node = root;
    Node* parent = nullptr;

    // search node to remove
    while (node && !equal(key, key_of_value(node->data))) {
      parent = node;

      if (comp(key, key_of_value(node->data))) {
        node = node->left;
      } else {
        node = node->right;
      }
    }

    // node is not found
    if (!node) {
      return root;
    }

    // found node with both childs
    if (node->left && node->right) {
      Node* succ = search_min(node->right);

      if (succ->parent != node) {
        succ->parent->left = succ->right;
        if (succ->right) {
          succ->right->parent = succ->parent;
        }
        succ->right = node->right;
        if (succ->right) {
          succ->right->parent = succ;
        }
      }

      succ->left = node->left;
      if (succ->left) {
        succ->left->parent = succ;
      }

      if (!parent) {
        root = succ;
      } else if (parent->left == node) {
        parent->left = succ;
      } else {
        parent->right = succ;
      }

      succ->parent = parent;

      delete node;
      return root;
    }

    // node has 0 or 1 child
    Node* child = node->left ? node->left : node->right;
    if (child) {
      child->parent = parent;
    }

    if (!parent) {
      root = child;
    } else if (parent->left == node) {
      parent->left = child;
    } else {
      parent->right = child;
    }

    delete node;
    return root;
  }

  Node* safe_copy(Node* root) {
    if (!root) {
      return nullptr;
    }

    Node* new_root = nullptr;
    try {
      new_root = new Node(root->data);
      std::stack<std::pair<Node*, Node*>> stack;
      stack.push({root, new_root});

      while (!stack.empty()) {
        auto [old_node, new_node] = stack.top();
        stack.pop();

        if (old_node->right) {
          new_node->right = new Node(old_node->right->data);
          new_node->right->parent = new_node;
          stack.push({old_node->right, new_node->right});
        }

        if (old_node->left) {
          new_node->left = new Node(old_node->left->data);
          new_node->left->parent = new_node;
          stack.push({old_node->left, new_node->left});
        }
      }
    } catch (...) {
      clear(new_root);
      throw;
    }
    return new_root;
  }

 public:
  // inorder forward iterator (left -> vert -> right)
  template <bool IsConst>
  class iterator_basic {
   private:
    Node* current;

   public:
    iterator_basic(Node* n = nullptr) : current(n) {}

    value_type& operator*() { return current->data; }

    value_type* operator->() { return &(current->data); }

    iterator_basic& operator++() {
      if (!current) {
        return *this;
      }
      if (current->right) {
        current = current->right;
        while (current->left) {
          current = current->left;
        }
      } else {
        Node* p = current->parent;
        while (p && current == p->right) {
          current = p;
          p = p->parent;
        }
        current = p;
      }
      return *this;
    }

    bool operator==(const iterator_basic& other) const { return current == other.current; }

    bool operator!=(const iterator_basic& other) const { return current != other.current; }
  };

  using iterator = iterator_basic<false>;
  using const_iterator = iterator_basic<true>;

 public:
  binary_search_tree() noexcept = default;

  binary_search_tree(const binary_search_tree& other) { root = safe_copy(other.root); };

  binary_search_tree(binary_search_tree&& other) : root(std::exchange(other.root, nullptr)) {};

  binary_search_tree& operator=(const binary_search_tree& other) {
    if (this != &other) {
      binary_search_tree tmp(other);
      swap(tmp);
    }
    return *this;
  }

  binary_search_tree& operator=(binary_search_tree&& other) {
    if (this != &other) {
      clear(root);
      root = std::exchange(other.root, nullptr);
    }
    return *this;
  }

  ~binary_search_tree() { clear(root); }

  Value at(const Key& key) const {
    Node* node = search_recursive(root, key);
    if (!node) {
      throw std::out_of_range("");
    }
    return node->data.second;
  }

  // recursive API

  void insert(const value_type& value) { root = insert_recursive(root, value); }

  bool contains(const Key& key) const { return search_recursive(root, key) != nullptr; }

  void erase(const Key& key) { root = delete_recursive(root, key); }

  // iterative API

  void inserti(const value_type& value) { root = insert_iterative(root, value); }

  bool containsi(const Key& key) const { return search_iterative(root, key) != nullptr; }

  void erasei(const Key& key) { root = delete_iterative(root, key); }

  void swap(binary_search_tree& other) noexcept { std::swap(root, other.root); }

  // iterators

  iterator begin() {
    Node* n = root;
    if (!n) {
      return iterator(nullptr);
    }
    while (n->left) {
      n = n->left;
    }
    return iterator(n);
  }

  iterator end() { return iterator(nullptr); }
};

}  // namespace my
