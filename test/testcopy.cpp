#include <iostream>
#include <vector>
#include "../algorithm.h"
using namespace std;

int main(int argc, char *argv[])
{
    vector<int> v{1, 2, 3, 4, 5};
    vector<int> v2{10, 11, 12, 13, 14, 15, 16};

    learnSTL::copy_n(v.begin(), 3, v2.begin());
    for(auto& elem : v2){
        cout << elem << " ";
    }
    cout << endl;
    return 0;
}
