#include <iostream>
#include <cassert>
#include <cstring>
#include <string>
#include "../tuple.h"
using namespace std;

int main()
{
    // test constructor3
    learnSTL::tuple<int, int, int> t1(1, 2, 3);
    assert(learnSTL::get<1>(t1) == 2);

    // test constructor1
    learnSTL::tuple<> emptyT;
    static_assert(!learnSTL::tuple_size<decltype(emptyT)>::value, "error in default constructor.");

    // test constructor2
    const int x = 13, y = 14, z = 520;
    learnSTL::tuple<int, int, float> t2(x, y, z);
    assert(learnSTL::get<2>(t2) == 520);

    // test copy constructor
    learnSTL::tuple<int, double, int> t1_c(t1);
    assert(t1_c == t1);

    // test move constructor
    learnSTL::tuple<int, int, float> t2_m(learnSTL::move(t2));
    assert(t2_m == t2);

    // test pair constructor
    learnSTL::pair<int, int> p(6, 66);
    learnSTL::tuple<int, int> t3(p);
    assert(learnSTL::tuple_size<decltype(t3)>::value == 2 &&
            learnSTL::get<1>(t3) == 66);

    // test compare
    assert(t1 < t2 && t2 > t1);
    assert(t1 <= t2 && t2 >= t1);

    // test make_tuple
    auto t4 = learnSTL::make_tuple(6, 66);
    assert(t4 == t3);

    // test tie
    int a = 10;
    auto t5 = learnSTL::tie(a);
    a = 20;
    assert(learnSTL::get<0>(t5) == 20);

    // test forward_as_tuple
    auto t6 = learnSTL::forward_as_tuple(learnSTL::move(a));
    assert(t6 == t5);

    cout << "SUCCESS test tuple.h" << endl;
    return 0;
}
