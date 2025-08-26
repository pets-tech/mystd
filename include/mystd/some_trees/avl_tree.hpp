#pragma once

#include <iomanip>
#include <sstream>

namespace my {

template <class Key, class Value, class Compare = std::less<Key>>
class avl_tree {
 public:
  using ValueType = std::pair<Key, Value>;
  using size_type = std::size_t;

 private:
  struct Node {
    ValueType data;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    int height = 1;
    Node(const ValueType& d) : data(d) {}
  };

  Node* root = nullptr;
  Compare cmp{};

 private:
  int height(Node* node) const {
    if (node == nullptr) {
      return 0;
    }
    return node->height;
  }

  int balance_factor(Node* node) const { return height(node->right) - height(node->left); }

  void update_height(Node* node) const { node->height = std::max(height(node->left), height(node->right)) + 1; }

  // left inbalance
  //        y      ->        x
  //       / \     ->       /  \
  //      x   d    ->     z     y
  //     / \       ->    / \   / \
  //    z   c      ->   a  b  c   d
  //   / \         ->
  //  a   b        ->
  Node* rotate_right(Node* y) {
    Node* x = y->left;
    Node* c = x->right;

    if (c) c->parent = y;

    x->right = y;
    y->left = c;

    x->parent = y->parent;
    y->parent = x;

    update_height(y);
    update_height(x);

    return x;
  }

  // right inbalance
  //        y         ->        x
  //       / \        ->      /   \
  //      a   x       ->     y     z
  //         / \      ->    / \   / \
  //        b   z     ->   a   b c   d
  //           / \    ->
  //          c   d   ->
  Node* rotate_left(Node* y) {
    Node* x = y->right;
    Node* b = x->left;

    if (b) b->parent = y;

    x->left = y;
    y->right = b;

    x->parent = y->parent;
    y->parent = x;

    update_height(y);
    update_height(x);

    return x;
  }

  // right-left
  //   y     ->   y      ->      x
  //  / \    ->  / \     ->     /  \
  // a   z   -> a   x    ->    y    z
  //    / \  ->    / \   ->   / \  / \
  //   x   d ->   b   z  ->  a  b  c  d
  //  / \    ->      / \ ->
  // b   c   ->     c  d ->
  Node* rotate_right_left(Node* y) {
    y->right = rotate_right(y->right);
    return rotate_left(y);
  }

  Node* rotate_left_right(Node* y) {
    y->left = rotate_left(y->left);
    return rotate_right(y);
  }

  // // right inbalance -> rotate left
  // h = 2   y      bf = 2 - 0 = 2
  // h = 1     x    bf = 1 - 0 = 1
  // h = 0       z  bf = 0 - 0 = 0

  // // left inbalance -> rotate right
  // h = 2       y  bf = 0 - 2 = -2
  // h = 1     x    bf = 0 - 1 = -1
  // h = 0   z      bf = 0 - 0 = 0

  // // right-left
  // h = 2   y      bf = 2 - 0 = 2
  // h = 1     x    bf = 0 - 1 = -1
  // h = 0   z      bf = 0 - 0 = 0

  // // left-right
  // h = 2   y      bf = 0 - 2 = -2
  // h = 1 x        bf = 1 - 0 = 1
  // h = 0   z      bf = 0 - 0 = 0

  Node* rebalance(Node* node) {
    if (!node) {
      return nullptr;
    }

    update_height(node);
    int bf = balance_factor(node);

    if (bf < -1 && balance_factor(node->left) <= 0) {
      // LL
      return rotate_right(node);
    } else if (bf < -1 && balance_factor(node->left) > 0) {
      // LR
      return rotate_left_right(node);
    } else if (bf > 1 && balance_factor(node->right) >= 1) {
      // RR
      return rotate_left(node);
    } else if (bf > 1 && balance_factor(node->right) < 0) {
      // RL
      return rotate_right_left(node);
    }

    return node;
  }

  void clear(Node* node) {
    if (node) {
      clear(node->left);
      clear(node->right);
      delete node;
    }
  }

  Node* search_min(Node* node) {
    Node* cur = node;
    while (cur && cur->left) cur = cur->left;
    return cur;
  }

  Node* insert(Node* node, const ValueType& v) {
    if (!node) {
      return new Node(v);
    }

    const Key& key = v.first;
    const Key& node_key = node->data.first;

    if (cmp(key, node_key)) {
      node->left = insert(node->left, v);
      if (node->left) {
        node->left->parent = node;
      }
    } else if (cmp(node_key, key)) {
      node->right = insert(node->right, v);
      if (node->right) {
        node->right->parent = node;
      }
    } else {
      return node;
    }

    return rebalance(node);
  }

  Node* erase(Node* node, const Key& key) {
    if (!node) {
      return nullptr;
    }

    const Key& node_key = node->data.first;

    if (cmp(key, node_key)) {
      node->left = erase(node->left, key);
      if (node->left) node->left->parent = node;
    } else if (cmp(node_key, key)) {
      node->right = erase(node->right, key);
      if (node->right) node->right->parent = node;
    } else {
      if (!node->left || !node->right) {
        Node* tmp = node->left ? node->left : node->right;
        if (tmp) tmp->parent = node->parent;
        delete node;
        return tmp;
      } else {
        Node* suc = search_min(node->right);
        node->data = suc->data;
        node->right = erase(node->right, suc->data.first);
      }
    }
    return rebalance(node);
  }

  bool contains(Node* node, const Key& key) const {
    if (!node) {
      return false;
    }

    const Key& node_key = node->data.first;

    if (cmp(key, node_key)) {
      return contains(node->left, key);
    }
    if (cmp(node_key, key)) {
      return contains(node->right, key);
    }

    return true;
  }

  void print_tree(Node* node, int indent = 0) const {
    if (!node) return;
    const int SPACES = 2;
    if (node->right) print_tree(node->right, indent + SPACES);
    if (indent) std::cout << std::setw(indent) << ' ';
    std::cout << node->data.first << "\n";
    if (node->left) print_tree(node->left, indent + SPACES);
  }

 public:
  avl_tree() = default;
  avl_tree(const avl_tree& other) = delete;
  avl_tree(avl_tree&& other) = delete;
  avl_tree& operator=(const avl_tree& other) = delete;
  avl_tree& operator=(avl_tree&& other) = delete;
  ~avl_tree() { clear(); }

  void insert(const ValueType& v) { root = insert(root, v); }

  void erase(const Key& key) { root = erase(root, key); }

  bool contains(const Key& key) const { return contains(root, key); }

  void clear() {
    clear(root);
    root = nullptr;
  }

  void printTree() const { print_tree(root, 1); }
};

}  // namespace my
