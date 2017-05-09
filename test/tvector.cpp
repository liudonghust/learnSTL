#include <cassert>
#include <iostream>
#include <memory>
#include "../iterator.h"
#include "../vector.h"
using namespace std;

// TEST vector
int main()
{
    typedef std::allocator<char> Myal;
    // TEST vector
    typedef learnSTL::vector<char, Myal> Mycont;
    char ch, carr[] = "abc";
    Mycont::allocator_type* p_alloc = (Myal*)0;
    Mycont::pointer p_ptr = (char*)0;
    Mycont::const_pointer p_cptr = (const char*)0;
    Mycont::reference p_ref = ch;
    Mycont::const_reference p_cref = (const char&)ch;
    Mycont::value_type* p_val = (char*)0;
    Mycont::size_type* p_size = (size_t*)0;
    Mycont::difference_type* p_diff = (ptrdiff_t*)0;
    Mycont v0;
    Myal al = v0.get_allocator();
    Mycont v0a(al);
    assert(v0.empty() && v0.size() == 0);
    assert(v0a.size() == 0 && v0a.get_allocator() == al);
    Mycont v1(5), v1a(6, 'x'), v1b(7, 'y', al);
    assert(v1.size() == 5 && v1.back() == '\0');
    assert(v1a.size() == 6 && v1a.back() == 'x');
    assert(v1b.size() == 7 && v1b.back() == 'y');
    Mycont v2(v1a);
    assert(v2.size() == 6 && v2.front() == 'x');
    Mycont v3(v1a.begin(), v1a.end());
    assert(v3.size() == 6 && v3.front() == 'x');
    const Mycont v4(v1a.begin(), v1a.end(), al);
    assert(v4.size() == 6 && v4.front() == 'x');
    v0 = v4;
    assert(v0.size() == 6 && v0.front() == 'x');
    assert(v0[0] == 'x' && v0.at(5) == 'x');

    v0.reserve(12);
    assert(12 <= v0.capacity());
    v0.resize(8);
    assert(v0.size() == 8 && v0.back() == '\0');
    v0.resize(10, 'z');
    assert(v0.size() == 10 && v0.back() == 'z');
    assert(v0.size() <= v0.max_size());

    Mycont::iterator p_it(v0.begin());
    Mycont::const_iterator p_cit(v4.begin());
    Mycont::reverse_iterator p_rit(v0.rbegin());
    Mycont::const_reverse_iterator p_crit(v4.rbegin());
    assert(*p_it == 'x' && *--(p_it = v0.end()) == 'z');
    assert(*p_cit == 'x' && *--(p_cit = v4.end()) == 'x');
    assert(*p_rit == 'z' && *--(p_rit = v0.rend()) == 'x');
    assert(*p_crit == 'x' && *--(p_crit = v4.rend()) == 'x');

    assert(v0.front() == 'x' && v4.front() == 'x');
    v0.push_back('a');
    assert(v0.back() == 'a');
    v0.pop_back();
    assert(v0.back() == 'z' && v4.back() == 'x');

    v0.assign(v4.begin(), v4.end());
    assert(v0.size() == v4.size() && v0.front() == v4.front());
    v0.assign(4, 'w');
    assert(v0.size() == 4 && v0.front() == 'w');
    assert(*v0.insert(v0.begin(), 'a') == 'a');
    assert(v0.front() == 'a' && *learnSTL::next(v0.begin()) == 'w');
    v0.insert(v0.begin(), 2, 'b');
    assert(v0.front() == 'b' && *learnSTL::next(v0.begin()) == 'b' && *learnSTL::next(learnSTL::next(v0.begin())) == 'a');
    v0.insert(v0.end(), v4.begin(), v4.end());
    assert(v0.back() == v4.back());
    v0.insert(v0.end(), carr, carr + 3);
    assert(v0.back() == 'c');
    v0.erase(v0.begin());
    assert(v0.front() == 'b' && *learnSTL::next(v0.begin()) == 'a');
    v0.erase(v0.begin(), learnSTL::next(v0.begin()));
    assert(v0.front() == 'a');
    v0.clear();
    assert(v0.empty());
    v0.swap(v1);
    assert(!v0.empty() && v1.empty());
    swap(v0, v1);
    assert(v0.empty() && !v1.empty());
    assert(v1 == v1 && v0 < v1);
    assert(v0 != v1 && v1 > v0);
    assert(v0 <= v1 && v1 >= v0);

    cout << "SUCCESS testing vector.h" << endl;

    return 0;
}
