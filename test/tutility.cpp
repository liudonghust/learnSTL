#include <iostream>
#include <functional>
#include <cassert>
#include <cstring>
#include "../utility.h"
using namespace std;

void test_make_pair(){
    int n = 1;
    int a[5] = {1, 2, 3, 4, 5};

    auto p1 = learnSTL::make_pair(n, a[1]);
    assert(p1.first == 1 && p1.second == 2);
    auto p2 = learnSTL::make_pair(ref(n), a);
    n = 7;
    assert(p2.first == 7 && *(p2.second+2) == 3);
}

void test_swap(){
    char a1[] = "ebcd";
    char a2[] = "afgh";
    learnSTL::swap(a1[0], a2[0]);
    assert(strcmp(a1, "abcd") == 0);
    assert(strcmp(a2, "efgh") == 0);
}

typedef learnSTL::pair<int, char> Pair_ic;
Pair_ic p0;

class Int{
public:
    Int(int v) : val(v) {}
    bool operator==(Int x) const { return val == x.val; }
    bool operator<(Int x) const { return val < x.val; }
private:
    int val;
};

int main()
{
    Pair_ic p1 = p0, p2(3, 'a');

    // TEST pair
    assert(p1.first == 0);
    assert(p1.second == 0);
    assert(p2.first == 3);
    assert(p2.second == 'a');
    assert(p2 == learnSTL::make_pair((Pair_ic::first_type)3, (Pair_ic::second_type)'a'));
    assert(p2 < learnSTL::make_pair((Pair_ic::first_type)4, (Pair_ic::second_type)'a'));
    assert(p2 < learnSTL::make_pair((Pair_ic::first_type)3, (Pair_ic::second_type)'b'));
    assert(p1 != p2);
    assert(p2 > p1);
    assert(p2 <= p2);
    assert(p2 >= p2);

    test_make_pair();

    // test rel_ops
    using namespace learnSTL::rel_ops;
    Int a(2), b(3);
    assert(a == a);
    assert(a < b);
    assert(a != b);
    assert(b > a);
    assert(a <= b);
    assert(b >= a);
    cout << "SUCCESS testing utility.h" << endl;
    return 0;
}

