
mystd is a stl-like library.

[![GitHub Pages](https://img.shields.io/badge/docs-online-blue?logo=github)](https://pets-tech.github.io/mystd/) [![codecov](https://codecov.io/gh/pets-tech/mystd/graph/badge.svg?token=92FQ42MXUZ)](https://codecov.io/gh/pets-tech/mystd) [![mystd CI](https://github.com/pets-tech/mystd/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/pets-tech/mystd/actions/workflows/ci.yml)


## Features

- [x] arrays
    - [x] static (std::array)   -> my::array
    - [x] dynamic (std::vector) -> my::vector
- [x] singly/doubly linked list
    - [x] singly linked (std::forwdard_list)
        - [x] index and array            -> my::arraybased::forward_list
        - [x] pointer and heap memory    -> my::heapbased::forward_list
    - [x] doubly linked (std::list)
        - [x] index and array            -> my::arraybased::list
        - [x] pointer and heap memory    -> my::heapbased::list
- [x] stack (adapter for deque, list, vector)
- [ ] queue (adapter for deque, list)
- [ ] deque
    - [ ] block array
    - [ ] on array
    - [ ] on list
- [ ] hash table
    - [ ] chaining (std::unordered_map)
    - [ ] open addressing
- [ ] trees
    - [ ] binary tree
    - [ ] binary search tree (BST)
    - [ ] ballanced tree (std::map)
        - [ ] AVL
        - [ ] red black
    - [ ] trie
    - [ ] segment tree
- [ ] heaps
    - [ ] binary heap (max/min)
        - priority queue
        - Dijkstra’s algorithm
- [ ] graphs
    - [ ] Adjacency list/matrix
    - [ ] DFS, BFS
    - [ ] DSU
    - [ ] Skip List
