#include "../numeric.h"
#include "../algorithm.h"
#include <iostream>
#include <functional>
#include <array>
#include <cassert>

using namespace std;

void test_iota(){
    array<int, 5> v = {0, 0, 0, 0, 0};
    array<int, 5> ans = {0, 1, 2, 3, 4};
    learnSTL::iota(v.begin(), v.end(), 0);
    assert(learnSTL::equal(v.begin(), v.end(), ans.begin()));
}

void test_accumulate(){
    array<int, 9> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    assert(learnSTL::accumulate(v.begin(), v.end(), 0) == 45);
    assert(learnSTL::accumulate(v.begin(), v.end(), -100) == -55);
    assert(learnSTL::accumulate(v.begin(), v.end(), 1, multiplies<int>()) == 362880);
}

void test_inner_product(){
    array<int, 6> v = {1, 2, 3, 4, 5, 6};
    assert(learnSTL::inner_product(v.begin(), v.end(), v.rbegin(), 0) == 56);
    assert(learnSTL::inner_product(v.cbegin(), v.cend(), v.cbegin(), 1, multiplies<int>(), plus<int>()) == 46080);
}

void test_partial_sum(){
    array<int, 6> v = {1, 2, 3, 4, 5, 6};
    array<int, 6> ret;
    array<int, 6> ans = {1, 3, 6, 10, 15, 21};
    learnSTL::partial_sum(v.begin(), v.end(), ret.begin());
    assert(learnSTL::equal(ret.begin(), ret.end(), ans.begin()));
}

void test_adjacent_difference(){
    array<int, 6> v = {1, 2, 3, 4, 5, 6};
    array<int, 6> ret;
    array<int, 6> ans = {1, 1, 1, 1, 1, 1};
    learnSTL::adjacent_difference(v.begin(), v.end(), ret.begin());
    assert(learnSTL::equal(ret.begin(), ret.end(), ans.begin()));
}

int main()
{
    test_iota();
    test_accumulate();
    test_inner_product();
    test_partial_sum();
    test_adjacent_difference();
    cout << "SUCCESS TEST NUMERIC." << endl;
    return 0;
}
