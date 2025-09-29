
#pragma once

class Trie {
 private:
  struct Node {
    Node* children[26];
    bool is_word;

    Node() {
      for (size_t i = 0; i < 26; ++i) {
        children[i] = nullptr;
      }
      is_word = false;
    }
  };
  Node* root_ = nullptr;

  void clear(Node* node) {
    if (node == nullptr) return;
    for (size_t i = 0; i < 26; ++i) {
      clear(node->children[i]);
    }
    delete node;
  }

 public:
  Trie() { root_ = new Node(); }

  ~Trie() { clear(root_); }

  void insert(string word) {
    Node* node = root_;
    for (auto ch : word) {
      int idx = ch - 'a';
      if (node->children[idx] == nullptr) {
        node->children[idx] = new Node();
      }
      node = node->children[idx];
    }
    node->is_word = true;
  }

  bool search(string word) {
    Node* node = root_;
    for (auto ch : word) {
      int idx = ch - 'a';
      if (node->children[idx] == nullptr) return false;
      node = node->children[idx];
    }
    return node->is_word;
  }

  bool startsWith(string prefix) {
    Node* node = root_;
    for (auto ch : prefix) {
      int idx = ch - 'a';
      if (node->children[idx] == nullptr) return false;
      node = node->children[idx];
    }
    return true;
  }
};
