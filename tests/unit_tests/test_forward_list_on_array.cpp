#include <gtest/gtest.h>
#include "mystd/forward_list_fixed_array.hpp"

namespace my::arraybased {
namespace testings {

TEST(ForwardListTest, Initialization) {
    forward_list<int> a;
    EXPECT_TRUE(a.empty());
    EXPECT_EQ(a.size(), 0);

    forward_list<int, 2> b;
    EXPECT_EQ(b.capacity(), 2);
}

TEST(ForwardListTest, PushFrontAndFront) {
    forward_list<double> a;
    a.push_front(42.42);
    EXPECT_FALSE(a.empty());
    EXPECT_EQ(a.front(), 42.42);
    EXPECT_EQ(a.size(), 1);

    a.push_front(13.13);
    EXPECT_EQ(a.front(), 13.13);
    EXPECT_EQ(a.size(), 2);
}

TEST(ForwardListTest, PopFront) {
    forward_list<int> a;
    a.push_front(10);
    a.push_front(20);

    a.pop_front();
    EXPECT_EQ(a.size(), 1);
    EXPECT_EQ(a.front(), 10);
    
    a.pop_front();
    EXPECT_TRUE(a.empty());    
}

TEST(ForwardListTest, PopFrontOnEmptyList) {
    forward_list<int> a;
    EXPECT_THROW(a.pop_front(), std::runtime_error);
}

TEST(ForwardListTest, PushFrontWhenFull) {
    forward_list<int, 2> a;
    a.push_front(10);
    a.push_front(20);
    EXPECT_THROW(a.push_front(30), std::runtime_error);
}

TEST(ForwardListTest, CopyAndAssignment) {
    forward_list<int, 2> a;
    a.push_front(10);
    a.push_front(20);

    a = a;
    EXPECT_EQ(a.front(), 20);
    EXPECT_EQ(a.size(), 2);

    forward_list<int, 2> b(a);
    forward_list<int, 2> c = a;
    forward_list<int, 2> d = a = b = c;

    EXPECT_EQ(a.size(), b.size());
    EXPECT_EQ(a.size(), c.size());
    EXPECT_EQ(a.size(), d.size());

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
    EXPECT_EQ(a.size(), 2);

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

    // pretty nice insertion :)
    a.emplace_front(13, 42);
    EXPECT_EQ(a.front().x, 13);
    EXPECT_EQ(a.front().y, 42);
}

TEST(ForwardListTest, Iterators) {
    forward_list<int, 10> a;
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
    forward_list<int, 4> a;
    a.push_front(10);
    a.push_front(20);
    a.push_front(30);
    a.push_front(40);

    EXPECT_EQ(a.size(), 4);
    
    a.erase(1);
    EXPECT_EQ(a.size(), 3);

    a.erase(2);
    EXPECT_EQ(a.size(), 2);

    EXPECT_EQ(a.front(), 40);
    
    a.pop_front();
    EXPECT_EQ(a.front(), 10);
}


}
}