#pragma once

#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <sstream>

#include "mystd/iterator/iterator.hpp"

namespace my {

enum class NodeColors { RED, BLACK };

/// @brief Red black tree has invariants:
/// 1. node -- red || black
/// 2. root -- black
/// 3. leafs -- black
/// 4. double red
/// 5. black-height eq to any leafs
///
/// from wiki notation: n - node, p - parent, g - grandparent,
/// u - uncle, s - sibling, d - distanct nephew, c - close nephew
///        g
///       / \
///      p   u
///     / \
///    s  n
///   / \
///  d   c
///
/// Notes:
///   - dublicates save in right subtree
template <typename ValueType, typename KeyOfValue,
          typename Compare = std::less<std::invoke_result_t<KeyOfValue, ValueType>>>
class rb_tree {
 public:
  using Key = std::invoke_result_t<KeyOfValue, ValueType>;

 private:
  struct Node {
    ValueType value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    NodeColors color = NodeColors::RED;
    explicit Node(const ValueType& v) : value(v) {}
  };

  Node* root = nullptr;
  size_t size_ = 0;
  KeyOfValue key_of_value{};
  Compare comp{};

 public:
  // iterators
  using iterator = my::bidirectional_iterator<ValueType, Node*, false, std::bidirectional_iterator_tag,
                                              TreeTraversalPolicy<Node*>, DefaultValueExtractor<ValueType>>;
  using const_iterator = my::bidirectional_iterator<ValueType, const Node*, true, std::bidirectional_iterator_tag,
                                                    TreeTraversalPolicy<const Node*>, DefaultValueExtractor<ValueType>>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 private:
  Node* find_node(Node* node, const Key& key) const {
    Node* current = node;

    while (current != nullptr) {
      if (comp(key, key_of_value(current->value))) {
        current = current->left;
      } else if (comp(key_of_value(current->value), key)) {
        current = current->right;
      } else {
        return current;
      }
    }

    return nullptr;
  }

  size_t count_node(Node* node, const Key& key) const {
    if (node == nullptr) return 0;

    if (comp(key, key_of_value(node->value))) {
      return count(node->left, key);
    } else if (comp(key_of_value(node->value), key)) {
      return count(node->right, key);
    } else {
      // dublicates lives in right subtree
      return 1 + count(node->right, key);
    }
  }

  static Node* search_min(Node* node) {
    if (node == nullptr) return nullptr;
    while (node->left) node = node->left;
    return node;
  }

  static Node* search_max(Node* node) {
    if (node == nullptr) return nullptr;
    while (node->right) node = node->right;
    return node;
  }

  Node* grandparent(Node* n) const { return (n && n->parent) ? n->parent->parent : nullptr; }

  Node* uncle(Node* n) {
    Node* g = grandparent(n);
    if (!n || !g) return nullptr;
    if (n->parent == g->left) return g->right;
    return g->left;
  }

