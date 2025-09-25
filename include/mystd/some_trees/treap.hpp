#pragma once

#include <functional>
#include <iomanip>
#include <random>
#include <stack>
#include <type_traits>
#include <utility>

#include "mystd/iterator/iterator.hpp"

namespace my {

/// @brief Tree + heap
/// ref: https://neerc.ifmo.ru/wiki/index.php?title=Декартово_дерево
template <typename ValueType, typename KeyOfValue,
          typename Compare = std::less<std::invoke_result_t<KeyOfValue, ValueType>>>
class treap {
 public:
  using value_type = ValueType;
  using key_type = std::invoke_result_t<KeyOfValue, ValueType>;
  using key_extractor = KeyOfValue;
  using key_compare = Compare;
  using size_type = std::size_t;

 private:
  struct Node {
    value_type value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
    std::uint64_t priority;
    Node(const value_type& v, std::uint64_t p) : value(v), priority(p) {}
    Node(value_type&& v, std::uint64_t p) : value(std::move(v)), priority(p) {}
  };

  Node* root_ = nullptr;
  size_type size_ = 0;
  key_extractor key_of{};
  key_compare comp{};
  std::mt19937_64 rnd{std::random_device{}()};

 public:
  using iterator = my::bidirectional_iterator<ValueType, Node*, false, std::bidirectional_iterator_tag,
                                              TreeTraversalPolicy<Node*>, DefaultValueExtractor<ValueType>>;
  using const_iterator = my::bidirectional_iterator<ValueType, const Node*, true, std::bidirectional_iterator_tag,
                                                    TreeTraversalPolicy<const Node*>, DefaultValueExtractor<ValueType>>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

 private:
  bool key_less(const key_type& a, const key_type& b) const { return comp(a, b); }

  bool key_equal(const key_type& a, const key_type& b) const { return !comp(a, b) && !comp(b, a); }

  void split(Node* node, const key_type& key, Node*& left, Node*& right) {
    if (node == nullptr) {
      left = right = nullptr;
      return;
    }
    if (key_less(key, key_of(node->value))) {
      split(node->left, key, left, node->left);
      if (node->left) node->left->parent = node;
      right = node;
    } else {
      split(node->right, key, node->right, right);
      if (node->right) node->right->parent = node;
      left = node;
    }
  }

  Node* merge(Node* t1, Node* t2) {
    if (t1 == nullptr) return t2;
    if (t2 == nullptr) return t1;
    if (t1->priority > t2->priority) {
      t1->right = merge(t1->right, t2);
      if (t1->right) t1->right->parent = t1;
      return t1;
    } else {
      t2->left = merge(t1, t2->left);
      if (t2->left) t2->left->parent = t2;
      return t2;
    }
  }

  Node* insert_node(Node* t, Node* node) {
    if (t == nullptr) return node;

    if (node->priority > t->priority) {
      split(t, key_of(node->value), node->left, node->right);
      if (node->left) node->left->parent = node;
      if (node->right) node->right->parent = node;
      node->parent = t ? t->parent : nullptr;
      return node;
    }

    if (key_less(key_of(node->value), key_of(t->value))) {
      t->left = insert_node(t->left, node);
      if (t->left) t->left->parent = t;
    } else {
      t->right = insert_node(t->right, node);
      if (t->right) t->right->parent = t;
    }

    return t;
  }

  Node* erase_node(Node* t, const key_type& key) {
    if (t == nullptr) return nullptr;

    if (key_equal(key, key_of(t->value))) {
      Node* tmp = merge(t->left, t->right);
      if (tmp) tmp->parent = t->parent;
      delete t;
      --size_;
      return tmp;
    }

    if (key_less(key, key_of(t->value))) {
      t->left = erase_node(t->left, key);
    } else {
      t->right = erase_node(t->right, key);
    }

    return t;
  }

  Node* find_node(Node* t, const key_type& key) const {
    while (t) {
      if (key_equal(key, key_of(t->value))) {
        return t;
      }
      if (key_less(key, key_of(t->value))) {
        t = t->left;
      } else {
        t = t->right;
      }
    }
    return nullptr;
  }

