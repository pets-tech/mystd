
mystd is a stl-like library.

[![GitHub Pages](https://img.shields.io/badge/docs-online-blue?logo=github)](https://pets-tech.github.io/mystd/) [![codecov](https://codecov.io/gh/pets-tech/mystd/graph/badge.svg?token=92FQ42MXUZ)](https://codecov.io/gh/pets-tech/mystd) [![mystd CI](https://github.com/pets-tech/mystd/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/pets-tech/mystd/actions/workflows/ci.yml)

> [!IMPORTANT]
> In fact, this is a demo, not realy production code. Real containers need weeks of dev and testing, not hours/days. Use at your own risk. I just implemented a lot of basic things quickly.


## Features

- [x] arrays
    - [x] static (std::array)   -> my::array
    - [x] dynamic (std::vector) -> my::vector
        - [ ] vector_buf
- [x] singly/doubly linked list
    - [x] singly linked (std::forwdard_list)
        - [x] index and array            -> my::arraybased::forward_list
        - [x] pointer and heap memory    -> my::heapbased::forward_list
    - [x] doubly linked (std::list)
        - [x] index and array            -> my::arraybased::list
        - [x] pointer and heap memory    -> my::heapbased::list
- [x] stack (adapter for deque, list, vector)
- [x] queue (adapter for deque, list)
- [x] deque
    - [x] on C-array (cyclic buffer)
    - [x] list of fixed blocks  -> std::deque
- [x] hash table
    - [x] hash table on separate chaining -> my::hashtable
        - [x] my::unordered_map         (todo: should be tested better)
        - [x] my::unordered_set         (todo: should be tested better)
        - [x] my::unordered_multimap    (todo: should be tested better)
        - [x] my::unordered_multiset    (todo: should be tested better)
    - [ ] hash table on open addressing
- [ ] trees
    - [x] binary tree
    - [x] binary search tree (BST)
        - [x] add iterator
    - [ ] ballanced tree (std::map)
        - [x] AVL
        - [x] red black
        - [ ] treep
        - [ ] std::map, std::set
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
