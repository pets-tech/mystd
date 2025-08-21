#pragma once
#include <limits>
#include <stack>
#include <vector>

namespace my {

enum class TraversalType { INORDER, PREORDER, POSTORDER };

template <class T, class Compare = std::less<T>>
class binary_tree {
 private:
  struct Node {
    T value;
    Node* left;
    Node* right;
    Node(T v) : value(v), left(nullptr), right(nullptr) {}
  };

  Node* root = nullptr;
  Compare comp;

  Node* insert_rimpl(Node* current, T value) {
    if (current == nullptr) {
      return new Node(value);
    }

    if (comp(value, current->value)) {
      current->left = insert_rimpl(current->left, value);
    } else {
      current->right = insert_rimpl(current->right, value);
    }

    return current;
  }

  Node* find_rimpl(Node* current, const T& value) const {
    if (!current) return nullptr;
    if (!comp(value, current->value) && !comp(current->value, value)) return current;
    if (comp(value, current->value)) return find_rimpl(current->left, value);
    return find_rimpl(current->right, value);
  }

  Node* findmin_rimpl(Node* current) const {
    if (!current) return nullptr;
    while (current->left) current = current->left;
    return current;
  }

  Node* remove_rimpl(Node* current, T value) {
    if (current == nullptr) {
      return nullptr;
    }

    if (comp(value, current->value)) {
      current->left = remove_rimpl(current->left, value);
    } else if (comp(current->value, value)) {
      current->right = remove_rimpl(current->right, value);
    } else {
      // no childred
      if (current->left == nullptr && current->right == nullptr) {
        delete current;
        return nullptr;
      }

      // only right child
      if (current->left == nullptr) {
        Node* tmp = current->right;
        delete current;
        return tmp;
      }

      // only left child
      if (current->right == nullptr) {
        Node* tmp = current->left;
        delete current;
        return tmp;
      }

      // both childeren
      Node* min_right = findmin_rimpl(current->right);
      current->value = min_right->value;
      current->right = remove_rimpl(current->right, min_right->value);
    }
    return current;
  }

  // left -> vert -> right
  void inorder_rimpl(Node* current, std::vector<T>& result) {
    if (current) {
      inorder_rimpl(current->left, result);
      result.push_back(current->value);
      inorder_rimpl(current->right, result);
    }
  }

  // vert -> left -> right
  void preorder_rimpl(Node* current, std::vector<T>& result) {
    if (current) {
      result.push_back(current->value);
      preorder_rimpl(current->left, result);
      preorder_rimpl(current->right, result);
    }
  }

  // left -> right -> vert
  void postorder_rimpl(Node* current, std::vector<T>& result) {
    if (current) {
      postorder_rimpl(current->left, result);
      postorder_rimpl(current->right, result);
      result.push_back(current->value);
    }
  }

  Node* safe_copy(Node* root) {
    if (!root) {
      return nullptr;
    }

    Node* new_root = nullptr;
    try {
      new_root = new Node(root->value);
      std::stack<std::pair<Node*, Node*>> stack;
      stack.push({root, new_root});

      while (!stack.empty()) {
        auto [old_node, new_node] = stack.top();
        stack.pop();

        if (old_node->right) {
          new_node->right = new Node(old_node->right->value);
          stack.push({old_node->right, new_node->right});
        }

        if (old_node->left) {
          new_node->left = new Node(old_node->left->value);
          stack.push({old_node->left, new_node->left});
        }
      }
    } catch (...) {
      clear(new_root);
      throw;
    }
    return new_root;
  }

  void clear(Node* current) {
    if (current) {
      clear(current->left);
      clear(current->right);
      delete current;
    }
  }

 public:
  explicit binary_tree() {}
  binary_tree(const binary_tree& other) { root = safe_copy(other.root); }
  binary_tree(binary_tree&& other) : root(std::exchange(other.root, nullptr)) {};

  binary_tree& operator=(const binary_tree& other) {
    if (this != &other) {
      binary_tree tmp(other);
      swap(tmp);
    }
    return *this;
  }

  binary_tree& operator=(binary_tree&& other) {
    if (this != &other) {
      clear(root);
      root = std::exchange(other.root, nullptr);
    }
    return *this;
  }

  ~binary_tree() { clear(root); }

  void clear() { clear(root); }

  void insert(const T& value) { root = insert_rimpl(root, value); }

  std::vector<T> traversal(TraversalType type) {
    std::vector<T> result;
    switch (type) {
      case TraversalType::PREORDER:
        preorder_rimpl(root, result);
        break;
      case TraversalType::POSTORDER:
        postorder_rimpl(root, result);
        break;
      default:
        inorder_rimpl(root, result);
        break;
    }
    return result;
  }

  bool search(const T& key) {
    if (find_rimpl(root, key)) {
      return true;
    }
    return false;
  }

  std::optional<T> findmin() {
    Node* node = findmin_rimpl(root);
    if (node) {
      return node->value;
    }
    return std::nullopt;
  }

  void remove(const T& value) { root = remove_rimpl(root, value); }

  void swap(binary_tree& other) noexcept { std::swap(root, other.root); }
};

}  // namespace my