  Node* sibling(Node* n) {
    if (!n || !n->parent) return nullptr;
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

  /// @brief this is need to simplify code (like in STL)
  bool is_effectively_black(Node* node) const { return node == nullptr || node->color == NodeColors::BLACK; }

  /// @brief Defenetly it is same as in libstdc++
  /// but without header node
  ///
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
  /// node:  is the newly inserted node
  void insert_rebalance(Node* node) {
    // TODO check and remove assign after rotation
    while (node != root && node->parent && node->parent->color == NodeColors::RED) {
      Node* p = node->parent;
      Node* g = grandparent(node);
      if (!g) break;
      Node* u = uncle(node);

      // uncle is red -> recolor and continue upward
      if (u && u->color == NodeColors::RED) {
        p->color = NodeColors::BLACK;
        u->color = NodeColors::BLACK;
        g->color = NodeColors::RED;
        node = g;
        continue;
      }

      // uncle is black or nullptr
      // triangle cases -> rotate to make it line
      if (node == p->right && p == g->left) {
        g->left = rotate_left(p);
        p = g->left;
        node = p->left;
      } else if (node == p->left && p == g->right) {
        g->right = rotate_right(p);
        p = g->right;
        node = p->right;
      }

      // line cases -> recolor and rotate g
      p->color = NodeColors::BLACK;
      g->color = NodeColors::RED;
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
      root->color = NodeColors::BLACK;
    }
    if (root) root->color = NodeColors::BLACK;
  }

  /// @brief wiki aproach
  /// x is is a place on tree where was deleted node
  ///   (might be nullptr)
  void erase_rebalance(Node* x, Node* parent) {
    while ((x != root) && is_effectively_black(x)) {
      if (parent->left == x) {
        Node* s = parent->right;

        // case 1 - sibling is red
        if (s && s->color == NodeColors::RED) {
          s->color = NodeColors::BLACK;
          parent->color = NodeColors::RED;
          rotate_left(parent);
          s = parent->right;
        }

        // sibling is black

        if (s && is_effectively_black(s->left) && is_effectively_black(s->right)) {
          // case 2
          s->color = NodeColors::RED;
          x = parent;
          if (x != root) parent = x->parent;
        } else {
          // case 3
          if (s && is_effectively_black(s->right)) {
            if (s->left) s->left->color = NodeColors::BLACK;
            s->color = NodeColors::RED;
            rotate_right(s);
            s = parent->right;
          }
          // case 4
          if (s) {
            s->color = parent->color;
            s->parent->color = NodeColors::BLACK;
            if (s->right) s->right->color = NodeColors::BLACK;
          }
          rotate_left(parent);
          break;
        }
      } else {
        // symmetry
        Node* s = parent->left;

        // case 1 - sibling is red
        if (s && s->color == NodeColors::RED) {
          s->color = NodeColors::BLACK;
          parent->color = NodeColors::RED;
          rotate_right(parent);
          s = parent->left;
        }

        // sibling is black

        if (s && is_effectively_black(s->left) && is_effectively_black(s->right)) {
          // case 2
          s->color = NodeColors::RED;
          x = parent;
          if (x != root) parent = x->parent;
        } else {
          // case 3
          if (s && is_effectively_black(s->left)) {
            s->right->color = NodeColors::BLACK;
            s->color = NodeColors::RED;
            rotate_left(s);
            s = parent->left;
          }
          // case 4
          if (s) {
            s->color = parent->color;
            parent->color = NodeColors::BLACK;
            if (s->left) s->left->color = NodeColors::BLACK;
          }
          rotate_right(parent);
          break;
        }
      }
    }
    if (x) x->color = NodeColors::BLACK;
  }

  void clear_nodes(Node* node) {
    if (node == nullptr) return;
    clear_nodes(node->left);
    clear_nodes(node->right);
    delete node;
  }

 public:
  rb_tree() {}
  rb_tree(const rb_tree& other) {}
  rb_tree(rb_tree&& other) {}
  ~rb_tree() { clear(); }

  rb_tree& operator=(const rb_tree& other) {}
  rb_tree& operator=(rb_tree&& other) {}

  void clear() {
    clear_nodes(root);
    root = nullptr;
    size_ = 0;
  }

  void insert(const ValueType& value) {
    const Key key = key_of_value(value);

    // find place to insert
    Node* parent = nullptr;
    Node* current = root;
    bool insert_left = true;

    while (current != nullptr) {
      parent = current;
      if (comp(key, key_of_value(current->value))) {
        current = current->left;
        insert_left = true;
      } else if (comp(key_of_value(current->value), key)) {
        current = current->right;
        insert_left = false;
      } else {
        // keys equals then insert in the right subtree
        current = current->right;
        insert_left = false;
      }
    }

    Node* new_node = new Node(value);
    ++size_;

    if (parent == nullptr) {
      root = new_node;
      root->color = NodeColors::BLACK;
      return;
    }

    new_node->parent = parent;

    // insert as in BST
    if (insert_left) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }

    // rebalance
    insert_rebalance(new_node);
  }

