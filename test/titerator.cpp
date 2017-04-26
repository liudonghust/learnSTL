#include "../iterator.h"
#include <assert.h>
#include <string>
#include <strstream>
#include <iostream>
#include <deque>
#include <cstddef>
using namespace learnSTL;
typedef char* PtrIt;

// TEST GENERAL PROPERTY TEMPLATES
void takes_ran_tag(random_access_iterator_tag) {}

void test_prop()
{
    random_access_iterator_tag* ran_tag = (random_access_iterator_tag*)0;
    bidirectional_iterator_tag* bid_tag = (bidirectional_iterator_tag*)0;
    forward_iterator_tag* fwd_tag = (forward_iterator_tag*)0;
    input_iterator_tag* in_tag = (input_iterator_tag*)0;
    output_iterator_tag* p_out_tag = 0;

    typedef iterator<input_iterator_tag, float, short, float*, float&> Iter;
    float fl;
    Iter::iterator_category* it_tag = (input_iterator_tag*)0;
    Iter::value_type* it_val = (float*)0;
    Iter::difference_type* it_dist = (short*)0;
    Iter::pointer it_ptr = (float*)0;
    Iter::reference it_ref = fl;

    typedef iterator_traits<Iter> Traits;
    Traits::iterator_category* tr_tag = (input_iterator_tag*)0;
    Traits::value_type* tr_val = (float*)0;
    Traits::difference_type* tr_dist = (short*)0;
    Traits::pointer tr_ptr = (float*)0;
    Traits::reference tr_ref = fl;
    typedef iterator_traits<PtrIt> Ptraits;
    char ch;
    takes_ran_tag(Ptraits::iterator_category());
    Ptraits::value_type* ptr_val = (char*)0;
    Ptraits::difference_type* ptr_dist = (ptrdiff_t*)0;
    Ptraits::pointer ptr_ptr = (char*)0;
    Ptraits::reference ptr_ref = ch;

    const char* pc = "abcdefg";
    advance(pc, 4);
    assert(*pc == 'e');
    advance(pc, -1);
    assert(*pc == 'd');
    assert(distance(pc, pc+3) ==3);
}

// TEST reverse_iterator
typedef reverse_iterator<PtrIt> RevIt;
class MyrevIt : public RevIt
{
public:
    MyrevIt(RevIt::iterator_type p) : RevIt(p) {}
    RevIt::iterator_type get_current() const { return (current); }
};


void test_revit()
{
    char* pc = (char*)"abcdefg" + 3;
    PtrIt pcit(pc);
    RevIt::iterator_type* p_iter = (PtrIt*)0;
    RevIt rit0, rit(pcit);

    assert(rit.base() == pcit);
    assert(*rit == 'c');
    assert(*++rit == 'b');
    assert(*rit++ == 'b' && *rit == 'a');
    assert(*--rit == 'b');
    assert(*rit-- == 'b' && *rit == 'c');
    assert(*(rit+=2) == 'a');
    assert(*(rit-=2) == 'c');
    assert(rit[2] == 'a');
    assert(rit == rit);
    assert(!(rit < rit) && rit < rit + 1);
    assert((rit+2) - rit == 2);

    MyrevIt myrit(pc);
    assert(myrit.get_current() == pcit);
}

// TEST INSERTION ITERATORS
typedef std::deque<char> Cont;
typedef back_insert_iterator<Cont> BackIt;
class MyBackIt : public BackIt
{
public:
    MyBackIt(BackIt::container_type& c) : BackIt(c) {}
    BackIt::container_type* get_container() const
    {
        return containerPtr;
    }
};

typedef front_insert_iterator<Cont> FrontIt;
class MyfrontIt : public FrontIt
{
public:
    MyfrontIt(FrontIt::container_type& c) : FrontIt(c) {}
    FrontIt::container_type* get_container() const
    {
        return containerPtr;
    }
};

typedef insert_iterator<Cont> InsIt;
class MyinsIt : public InsIt
{
public:
    MyinsIt(InsIt::container_type& c, Cont::iterator it) : InsIt(c, it) {}
    InsIt::container_type* get_container() const { return (containerPtr); }
    Cont::iterator get_iterator() const { return (iter); }
};


void test_inserts()
{
    Cont c0;
    char ch;
    BackIt::container_type* pbi_cont = (Cont*)0;
    BackIt::reference pbi_ref = ch;
    BackIt::value_type* pbi_val = (char*)0;
    BackIt bit(c0);
    *bit = 'a', ++bit;
    *bit++ = 'b';
    assert(c0[0] == 'a' && c0[1] == 'b');
    MyBackIt mybkit(c0);
    assert(mybkit.get_container() == &c0);
    *(learnSTL::back_inserter(c0))++ = 'x';
    assert(c0[2] == 'x');

    FrontIt::container_type* pfi_cont = (Cont*)0;
    FrontIt::reference pfi_ref = ch;
    FrontIt::value_type *pfi_val = (char*)0;
    FrontIt fit(c0);
    *fit = 'c', ++fit;
    *fit++ = 'd';
    assert(c0[0] = 'd' && c0[1] == 'c');
    MyfrontIt myfrit(c0);
    assert(myfrit.get_container() == &c0);
    *(learnSTL::front_inserter(c0))++ = 'y';
    assert(c0[0] == 'y');

    InsIt::container_type* pii_cont = (Cont*)0;
    InsIt::reference pii_ref = ch;
    InsIt::value_type* pii_val = (char*)0;
    InsIt iit(c0, c0.begin());
    *iit = 'e', ++iit;
    *iit++ = 'f';
    assert(c0[0] == 'e' && c0[1] == 'f');
    MyinsIt myinsit(c0, c0.begin());
    assert(myinsit.get_container() == &c0);
    assert(myinsit.get_iterator() == c0.begin());
    *(learnSTL::inserter(c0, c0.begin()))++ = 'z';
    assert(c0[0] == 'z');
}

// TEST istream_iterator
void test_istreamit()
{
    std::istrstream istr("0 1 2 3");
    typedef istream_iterator<int, char, std::char_traits<char>, ptrdiff_t> IstrIt;
    IstrIt::char_type* p_char = (char*)0;
    IstrIt::traits_type* p_traits = (std::char_traits<char>*)0;
    IstrIt::istream_type* p_istream = (std::istream*)0;
    IstrIt iit0, iit(istr);
    for (int i = 0; i < 5 && iit != iit0; ++i) {
        assert(*iit++ == i);
        assert(i == 4);
    } 
}

// TEST ostream_iterator
void test_ostreamit()
{
    std::ostrstream ostr0, ostr;
    typedef ostream_iterator<int, char, std::char_traits<char> > OstrIt;
    OstrIt::value_type* p_val = (int*)0;
    OstrIt::char_type* p_char = (char*)0;
    OstrIt::traits_type* p_traits = (std::char_traits<char>*)0;
    OstrIt::ostream_type* p_ostream = (std::basic_ostream<char, std::char_traits<char> >*)0;
    OstrIt oit0(ostr0), oit(ostr, "||");
    *oit0 = 1, ++oit0;
    *oit0++ = 2;
    ostr0 << std::ends;
    assert(strcmp(ostr0.str(), "12") == 0);
    ostr.freeze(false);

    *oit = 1, ++oit;
    *oit++ = 2;
    ostr << std::ends;
    assert(strcmp(ostr.str(), "1||2||") == 0);
    ostr.freeze(false);
}

/*
int main(int argc, char *argv[])
{
    test_prop();
    test_revit();
    std::cout << "SUCCESS testing <iterator>" << std::endl;
    return 0;
}
*/
