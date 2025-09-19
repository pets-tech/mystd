#pragma once

#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

namespace my {

enum class node_colors { RED, BLACK };

/// @brief Red black tree has invariants:
/// 1. node -- red || black
/// 2. root -- black
/// 3. leafs -- black
/// 4. double red
/// 5. black-height eq to any leafs
///
/// from wiki notation: n - node, p - parent, g - grandparent, u - uncle
///   s - sibling, d - distanct nephew, c - close nephew
///       g
///      / \
///     p   u
///    / \
///   s  n
///  / \
/// d   c
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
  size_t size_ = 0;

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
    if (!n || !n->parent) {
      return nullptr;
    }
    return (n == n->parent->left) ? n->parent->right : n->parent->left;
  }

  /// @brief sets v subtree instead u
  void transplant(Node* u, Node* v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    if (v) v->parent = u->parent;
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
    Node* g = n->parent;

    pivot->parent = g;
    if (g) {
      if (g->left == n) {
        g->left = pivot;
      } else {
        g->right = pivot;
      }
    } else {
      root = pivot;
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
    Node* g = n->parent;

    pivot->parent = g;
    if (g) {
      if (g->left == n) {
        g->left = pivot;
      } else {
        g->right = pivot;
      }
    } else {
      root = pivot;
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
        p = g->left;
        node = p->left;
      } else if (node == p->left && p == g->right) {
        g->right = rotate_right(p);
        p = g->right;
        node = p->right;
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

  Node* find(Node* node, const Key& key) {
    if (node == nullptr) {
      return node;
    }

    if (cmp(key, node->data.first)) {
      return find(node->left, key);
    } else if (cmp(node->data.first, key)) {
      return find(node->right, key);
    } else {
      return node;
    }
  }

  // ! dublicates in right subtree
  size_t count(Node* node, const Key& key) {
    if (node == nullptr) {
      return 0;
    }

    if (cmp(key, node->data.first)) {
      return count(node->left, key);
    } else if (cmp(node->data.first, key)) {
      return count(node->right, key);
    } else {
      return 1 + count(node->right, key);
    }
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

  /// @brief wiki aproach
  void erase_rebalance(Node* x, Node* parent) {
    while ((x != root) && (x == nullptr || x->color == node_colors::BLACK)) {
      if (parent->left == x) {
        Node* s = parent->right;

        // case 1 - sibling is red
        if (s && s->color == node_colors::RED) {
          s->color = node_colors::BLACK;
          parent->color = node_colors::RED;
          // parent = rotate_left(parent);
          rotate_left(parent);
          s = parent->right;
        }

        // sibling is black

        if (s != nullptr && (!s->left || s->left->color == node_colors::BLACK) &&
            (!s->right || s->right->color == node_colors::BLACK)) {
          // case 2
          s->color = node_colors::RED;
          x = parent;
          if (x != root) parent = x->parent;
        } else {
          // case 3
          if (!s->right || s->right->color == node_colors::BLACK) {
            if (s->left) s->left->color = node_colors::BLACK;
            s->color = node_colors::RED;
            rotate_right(s);
            s = parent->right;
          }
          // case 4
          s->color = parent->color;
          s->parent->color = node_colors::BLACK;
          if (s->right) s->right->color = node_colors::BLACK;
          rotate_left(parent);
          x = root;
        }
      } else {
        Node* s = parent->left;

        // case 1 - sibling is red
        if (s && s->color == node_colors::RED) {
          s->color = node_colors::BLACK;
          parent->color = node_colors::RED;
          // parent = rotate_right(parent);
          rotate_right(parent);
          s = parent->left;
        }

        // sibling is black

        if (s != nullptr && (!s->left || s->left->color == node_colors::BLACK) &&
            (!s->right || s->right->color == node_colors::BLACK)) {
          // case 2
          s->color = node_colors::RED;
          x = parent;
          if (x != root) parent = x->parent;
        } else {
          // case 3
          if (!s->left || s->left->color == node_colors::BLACK) {
            s->right->color = node_colors::BLACK;
            s->color = node_colors::RED;
            rotate_left(s);
            s = parent->left;
          }
          // case 4
          s->color = parent->color;
          parent->color = node_colors::BLACK;
          if (s->left) s->left->color = node_colors::BLACK;
          rotate_right(parent);
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
  using const_iterator = iterator_basic;

 public:
  redblack_tree() {}
  redblack_tree(const redblack_tree& other) {}
  redblack_tree(redblack_tree&& other) {}
  ~redblack_tree() { clear(); }

  redblack_tree& operator=(const redblack_tree& other) {}
  redblack_tree& operator=(redblack_tree&& other) {}

  void clear() {
    clear(root);
    root = nullptr;
    size_ = 0;
  }

  void insert(const ValueType& kv) {
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
      } else {
        // keys equals then insert on the right
        current = current->right;
        insert_left = false;
      }
    }

    Node* new_node = new Node(kv);
    ++size_;

    if (root == nullptr) {
      root = new_node;
      root->color = node_colors::BLACK;
      return;
    }

    // insert and rebalance
    insert_and_rebalance(insert_left, new_node, parent);
  }

  void erase(const Key& key) {
    Node* z = root;
    while (z && key != z->data.first) {
      if (cmp(key, z->data.first))
        z = z->left;
      else
        z = z->right;
    }

    if (!z) return;

    Node* y = z;
    node_colors y_original_color = y->color;
    Node* x = nullptr;
    Node* x_parent = nullptr;

    if (!z->left) {
      x = z->right;
      x_parent = z->parent;
      transplant(z, z->right);
    } else if (!z->right) {
      x = z->left;
      x_parent = z->parent;
      transplant(z, z->left);
    } else {
      y = search_min(z->right);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == z) {
        x_parent = y;
      } else {
        transplant(y, y->right);
        y->right = z->right;
        if (y->right) y->right->parent = y;
        x_parent = y->parent;
      }
      transplant(z, y);
      y->left = z->left;
      if (y->left) y->left->parent = y;
      y->color = z->color;
    }

    if (y_original_color == node_colors::BLACK) {
      erase_rebalance(x, x_parent);
    }

    delete z;
    --size_;
  }

  Value find(const Key& key) {
    Node* node = root;
    auto result = find(node, key);

    if (result == nullptr) {
      return std::numeric_limits<Value>::max();
    }
    return result->data.second;
  }

  Value& operator[](const Key& key) {
    Node* node = root;
    auto val = find(node, key);
    return val->data.second;
  }

  bool contains(const Key& key) {
    auto result = find(key);

    Value max_val = std::numeric_limits<Value>::max();
    Value relative_diff = std::fabs(1.0 - result / max_val);

    if (relative_diff < 10e-3) {
      return false;
    }
    return true;
  }

  size_t count(const Key& key) {
    Node* node = root;
    return count(node, key);
  }

  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

  iterator begin() { return iterator(minimum(root)); }

  iterator end() { return iterator(nullptr); }

  void print_tree(Node* node, int indent = 0, bool debug = false) const {
    if (!node) return;
    const int SPACES = 2;
    print_tree(node->right, indent + SPACES);
    std::cout << std::setw(indent) << ' ';

    if (debug) {
      std::cout << node->data.first << (node->color == node_colors::RED ? " (R)" : " (B)") << node
                << " parent=" << (node->parent ? node->parent : 0) << " left=" << (node->left ? node->left : 0)
                << " right=" << (node->right ? node->right : 0) << "\n";
    } else {
      std::cout << node->data.first << (node->color == node_colors::RED ? "R" : "B") << "\n";
    }
    print_tree(node->left, indent + SPACES);
  }

  void print() { print_tree(root, 0); }

  /// validation rb tree
 public:
  bool verify_binary_tree(Node* x) {
    if (x == nullptr) return true;

    if (x->left && x->left->parent != x) return false;
    if (x->right && x->right->parent != x) return false;

    if (x->parent && x->parent->left != x && x->parent->right != x) {
      return false;
    }

    return verify_binary_tree(x->left) && verify_binary_tree(x->right);
  }

  int rb_and_black_height(Node* x) {
    if (x == nullptr) return 1;

    int left_black_height = rb_and_black_height(x->left);
    int right_black_height = rb_and_black_height(x->right);

    if (left_black_height == -1 || right_black_height == -1 || left_black_height != right_black_height) {
      return -1;
    }

    // red - red nodes check
    if (x->color == node_colors::RED) {
      if (x->left && x->left->color == node_colors::RED) {
        return -1;
      }
      if (x->right && x->right->color == node_colors::RED) {
        return -1;
      }
    }

    return left_black_height + (x->color == node_colors::BLACK ? 1 : 0);
  }

  bool is_rb_tree() {
    if (root == nullptr) return true;

    if (root->color == node_colors::RED) return false;

    return rb_and_black_height(root) != -1;
  }

  bool is_binary_tree() { return verify_binary_tree(root); }

  void inorder(Node* current, std::vector<int>& result) {
    if (current) {
      inorder(current->left, result);
      result.push_back(current->data.first);
      inorder(current->right, result);
    }
  }
};

}  // namespace my