  void erase(const Key& key) {
    Node* z = find_node(root, key);
    if (!z) return;

    Node* y = z;
    NodeColors y_original_color = y->color;
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

    if (y_original_color == NodeColors::BLACK) {
      // if x nullptr, use x_parent to got where z was lives
      erase_rebalance(x, x_parent);
    }

    delete z;
    --size_;
  }

  ValueType* find(const Key& key) {
    Node* n = find_node(root, key);
    return n ? &n->value : nullptr;
  }

  const ValueType* find(const Key& key) const {
    Node* n = find_node(root, key);
    return n ? &n->value : nullptr;
  }

  bool contains(const Key& key) const { return find_node(root, key) != nullptr; }

  size_t count(const Key& key) const { return count_node(root, key); }

  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

  // iterators

  iterator begin() { return iterator(search_min(root)); }
  iterator end() { return iterator(nullptr); }
  const_iterator begin() const { return const_iterator(search_min(root)); }
  const_iterator end() const { return const_iterator(nullptr); }
  const_iterator cbegin() const { return const_iterator(search_min(root)); }
  const_iterator cend() const { return const_iterator(nullptr); }

  // TODO implement true bechevier with end() == rbegin()
  reverse_iterator rbegin() { return root ? reverse_iterator(iterator(search_max(root))) : reverse_iterator(end()); }
  reverse_iterator rend() { return root ? reverse_iterator(iterator(search_min(root))) : reverse_iterator(end()); }
  const_reverse_iterator rbegin() const {
    return root ? const_reverse_iterator(const_iterator(search_max(root))) : const_reverse_iterator(end());
  }
  const_reverse_iterator rend() const {
    return root ? const_reverse_iterator(const_iterator(search_min(root))) : const_reverse_iterator(end());
  }
  const_reverse_iterator crbegin() const {
    return root ? const_reverse_iterator(const_iterator(search_max(root))) : const_reverse_iterator(end());
  }
  const_reverse_iterator crend() const {
    return root ? const_reverse_iterator(const_iterator(search_min(root))) : const_reverse_iterator(end());
  }

 public:
  // verify tree helpers
  bool verify_binary_tree(Node* x) {
    if (x == nullptr) return true;
    if (x->left && x->left->parent != x) return false;
    if (x->right && x->right->parent != x) return false;
    if (x->parent && x->parent->left != x && x->parent->right != x) return false;
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
    if (x->color == NodeColors::RED) {
      if (x->left && x->left->color == NodeColors::RED) {
        return -1;
      }
      if (x->right && x->right->color == NodeColors::RED) {
        return -1;
      }
    }

    return left_black_height + (x->color == NodeColors::BLACK ? 1 : 0);
  }

  bool is_rb_tree() {
    if (root == nullptr) return true;
    if (root->color == NodeColors::RED) return false;
    return rb_and_black_height(root) != -1;
  }

  bool is_binary_tree() { return verify_binary_tree(root); }

  void inorder(Node* current, std::vector<int>& result) {
    if (current) {
      inorder(current->left, result);
      result.push_back(current->value.first);
      inorder(current->right, result);
    }
  }

  // helpers

  void print_tree(Node* node, int indent = 0, bool debug = false) const {
    if (!node) return;
    const int SPACES = 2;
    print_tree(node->right, indent + SPACES);
    std::cout << std::setw(indent) << ' ';

    if (debug) {
      std::cout << node->value.first << (node->color == NodeColors::RED ? " (R)" : " (B)") << node
                << " parent=" << (node->parent ? node->parent : 0) << " left=" << (node->left ? node->left : 0)
                << " right=" << (node->right ? node->right : 0) << "\n";
    } else {
      std::cout << node->value.first << (node->color == NodeColors::RED ? "R" : "B") << "\n";
    }
    print_tree(node->left, indent + SPACES);
  }

  void print() { print_tree(root, 0); }
};

}  // namespace my
