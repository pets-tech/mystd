#include "array_stack/array.hpp"

#include <iostream>

int main() {

    // construction
    my::stack::array<int, 5> a1;
    my::stack::array<float, 5> a2;
    my::stack::array<double, 1000000> a21;
    my::stack::array<my::stack::array<int, 5>, 5> a8;
 
    // from initializer list
    my::stack::array<int, 5> a3 = {1,2,3,4,5};

    // copy
    my::stack::array<float, 5> a4 = a2;
    my::stack::array<float, 5> a41(a2);
    my::stack::array<float, 5> a42{a2};

    // elements access
    std::cout << a4[0] << " " << a2[0] << "\n";
    std::cout << a4.front() << " " << a2.front() << "\n";

    // modify
    a1.fill(13);
    a3.swap(a1);


}