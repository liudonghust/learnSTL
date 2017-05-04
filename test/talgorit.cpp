// test "algorithm"
#include <cassert>
#include <cctype>
#include <iostream>
#include <cstring>
#include <functional>
#include <vector>
#include <initializer_list>
#include "../algorithm.h"
using namespace std;

// FUNCTION OBJECTS
equal_to<char> equf;
less<char> lessf;

bool veccmp(vector<int>& v, initializer_list<int> initlist)
{
    return v.size() == initlist.size() && learnSTL::equal(v.begin(), v.end(), initlist.begin());
}

// TEST RANGE-COMPARE TEMPLATE FUNCTIONS
void test_range_compate(char* first, char* last)
{
   strcpy(first, "abccefg");
   assert(learnSTL::equal(first, last, first));
   assert(learnSTL::equal(first, last, first, equf));
   assert(learnSTL::lexicographical_compare(first, last - 1, first, last));
   assert(learnSTL::lexicographical_compare(first, last - 1, first, last, lessf));
   assert(learnSTL::mismatch(first, last, first).second == last);
   assert(learnSTL::mismatch(first, last, first, equf).second == last);
}

void test_copy(char* first, char* last)
{
    strcpy(first, "abcdefg");
    learnSTL::copy(first, first+3, first + 1);
    assert(strcmp(first, "aaaaefg")== 0);
    learnSTL::copy_backward(first+2, first+5, first+3);
    assert(strcmp(first, "eaeaefg") == 0);
    learnSTL::fill(first, first + 3, 'x');
    assert(strcmp(first, "xxxaefg") == 0);
    learnSTL::fill_n(first, 2, 'y');
    assert(strcmp(first, "yyxaefg") == 0);
    vector<int> v{0, 0, 0 , 0, 0, 0};
    learnSTL::iota(v.begin(), v.end(), 1);
    assert(veccmp(v, {1, 2, 3, 4, 5, 6}));
}

int main()
{
    char buf[] = "abccefg";
    char dest[] = "1234567";
    char *first = buf;
    char *last = buf + 7;
    test_range_compate(first, last);
    test_copy(first, last);
    cout << "SUCCESS TEST algorithm" << endl;
    return 0;
}
