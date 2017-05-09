#include <iostream>
#include <cassert>
#include <cstddef>
#include "../array.h"

using namespace std;

// TEST array.h
int main()
{
    typedef typename learnSTL::array<int, 5> Array5;
    int x = 0;
    Array5::reference a_ref = x;
    Array5::const_reference a_cref= (const int&)x;
    Array5::size_type* p_sz = (size_t*)x;
    Array5::difference_type* p_diff = (ptrdiff_t*)0;
    Array5::value_type* p_val = (int*)0;
    Array5::pointer a_p = (int*)0;
    Array5::const_pointer a_cp = (const int*)0;

    Array5 a;
    // capacity
    assert(!a.empty() && a.size() == 5 && a.max_size() == 5);

    // assignment and element access
    a.fill(2);
    assert(a.front() == 2 && a.back() == 2 && a[2] == 2);

    // tuple like
    //assert(learnSTL::tuple_size<Array5>::value == 5);
    //typename learnSTL::tuple_element<1, Array5>::type* t_p = (int*)0;
    //assert(learnSTL::get<1>(a) == 2);

    // swap
    Array5 a2 = {3, 4, 5, 6, 7};
    learnSTL::swap(a, a2);
    assert(a[0] == 3 && a2[0] == 2);
    cout << "SUCCESS TEST array.h" << endl;
    return 0;
}
