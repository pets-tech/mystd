#include "array_heap/array.hpp"

#include <iostream>

int main() {

    my::heap::array<int, 1000000000> arr;

    arr.fill(42);

    std::cout << arr[0] << "\n";
}