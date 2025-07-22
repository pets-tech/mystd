#include <iostream>
#include "list/forward_list.hpp"

int main() {
    my::forward_list<int> list;
    list.push_front(42);
    list.push_front(13);
    list.push_front(7);
    list.push_front(9);
    list.push_front(21);
    std::cout << list.front() << "\n";
    std::cout << list.front() << "\n";
    std::cout << list.front() << "\n";
}