  Node* lower_bound_node(Node* t, const key_type& key) const {
    Node* res = nullptr;
    while (t) {
      if (!key_less(key_of(t->value), key)) {
        res = t;
        t = t->left;
      } else {
        t = t->right;
      }
    }
    return res;
  }

  static Node* search_min(Node* node) {
    if (node == nullptr) return nullptr;
    while (node->left) node = node->left;
    return node;
  }

  void clear_nodes(Node* t) {
    if (t == nullptr) return;
    clear_nodes(t->left);
    clear_nodes(t->right);
    delete t;
  }

  Node* safe_copy(Node* root) {
    // copy nodes data only
    if (!root) {
      return nullptr;
    }

    Node* new_root = nullptr;
    try {
      new_root = new Node(root->value, root->priority);
      std::stack<std::pair<Node*, Node*>> stack;
      stack.push({root, new_root});

      while (!stack.empty()) {
        auto [old_node, new_node] = stack.top();
        stack.pop();

        if (old_node->right) {
          new_node->right = new Node(old_node->right->value, old_node->right->priority);
          new_node->right->parent = new_node;
          stack.push({old_node->right, new_node->right});
        }

        if (old_node->left) {
          new_node->left = new Node(old_node->left->value, old_node->left->priority);
          new_node->left->parent = new_node;
          stack.push({old_node->left, new_node->left});
        }
      }
    } catch (...) {
      clear_nodes(new_root);
      throw;
    }
    return new_root;
  }

 public:
  treap() = default;
  ~treap() { clear(); }

  treap(std::initializer_list<value_type> init) {
    for (const auto& el : init) {
      insert(el);
    }
  }

  treap(const treap& other) {
    root_ = safe_copy(other.root_);
    size_ = other.size_;
  }

  treap(treap&& other) : root_(std::exchange(other.root_, nullptr)), size_(std::exchange(other.size_, 0)) {}

  treap& operator=(const treap& other) {
    if (this != &other) {
      treap tmp(other);
      swap(tmp);
    }
    return *this;
  }

  treap& operator=(treap&& other) {
    if (this != &other) {
      clear();
      root_ = std::exchange(other.root_, nullptr);
      size_ = std::exchange(other.size_, 0);
    }
    return *this;
  }

  void clear() {
    clear_nodes(root_);
    root_ = nullptr;
    size_ = 0;
  }

  void swap(treap& other) {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  void insert(const value_type& value) {
    auto key = key_of(value);
    Node* node = new Node(value, rnd());
    root_ = insert_node(root_, node);
    if (root_) root_->parent = nullptr;
    ++size_;
  }

  void erase(const key_type& key) { root_ = erase_node(root_, key); }

  bool contains(const key_type& key) const { return find_node(root_, key) != nullptr; }

  iterator find(const key_type& key) {
    Node* found = find_node(root_, key);
    return iterator(found);
  }

  size_type size() const { return size_; }
  bool empty() const { return size_ == 0; }

  // iterators

  iterator begin() { return iterator(search_min(root_)); }
  iterator end() { return iterator(nullptr); }
  const_iterator begin() const { return const_iterator(search_min(root_)); }
  const_iterator end() const { return const_iterator(nullptr); }
  const_iterator cbegin() const { return const_iterator(search_min(root_)); }
  const_iterator cend() const { return const_iterator(nullptr); }

  // printer

  /// for value_type not pair
  void print_tree(Node* node, int indent = 0, bool debug = true) const {
    if (!node) return;
    const int SPACES = 2;
    print_tree(node->right, indent + SPACES);
    std::cout << std::setw(indent) << ' ';

    if (debug) {
      std::cout << node->value << " " << node << " parent=" << (node->parent ? node->parent : 0)
                << " left=" << (node->left ? node->left : 0) << " right=" << (node->right ? node->right : 0) << "\n";
    } else {
      std::cout << node->value << "\n";
    }
    print_tree(node->left, indent + SPACES);
  }

  void print() { print_tree(root_, 0); }
};

}  // namespace my
