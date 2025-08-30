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

 public:
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

  Node* sibling(Node* n) {
    if (n == n->parent->left) {
      return n->parent->right;
    } else {
      return n->parent->left;
    }
  }

  void transplant(Node* a, Node* b) {
    if (a->parent == nullptr) {
      root = b;
    } else if (a == a->parent->left) {
      a->parent->left = b;
    } else {
      a->parent->right = b;
    }
    if (b != nullptr) {
      b->parent = a->parent;
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

  /// @brief Defenetly it is same as in libstdc++
  /// but without header node (to simplify logic)
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
        break;
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
    root->color = node_colors::BLACK;
  }

  void clear(Node* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
  }

  static Node* search_min(Node* x) {
    if (!x) {
      return nullptr;
    }

    while (x->left) {
      x = x->left;
    }
    return x;
  }

  static Node* search_max(Node* x) {
    if (!x) {
      return nullptr;
    }

    while (x->right) {
      x = x->right;
    }
    return x;
  }

  static Node* successor(Node* x) {
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

  static Node* predecessor(Node* x) {
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

  /// @brief en wiki aproach
  void fixDelete(Node* x, Node* parent) {
    while (x != root && (x == nullptr || x->color == node_colors::BLACK)) {
      if (x == (parent ? parent->left : nullptr)) {
        Node* w = parent->right;

        // case 3
        if (w->color == node_colors::RED) {
          w->color = node_colors::BLACK;
          parent->color = node_colors::RED;
          parent = rotate_left(parent);
          w = parent->right;
        }

        if ((w->left == nullptr || w->left->color == node_colors::BLACK) &&
            (w->right == nullptr || w->right->color == node_colors::BLACK)) {
          // case 2
          w->color = node_colors::RED;
          x = parent;
          parent = parent->parent;
        } else {
          if (w->right == nullptr || w->right->color == node_colors::BLACK) {
            // case 5
            if (w->left) w->left->color = node_colors::BLACK;
            w->color = node_colors::RED;
            w = rotate_right(w);
            w = parent->right;
          }

          // case 6
          w->color = parent->color;
          parent->color = node_colors::BLACK;
          if (w->right) w->right->color = node_colors::BLACK;
          parent = rotate_left(parent);
          x = root;
        }
      } else {  // symetry cases
        Node* w = parent->left;

        if (w->color == node_colors::RED) {
          // case 3
          w->color = node_colors::BLACK;
          parent->color = node_colors::RED;
          parent = rotate_right(parent);
          w = parent->left;
        }

        if ((w->left == nullptr || w->left->color == node_colors::BLACK) &&
            (w->right == nullptr || w->right->color == node_colors::BLACK)) {
          // case 2
          w->color = node_colors::RED;
          x = parent;
          parent = parent->parent;
        } else {
          if (w->left == nullptr || w->left->color == node_colors::BLACK) {
            // case 5
            if (w->right) w->right->color = node_colors::BLACK;
            w->color = node_colors::RED;
            w = rotate_left(w);
            w = parent->left;
          }

          // case 6
          w->color = parent->color;
          parent->color = node_colors::BLACK;
          if (w->left) w->left->color = node_colors::BLACK;
          parent = rotate_right(parent);
          x = root;
        }
      }
    }
    if (x) x->color = node_colors::BLACK;
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
        current = search_min(current->right);
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

  void erase(const Key& key) {
    Node* node = root;
    Node* parent = nullptr;

    // search node to remove
    while (node && key != node->data.first) {
      parent = node;

      if (cmp(key, node->data.first)) {
        node = node->left;
      } else {
        node = node->right;
      }
    }

    // node is not found
    if (!node) {
      return;
    }

    Node* y = node;
    Node* x = nullptr;
    node_colors y_original_color = y->color;

    // both cildren
    if (node->left != nullptr && node->right != nullptr) {
      y = search_min(node->right);
      y_original_color = y->color;
      x = y->right;

      if (y->parent == node) {
        if (x) x->parent = y;
      } else {
        transplant(y, y->right);
        y->right = node->right;
        y->right->parent = y;
      }

      transplant(node, y);
      y->left = node->left;
      y->left->parent = y;
      y->color = node->color;

    } else {
      // 2. <= 1 child
      x = (node->left != nullptr) ? node->left : node->right;
      transplant(node, x);
    }

    delete node;

    // remove black node -> black height changed, fix it
    if (y_original_color == node_colors::BLACK) {
      fixDelete(x, (x ? x->parent : nullptr));
    }
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
