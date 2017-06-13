#include "../forward_list.h"
#include <iostream>
#include <cassert>
#include <algorithm>
using namespace std;

int main(){
    learnSTL::forward_list<int> f1{2, 2, 3, 3, 60};
    learnSTL::forward_list<int> f2{2, 2, 3, 3 };
    cout << boolalpha << (f1 <= f2) << endl;
    cout << "SUCCESS test forward_list" << endl;
}
