#pragma once

#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace my {

enum class node_colors { RED, BLACK };

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
  void insert_and_rebalance(bool insert_left, Node* node, Node* parent) {
    node->parent = parent;

    // insert as in BST
    if (insert_left) {
      parent->left = node;
    } else {
      parent->right = node;
    }

    // rebalance
    while (node != root && node->parent->color == node_colors::RED) {
      Node* p = node->parent;
      Node* g = grandparent(node);
      Node* u = uncle(node);

      // uncle is red
      if (u && u->color == node_colors::RED) {
        p->color = node_colors::BLACK;
        u->color = node_colors::BLACK;
        g->color = node_colors::RED;
        node = g;
        continue;
      }

      // uncle is black or nullptr

      // triangle case
      if (node == p->right && p == g->left) {
        g->left = rotate_left(p);
      } else if (node == p->left && p == g->right) {
        g->right = rotate_left(p);
      }

      // line case
      p->color = node_colors::BLACK;
      g->color = node_colors::RED;
      if (node == p->left && p == g->left) {
        if (g->parent) {
          if (g == g->parent->left) {
            g->parent->left = rotate_right(g);
          } else {
            g->parent->right = rotate_right(g);
          }
        } else {
          root = rotate_right(g);
        }
      } else if (node == p->right && p == g->right) {
        if (g->parent) {
          if (g == g->parent->left) {
            g->parent->left = rotate_left(g);
          } else {
            g->parent->right = rotate_left(g);
          }
        } else {
          root = rotate_left(g);
        }
      }

      root->color = node_colors::BLACK;
    }
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

  void insert(const ValueType& kv) {
    Node* new_node = new Node(kv);

    if (root == nullptr) {
      root = new_node;
      root->color = node_colors::BLACK;
      return;
    }

    // find place to insert
    Node* current = root;
    Node* parent = nullptr;
    bool insert_left = true;

    const Key& key = kv.first;

    while (current != nullptr) {
      parent = current;
      if (cmp(key, current->data.first)) {
        current = current->left;
        insert_left = true;
      } else if (cmp(current->data.first, key)) {
        current = current->right;
        insert_left = false;
      }
    }

    // insert and rebalance
    insert_and_rebalance(insert_left, new_node, parent);
  }

  iterator begin() { return iterator(minimum(root)); }

  iterator end() { return iterator(nullptr); }

  void print_tree(Node* node, int indent = 0) const {
    if (!node) return;
    const int SPACES = 2;
    print_tree(node->right, indent + SPACES);
    std::cout << std::setw(indent) << ' ';
    std::cout << node->data.first << (node->color == node_colors::RED ? " (R)" : " (B)") << node
              << " parent=" << (node->parent ? node->parent : 0) << " right=" << (node->right ? node->right : 0)
              << "\n";
    print_tree(node->left, indent + SPACES);
  }

  void print() { print_tree(root, 0); }
};

}  // namespace my
