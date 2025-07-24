#include <gtest/gtest.h>
#include "mystd/forward_list_linked_nodes.hpp"

namespace my::heapbased {
namespace testings {

TEST(ForwardListTest, Initialization) {
    forward_list<int> a;
    EXPECT_TRUE(a.empty());

    forward_list<float> b = {1.0, 2.0, 3.0};
    EXPECT_EQ(b.front(), 1.0);
};

TEST(ForwardListTest, PushFrontAndFront) {
    forward_list<double> a;
    a.push_front(42.42);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.front(), 42.42);

    a.push_front(13.13);
    EXPECT_EQ(a.front(), 13.13);
}

TEST(ForwardListTest, PopFront) {
    forward_list<int> a;
    a.push_front(10);
    a.push_front(20);

    a.pop_front();
    EXPECT_EQ(a.front(), 10);
    
    a.pop_front();
    EXPECT_TRUE(a.empty());
}

TEST(ForwardListTest, PopFrontOnEmptyList) {
    forward_list<int> a;
    EXPECT_THROW(a.pop_front(), std::runtime_error);
}

TEST(ForwardListTest, CopyAndAssignment) {
    forward_list<int> a;
    a.push_front(10);
    a.push_front(20);

    a = a;
    EXPECT_EQ(a.front(), 20);

    forward_list<int> b(a);
    forward_list<int> c = a;
    forward_list<int> d = a = b = c;

    EXPECT_EQ(a.front(), b.front());
    EXPECT_EQ(a.front(), c.front());

    b.pop_front();
    EXPECT_EQ(a.front(), 20);
    EXPECT_EQ(b.front(), 10);
    c.pop_front(); c.pop_front();
    EXPECT_TRUE(c.empty());
}

TEST(ForwardListTest, MoveConstructor) {
    forward_list<int> a;
    a.push_front(1);
    a.push_front(2);

    a = std::move(a);
    EXPECT_EQ(a.front(), 2);

    forward_list<int> b = std::move(a);
    EXPECT_EQ(b.front(), 2);
    EXPECT_TRUE(a.empty());

    forward_list<int> c(std::move(b));
    EXPECT_EQ(c.front(), 2);
    EXPECT_TRUE(b.empty());
}

TEST(ForwardListTest, CustomType) {
    struct Point { int x, y; };
    forward_list<Point> a;
    a.push_front(Point{1, 2});
    a.push_front(Point{3, 4});

    EXPECT_EQ(a.front().x, 3);
    EXPECT_EQ(a.front().y, 4);
}

TEST(ForwardListTest, Iterators) {
    forward_list<int> a;
    a.push_front(1);
    a.push_front(2);
    a.push_front(3); // it is head

    // go from head to tail 3 -> 2 -> 1
    size_t i = 3;
    for (auto it = a.begin(); !(it == a.end()); ++it) {
        EXPECT_EQ(*it, i--);
    }
    
    i = 3;
    for (const auto& item : a) {
        EXPECT_EQ(item, i--);
    }
}

TEST(ForwardListTest, ElemetErase) {
    forward_list<int> a;
    a.push_front(10);
    a.push_front(20);
    a.push_front(30);
    a.push_front(40);

    auto it = a.begin();
    ++it;
    a.erase(it);

    EXPECT_EQ(a.front(), 40);

    it = a.begin();
    ++it;
    a.erase(it);

    a.pop_front();
    EXPECT_EQ(a.front(), 10);
}

}
}