#pragma once

#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace my {

enum node_colors { RED, BLACK };

/// @brief Red black tree has invariants:
/// 1. node -- red || black
/// 2. root -- black
/// 3. leafs -- black
/// 4. can not -- red node and red node->parent
/// 5. black-height eq to any leafs
///
/// from wiki notation: n - node, p - parent, g - grandparent, u - uncle
///       g
///      / \
///     p   u
///      \
///       n
///
template <class Key, class Value, class Compare = std::less<Key>>
class redblack_tree {
  using ValueType = std::pair<Key, Value>;

 private:
  struct Node {
    ValueType data;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    node_colors color = node_colors::RED;
    Node(const ValueType& v) : data(v) {}
  };

  Node* root = nullptr;
  Compare cmp{};

  Node* grandparent(Node* n) { return (n && n->parent) ? n->parent->parent : nullptr; }

  Node* uncle(Node* n) {
    Node* g = grandparent(n);

    if (!n || !g) return nullptr;

    if (n->parent == g->left) {
      return g->right;
    } else {
      return g->left;
    }
  }

  /// @brief
  ///   g             g
  ///   |child        |child
  ///   n           pivot
  ///  / \          /   \
    /// a  pivot =>  n     c
  ///    /  \     / \
    ///   b    c   a   b
  Node* rotate_left(Node* n) {
    if (!n || !n->right) return n;

    Node* pivot = n->right;
    Node* b = pivot->left;

    // pivot parent
    pivot->parent = n->parent;
    if (n->parent) {
      if (n->parent->left == n) {
        n->parent->left = pivot;
      } else {
        n->parent->right = pivot;
      }
    }

    // b
    n->right = b;
    if (b) b->parent = n;

    // rotate
    pivot->left = n;
    n->parent = pivot;

    return pivot;
  }

  /// @brief
  ///       g           g
  ///       |child      |child
  ///       n         pivot
  ///     /  \        /  \
    ///  pivot  c  =>  a    n
  ///  /  \              / \
    /// a    b            b   c
  Node* rotate_right(Node* n) {
    if (!n || !n->left) return n;

    Node* pivot = n->left;
    Node* b = pivot->right;

    pivot->parent = n->parent;
    if (n->parent) {
      if (n->parent->left == n) {
        n->parent->left = pivot;
      } else {
        n->parent->right = pivot;
      }
    }

    n->left = b;
    if (b) b->parent = n;

    pivot->right = n;
    n->parent = pivot;

    return pivot;
  }

  /// @brief
  /// left triangle to line
  ///     g   ->    g
  ///    / \  ->   / \
    ///   p   u ->  p   u
  ///    \    -> /
  ///     n   -> n
  ///
  /// right triangle to line
  ///     g   ->    g
  ///    / \  ->   / \
    ///   u   p ->  u   p
  ///      /  ->       \
    ///     n   ->        n
  ///
  ///
  ///
  Node* insert_rebalance(Node* n) {
    std::cout << n << "\n";

    // case 1 root -- recolor to black
    if (n->parent == nullptr) {
      n->color = node_colors::BLACK;
      return n;
    }

    // case2 parent is black it is ok
    if (n->parent->color == node_colors::BLACK) {
      return n;
    }

    // case3 and nested --  parent is red
    Node* u = uncle(n);
    Node* g = grandparent(n);

    if (u != nullptr && u->color == node_colors::RED) {
      // case -- uncle is red

      // recolor recursive
      n->parent->color = node_colors::BLACK;
      u->color = node_colors::BLACK;
      g->color = node_colors::RED;
      insert_rebalance(g);
      return

    } else {
      // case -- uncle is black

      // triangle case
      if (n == n->parent->right && n->parent == g->left) {
        n = rotate_left(n->parent);
      } else if (n == n->parent->left && n->parent == g->right) {
        n = rotate_right(n->parent);
      }

      // line case
      n->parent->color = node_colors::BLACK;
      g->color = node_colors::RED;
      if (n == n->parent->left && n->parent == g->left) {
        return rotate_right(g);
      } else {
        return rotate_left(g);
      }
    }

    return n;  // tree was ok
  }

  void pr(Node* n) {
    while (n) {
      std::cout << n << " right: " << n->parent << "\n";
      n = n->right;
    }
  }

  Node* insert(Node* node, Node* parent, const ValueType& v) {
    if (!node) {
      Node* new_node = new Node(v);
      // Node* res = insert_rebalance(new_node);
      // while (res && res->parent) res = res->parent;
      return new_node;
    }

    const Key& key = v.first;

    if (cmp(key, node->data.first)) {
      node->left = insert(node->left, node, v);
      if (node->left) {
        node->left->parent = node;
      }
    } else if (cmp(node->data.first, key)) {
      node->right = insert(node->right, node, v);
      if (node->right) {
        node->right->parent = node;
      }
    } else {
      return node;
    }

    return node;
  }

  void clear(Node* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
  }

  static Node* minimum(Node* n) {
    while (n && n->left) n = n->left;
    return n;
  }

 public:
  class iterator_basic {
   private:
    Node* current;

   public:
    iterator_basic(Node* n = nullptr) : current(n) {}

    ValueType& operator*() { return current->data; }
    ValueType* operator->() { return &(current->data); }

    iterator_basic& operator++() {
      if (!current) return *this;
      if (current->right) {
        current = minimum(current->right);
      } else {
        Node* p = current->parent;
        while (p && current == p->left) {
          current = p;
          p = p->parent;
        }
        current = p;
      }
      return *this;
    }

    bool operator==(const iterator_basic& other) { return current == other.current; }
    bool operator!=(const iterator_basic& other) { return current != other.current; }
  };

  using iterator = iterator_basic;

 public:
  redblack_tree() {}
  redblack_tree(const redblack_tree& other) {}
  redblack_tree(redblack_tree&& other) {}
  ~redblack_tree() { clear(); }

  redblack_tree& operator=(const redblack_tree& other) {}
  redblack_tree& operator=(redblack_tree&& other) {}

  void clear() { clear(root); }

  void insert(const ValueType& kv) { root = insert(root, nullptr, kv); }

  iterator begin() { return iterator(minimum(root)); }

  iterator end() { return iterator(nullptr); }

  void print_tree(Node* node, int indent = 0) const {
    if (!node) return;
    const int SPACES = 2;
    if (node->right) print_tree(node->right, indent + SPACES);
    if (indent) std::cout << std::setw(indent) << ' ';
    std::cout << node->data.first << "\n";
    if (node->left) print_tree(node->left, indent + SPACES);
  }

  void print() { print_tree(root, 1); }
};

}  // namespace my
