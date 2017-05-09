#include "../numeric.h"
#include <iostream>
#include <functional>
#include <cassert>
#include <cstring>

using namespace std;

multiplies<char> multipliesf;
plus<char> plusf;

int main()
{
    char buf[] = "\1\2\3\4\5\6";
    char dest[] = "123456";
    char *first = buf, *last=buf+6;
    char val = 0;
    assert(learnSTL::accumulate(first, last, val) == 21);
    assert(learnSTL::accumulate(first, last, val, plusf) == 21);
    assert(learnSTL::inner_product(first, last, first, val) == 91);
    assert(learnSTL::inner_product(first, last, first, val, plusf, multipliesf) == 91);
    learnSTL::partial_sum(first, last, dest);
    assert(strcmp(dest, "\1\3\6\12\17\25") == 0);
    learnSTL::adjacent_difference(first, last, dest);
    assert(strcmp(dest, "\1\1\1\1\1\1") == 0);
    learnSTL::partial_sum(first, last, dest, plusf);
    assert(strcmp(dest, "\1\3\6\12\17\25") == 0);
    learnSTL::adjacent_difference(first, last, dest, plusf);
    assert(strcmp(dest, "\1\3\5\7\11\13") == 0);

    cout << "SUCCESS testing numeric.h" << endl;
    return 0;
}
