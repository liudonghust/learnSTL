#ifndef MY_ALGORITHM_H
#define MY_ALGORITHM_H

#include <cstring>
#include <type_traits>
#include <initializer_list>
#include "utility.h"
#include "iterator.h"

namespace learnSTL{

    ///: --------------------- range compare ------------------------
	// equal
	template<typename T1, typename T2 = T1>
	struct __equal_to{
		bool operator()(const T1& x, const T1& y) const { return x == y; }
		bool operator()(const T1& x, const T2& y) const { return x == y; }
		bool operator()(const T2& x, const T2& y) const { return x == y; }
		bool operator()(const T2& x, const T1& y) const { return x == y; }
	};

	template<typename T>
	struct __equal_to < T, T > {
		bool operator()(const T& x, const T& y) const { return x == y; }
	};

	template<typename T>
	struct __equal_to <const T, T > {
		bool operator()(const T& x, const T& y) const { return x == y; }
	};

	template<typename T>
	struct __equal_to < T, const T > {
		bool operator()(const T& x, const T& y) const { return x == y; }
	};

	template<typename InputIter1, typename InputIter2, typename BinPred> inline
		bool equal(InputIter1 first1, InputIter2 last1, InputIter2 first2, BinPred pred)
	{
		for (; first1 != last1; ++first1, ++first2)
			if (!pred(*first1, *first2))
				return false;
		return true;
	}
	template<typename InputIter1, typename InputIter2> inline
		bool equal(InputIter1 first1, InputIter1 last1, InputIter2 first2)
	{
		typedef typename iterator_traits<InputIter1>::value_type v1;
		typedef typename iterator_traits<InputIter2>::value_type v2;
		return equal(first1, last1, first2, __equal_to<v1, v2>());
	}

	// lexicographical_compare
	template<typename T1, typename T2 = T1>
	struct __less{
		bool operator()(const T1& x, const T1& y) const { return x < y; }
		bool operator()(const T1& x, const T2& y) const { return x < y; }
		bool operator()(const T2& x, const T2& y) const { return x < y; }
		bool operator()(const T2& x, const T1& y) const { return x < y; }
	};
	template<typename T>
	struct __less < T, T > {
		bool operator()(const T& x, const T& y) const { return x < y; }
	};
	template<typename T>
	struct __less < const T, T > {
		bool operator()(const T& x, const T& y) const { return x < y; }
	};
	template<typename T>
	struct __less < T, const T > {
		bool operator()(const T& x, const T& y) const { return x < y; }
	};

	template<typename Compare, typename InputIter1, typename InputIter2> inline
		bool __lexicographical_compare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, Compare comp)
	{
		for (; first2 != last2; ++first1, ++first2){
			if (first1 == last1 || comp(*first1, *first2))
				return true;
			if (comp(*first2, *first1))
				return false;
		}
		return false;
	}

	template<typename InputIter1, typename InputIter2, typename Compare> inline
		bool lexicographical_compare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, Compare comp)
	{
		typedef typename std::add_lvalue_reference<Compare>::type Comp_ref;
		return learnSTL::__lexicographical_compare<Comp_ref>(first1, last1, first2, last2, comp);
	}
	template<typename InputIter1, typename InputIter2> inline
		bool lexicographical_compare(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2)
	{
		typedef typename iterator_traits<InputIter1>::value_type v1;
		typedef typename iterator_traits<InputIter2>::value_type v2;
		return lexicographical_compare(first1, last1, first2, last2, __less<v1, v2>());
	}
	
    // mismatch 
    template<typename InputIter1, typename InputIter2, typename BinPred>
    std::pair<InputIter1, InputIter2> mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, BinPred pred)
    {
        for(; first1 != last1; ++first1, ++first2){
            if(!(pred(*first1, *first2)))
                break;
        }
        return std::make_pair(first1, first2);
    }
    

    template<typename InputIter1, typename InputIter2>
    std::pair<InputIter1, InputIter2> mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2)
    {
        typedef typename iterator_traits<InputIter1>::value_type v1;
        typedef typename iterator_traits<InputIter2>::value_type v2;
        return mismatch(first1, last1, first2, __equal_to<v1, v2>());
    }

    ///~ ------------------ end range compare -----------------------------
    
    ///: ----------------- copy move and assignment -----------------------
	// copy
	template<typename InputIter, typename OutputIter> inline
		OutputIter copy(InputIter first, InputIter last, OutputIter result)
	{
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}
	// specialize for pointer
	template<typename In, typename Out> inline
		typename std::enable_if<std::is_same<typename std::remove_const<In>, Out>::value &&
								std::is_trivially_copy_assignable<Out>::value,
								Out*>::type
		copy(In* first, In* last, Out* result)
	{
		const size_t n = static_cast<size_t>(last - first);
		if (n > 0)
			std::memmove(result, first, n*sizeof(Out));
		return result + n;
	}

    //copy_if
    template<typename InputIter, typename OutputIter, typename Predicate>
    OutputIter copy_if(InputIter first, InputIter last, OutputIter result, Predicate pred)
    {
        for(; first != last; ++first){
            if(pred(*first)){
                *result = *first;
                ++result;
            }
        }
        return result;
    }
    
    //copy_n
    template<typename InputIter, typename OutputIter>
    OutputIter copy_n(InputIter first, size_t n, OutputIter result)
    {
        for (size_t i = 0; i < n; ++i) {
            *result = *first;
            ++first;
            ++result;
        }
        return result;
    }

    template<typename In, typename Out> inline
        typename std::enable_if<std::is_same<typename std::remove_const<In>::type, Out>::value && 
                                std::is_trivially_copy_assignable<Out>::value,
                                Out*>::type
    copy_n(In* first, size_t n, Out* result) 
    {
        if(n > 0) {
            std::memmove(result, first, n*sizeof(Out));
        }
        return result + n;
    }

	// copy_backward
	template<typename BiIter, typename OutputIter> inline
		OutputIter copy_backward(BiIter first, BiIter last, OutputIter result)
	{
		while (first != last)
			*(--result) = *(--last);
		return result;
	}

	template<typename In, typename Out> inline
        typename std::enable_if<std::is_same<typename std::remove_const<In>, Out>::value && 
                                std::is_trivially_copy_assignable<Out>::value,
                                Out*>::type
		copy_backward(In* first, In* last, Out* result)
	{
		const size_t n = static_cast<size_t>(last - first);
		if (n > 0){
			result -= n;
			std::memmove(result, first, n * sizeof(Out));
		}
		return result;
	}


	// move
	template<typename InputIter, typename OutputIter> inline
		OutputIter move(InputIter first, InputIter last, OutputIter result)
	{
		for (; first != last; ++first, ++result)
			*result = std::move(*first);
		return result;
	}

	template<typename T, typename U> inline
		typename std::enable_if<std::is_same<typename std::remove_const<T>::type, U>::value &&
								std::is_trivially_copy_assignable<U>::value,
								U*>::type
		move(T* first, T* last, U* result)
	{
		const size_t n = static_cast<size_t>(last - first);
		if (n > 0)
			std::memmove(result, first, n * sizeof(U));
		return result + n;
	}

	// move_backward
	template<typename BiIter1, typename BiIter2> inline
		BiIter2 __move_backward(BiIter1 first1, BiIter1 last1, BiIter2 result)
	{
		while (first1 != last1){
			*(--result) = std::move(*(--last1));
		}
		return result;
	}
	// specialize for pointer
	template<typename T1, typename T2> inline
		typename std::enable_if<std::is_same<typename std::remove_const<T1>::type, T2>::value &&
											std::is_trivially_copy_assignable<T2>::value,
											T2*>::type
		__move_backward(T1* first1, T1* last1, T2* result)
	{
		const size_t n = static_cast<size_t>(last1 - first1);
		if (n > 0){
			result -= n;
			std::memmove(result, first1, n*sizeof(T2));
		}
		return result;
	}

	template<typename BiIter1, typename BiIter2> inline
		BiIter2 move_backward(BiIter1 first1, BiIter1 last1, BiIter2 result)
	{
		return __move_backward(first1, last1, result);
	}

	// fill_n 
	template<typename OutputIter, typename Size, typename T> inline
		OutputIter __fill_n(OutputIter first, Size n, const T& val)
	{
		for (; n > 0; ++first, --n)
			*first = val;
		return first;
	}
	// for char*(iter) and char(val)
	template<typename T, typename Size, typename U> inline
		typename std::enable_if
		<
		std::is_integral<T>::value && sizeof(T) == 1 &&
		!std::is_same<T, bool>::value &&
		std::is_integral<U>::value && sizeof(U) == 1,
		T*
		>::type
		__fill_n(T* first, Size n, const U& value)
	{
		if (n > 0)
			std::memset(first, (unsigned char)value, (size_t)n);
		return first + n;
	}

	template<typename OutputIter, typename Size, typename T> inline
		OutputIter fill_n(OutputIter first, Size n, const T& val)
	{
		return __fill_n(first, n, val);
	}

	//fill
	template<typename ForwardIter, typename T> inline
		void __fill(ForwardIter first, ForwardIter last, const T& val, forward_iterator_tag)
	{
		for (; first != last; ++first)
			*first = val;
	}
	template<typename RandomIter, typename T> inline
		void __fill(RandomIter first, RandomIter last, const T& val, random_access_iterator_tag)
	{
		fill_n(first, last - first, val);
	}
	template<typename ForwardIter, typename T> inline
		void fill(ForwardIter first, ForwardIter last, const T& val)
	{
		__fill(first, last, val, typename iterator_traits<ForwardIter>::iterator_category());
	}

	//generate
	template<typename ForwardIter, typename Generator> inline
		void generate(ForwardIter first, ForwardIter last, Generator gen)
	{
		for (; first != last; ++first)
			*first = gen();
	}

	//generate_n
	template<typename OutputIter, typename Size, typename Generator> inline
		OutputIter generate_n(OutputIter first, Size n, Generator gen)
	{
		typedef decltype(std::__convert_to_integral(n)) Intergral;
		Intergral new_n = n;
		for (; new_n > 0; ++first, --new_n)
			*first = gen();
		return first;
	}
	///~ ------------------------ end copy move and assignment -----------------
	

	///: ------------------------ replace --------------------------------------
	// replace
	template<typename ForwardIter, typename T> inline
		void replace(ForwardIter first, ForwardIter last, const T& oldVal, const T& newVal)
	{
		for (; first != last; ++first){
			if (*first == oldVal)
				*first == newVal;
		}
	}

	// replace_copy
	template<typename ForwardIter, typename OutputIter, typename T> inline
		OutputIter replace_copy(ForwardIter first, ForwardIter last, OutputIter result, const T& oldVal, const T& newVal)
	{
		for (; first != last; ++first, ++result){
			if (*first == oldVal)
				*result = newVal;
			else
				*result = *first;
		}
		return result;
	}

	// replace_if
	template<typename ForwardIter, typename UnaryPred, typename T> inline
		void replace_if(ForwardIter first, ForwardIter last, UnaryPred pred, const T& newVal)
	{
		for (; first != last; ++first){
			if (pred(*first))
				*first = newVal;
		}
	}

	// replace_copy_if
	template<typename ForwardIter, typename OutputIter, typename UnaryPred, typename T> inline
		OutputIter replace_copy_if(ForwardIter first, ForwardIter last, OutputIter result, UnaryPred pred, const T& newVal)
	{
		for (; first != last; ++first, ++result){
			if (pred(*first))
				*result = newVal;
			else
				*result = *first;
		}
		return result;
	}
	///~ -------------------------- end replace -------------------------------------------


	///: -------------------------- swap ------------------------------------------
	// swap_ranges
	template<typename ForwardIter1, typename ForwardIter2> inline
		ForwardIter2 swap_ranges(ForwardIter1 first, ForwardIter1 last, ForwardIter2 result)
	{
		for (; first != last; ++first, ++result)
			swap(*first, *result);
		return result;
	}

	///~ ------------------------- end swap ------------------------------------


	///: -------------------------- transform ----------------------------------
	template<typename ForwardIter, typename OutputIter, typename UnaryOP> inline
		OutputIter transform(ForwardIter first, ForwardIter last, OutputIter result, UnaryOP op)
	{
		for (; first != last; ++first, ++result)
			*result = op(*first);
		return result;
	}
	template<typename ForwardIter1, typename ForwardIter2, typename OutputIter, typename BinOP> inline
		OutputIter transform(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, OutputIter result, BinOP op)
	{
		for (; first1 != last1; ++first1, ++first2, ++result)
			*result = op(*first1, *first2);
		return result;
	}
	///~ --------------------- end transform -----------------------------------

	// for_each
	template<typename InputIter, typename UnaryOP> inline
		UnaryOP for_each(InputIter first, InputIter last, UnaryOP op)
	{
		for (; first != last; ++first)
			op(*first);
		return std::move(op);
	}


	///: ---------------------- find not sorted -----------------------------
	// count
	template<typename InputIter, typename T> inline
		typename iterator_traits<InputIter>::difference_type
		count(InputIter first, InputIter last, const T& val)
	{
		typename iterator_traits<InputIter>::difference_type cn(0);
		for (; first != last; ++first)
			if (*first == val)
				++cn;
		return cn;
	}

	// count_if
	template<typename InputIter, typename UnaryPred> inline
		typename iterator_traits<InputIter>::difference_type
		count_if(InputIter first, InputIter last, UnaryPred pred)
	{
		typename iterator_traits<InputIter>::difference_type cn(0);
		for (; first != last; ++first)
			if (pred(*first))
				++cn;
		return cn;
	}

    //min_element
    template<typename ForwardIter, typename Compare> inline
        ForwardIter min_element(ForwardIter first, ForwardIter last, Compare comp)
        {
            if(first != last){
                ForwardIter next = first;
                while(++next != last){
                    if(comp(*next, *first))
                        first = next;
                }
            }
            return first;
        }

    template<typename ForwardIter> inline
        ForwardIter min_element(ForwardIter first, ForwardIter last)
        {
            typedef typename iterator_traits<ForwardIter>::value_type v;
            return min_element(first, last, __less<v>());
        }
    
    // min
    template<typename T, typename Compare> inline
        const T& min(const T& a, const T& b, Compare comp)
        {
            return comp(a, b) ? a : b;
        }
    template<typename T> inline
        const T& min(const T& a, const T& b)
        {
            return min(a, b, __less<T>());
        }

    template<typename T, typename Compare> inline
        T min(std::initializer_list<T> initlist, Compare comp)
        {
            return *(min_element(initlist.begin(), initlist.end(), comp));
        }

    template<typename T> inline
        T min(std::initializer_list<T> initlist)
        {
            return *(min_element(initlist.begin(), initlist.end(), __less<T>()));
        }

    // max_element
    template<typename ForwardIter, typename Compare> inline
        ForwardIter max_element(ForwardIter first, ForwardIter last, Compare comp)
        {
            if(first != last){
                ForwardIter next = first;
                while(++next != last){
                    if(comp(*first, *next))
                        first = next;
                }
            }
            return first;
        }
    template<typename ForwardIter> inline
        ForwardIter max_element(ForwardIter first, ForwardIter last)
        {
            typedef typename iterator_traits<ForwardIter>::value_type v;
            return max_element(first, last, __less<v>());
        }

    // max
    template<typename T, typename Compare> inline
        const T& max(const T& a, const T& b, Compare comp)
        {
            return comp(a, b) ? b : a;
        }
    template<typename T> inline
        const T& max(const T& a, const T& b)
        {
            return max(a, b, __less<T>());
        }
    
    template<typename T, typename Compare> inline
        T max(std::initializer_list<T> initlist, Compare comp)
        {
            return *(max_element(initlist.begin(), initlist.end(), comp));
        }

    template<typename T> inline
        T max(std::initializer_list<T> initlist)
        {
            return *(max_element(initlist.begin(), initlist.end(), __less<T>()));
        }

    //minmax_element
    template<typename ForwardIter, typename Compare> inline
        std::pair<ForwardIter, ForwardIter>
        minmax_element(ForwardIter first, ForwardIter last, Compare comp)
        {
            std::pair<ForwardIter, ForwardIter> result(first, first);
            if(first != last){
                if (++first != last) {
                    if (comp(*first, *(result.first))) {
                        result.first = first;
                    }
                    else {
                        result.second = first;
                    }
                    while (++first != last){
                        ForwardIter pre = first;
                        if (++first != last) {
                            if (comp(*pre, *first)) {
                                if (comp(*pre, *(result.first))) {
                                    result.first = pre;
                                }
                                if(!(comp(*first, *(result.second)))){
                                    result.second = first;
                                }
                            }
                            else {
                                if(comp(*first, *(result.first)))
                                    result.first = first;
                                if(!comp(*pre, *(result.second)))
                                    result.second = pre;
                            }    
                        }
                        else {
                            if(comp(*pre, *(result.first)))
                                result.first = pre;
                            else if(!comp(*pre, *(result.second)))
                                result.second = pre;
                            break;
                        }
                    }
                }
            }
            return result;
        }

    template<typename ForwardIter> inline
        std::pair<ForwardIter, ForwardIter>
        minmax_element(ForwardIter first, ForwardIter last)
        {
            typedef typename iterator_traits<ForwardIter>::value_type v;
            return minmax_element(first, last, __less<v>());
        }

    // minmax
    template<typename T, typename Compare> inline
        std::pair<const T&, const T&>
        minmax(const T& a, const T& b, Compare comp)
        {
            return comp(a, b) ? std::pair<const T&, const T&>(a, b) : 
                                std::pair<const T&, const T&>(b, a);
        }
    template<typename T> inline
        std::pair<const T&, const T&>
        minmax(const T& a, const T& b)
        {
            return minmax(a, b, __less<T>());
        }
    
    template<typename T, typename Compare> inline
        std::pair<T, T> minmax(std::initializer_list<T> initlist, Compare comp)
        {
            typedef typename std::initializer_list<T>::const_iterator Iter;
            Iter first = initlist.cbegin();
            Iter last = initlist.cend();
            std::pair<T, T> result(*first, *first);
            if(first != last){
                if(++first != last){
                    if(comp(*first, *(result.first)))
                        result.first = *first;
                    else
                        result.second = *first;
                }
                while(++first != last){
                    Iter pre = first;
                    if (++first != last) {
                        if (comp(*pre, *first)) {
                            if(comp(*pre, *(result.first)))
                                result.first = *pre;
                            if(!comp(*first, *(result.second)))
                                result.second = *first;
                        }
                        else {
                            if(comp(*first, *(result.first)))
                                result.first = *first;
                            if(!comp(*pre, *(result.second)))
                                result.second = *pre;
                        }
                    }
                    else {
                        if(comp(*pre, *(result.first)))
                            result.first = *pre;
                        else if(!comp(*pre, *(result.second)))
                            result.second = *pre;
                        break;
                    }
                }
            }
            return result;
        }

    template<typename T> inline
        std::pair<T, T> minmax(std::initializer_list<T> initlist)
        {
            return minmax(initlist, __less<T>());
        }

    // find 
    template<typename InputIter, typename T> inline
        InputIter find(InputIter first, InputIter last, const T& val)
        {
            for(; first != last; ++first)
                if(*first == val)
                    break;
            return first;
        }

    // find_if
    template<typename InputIter, typename UnaryPred> inline
        InputIter find_if(InputIter first, InputIter last, UnaryPred pred)
        {
            for(; first != last; ++first)
                if(pred(*first))
                    break;
            return first;
        }

    // find_if_not
    template<typename InputIter, typename UnaryPred> inline
        InputIter find_if_not(InputIter first, InputIter last, UnaryPred pred)
        {
            for(; first != last; ++first)
                if(!pred(*first))
                    break;
            return first;
        }

    
	//adjacent_find
	template<typename ForwardIter, typename BinPred> inline
		ForwardIter adjacent_find(ForwardIter first, ForwardIter last, BinPred pred)
	{
		if (first != last){
			ForwardIter next = first;
			while (++next != last){
				if (pred(*first, *next))
					return first;
				first = next;
			}
		}
		return last;
	}
	//adjacent_find
	template<typename ForwardIter> inline
		ForwardIter adjacent_find(ForwardIter first, ForwardIter last)
	{
		typedef typename iterator_traits<ForwardIter>::value_type v;
		return adjacent_find(first, last, __equal_to<v>());
	}
    
    ///~ --------------------- end of finding non-sorted ----------------------------
    

    ///: --------------------- finding sorted -------------------------------------
    // lower_bound
    template<typename ForwardIter, typename T, typename Compare> inline
        ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& val, Compare comp)
        {
            typedef typename iterator_traits<ForwardIter>::difference_type difference_type;
            difference_type len = distance(first, last);
            while(len != 0){   // len op is cheaper than distance.  so not while(first != last)
                difference_type len_half = len / 2;
                ForwardIter mid = first;
                advance(mid, len_half);
                if(comp(*mid, val)){
                    first = ++mid;
                    len -= len_half + 1;
                }
                else
                    len = len_half;
            }
            return first;
        }

    template<typename ForwardIter, typename T> inline
        ForwardIter lower_bound(ForwardIter first, ForwardIter last, const T& val)
        {
            return lower_bound(first, last, val, __less<typename iterator_traits<ForwardIter>::value_type, T>());
        }

    // upper_bound
    template<typename ForwardIter, typename T, typename Compare> inline
        ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& val, Compare comp)
        {
            typedef typename iterator_traits<ForwardIter>::difference_type difference_type;
            difference_type len = distance(first, last);
            while(len != 0){
                difference_type len_half = len / 2;
                ForwardIter mid = first;
                advance(mid, len_half);
                if(!comp(val, *mid)){
                    first = ++mid;
                    len -= len_half + 1;
                }
                else
                    len = len_half;
            }
            return first;
        }

    template<typename ForwardIter, typename T> inline
        ForwardIter upper_bound(ForwardIter first, ForwardIter last, const T& val)
        {
            return upper_bound(first, last, val, __less<T, typename iterator_traits<ForwardIter>::value_type>());
        }

    // equal_range
    template<typename ForwardIter, typename T, typename Compare> inline
        std::pair<ForwardIter, ForwardIter>
        equal_range(ForwardIter first, ForwardIter last, const T& val, Compare comp)
        {
            typedef typename iterator_traits<ForwardIter>::difference_type difference_type;
            difference_type len = distance(first, last);
            while(len != 0){
                difference_type len_half = len / 2;
                ForwardIter mid = first;
                advance(mid, len_half);
                if(comp(*mid, val)){
                    first = ++mid;
                    len -= len_half + 1;
                }
                else if(comp(val, *mid)){
                    last = mid;
                    len = len_half;
                }
                else{
                    ForwardIter mid_next = mid;
                    return std::pair<ForwardIter, ForwardIter>
                        (lower_bound(first, mid, val, comp), upper_bound(++mid_next, last, val, comp));
                }
            }
            return std::pair<ForwardIter, ForwardIter>(first, first);
        }

    template<typename ForwardIter, typename T> inline
        std::pair<ForwardIter, ForwardIter>
        equal_range(ForwardIter first, ForwardIter last, const T& val)
        {
            return equal_range(first, last, val, __less<typename iterator_traits<ForwardIter>::value_type, T>());
        }

    // binary_search
    template<typename ForwardIter, typename T, typename Compare> inline
        bool binary_search(ForwardIter first, ForwardIter last, const T& val, Compare comp)
        {
            first = lower_bound(first, last, val, comp);
            return first != last && !comp(val, *first);
        }

    template<typename ForwardIter, typename T> inline
        bool binary_search(ForwardIter first, ForwardIter last, const T& val)
        {
            return binary_search(first, last, val, __less<typename iterator_traits<ForwardIter>::value_type, T>());
        }

    // includes
    template<typename InputIter1, typename InputIter2, typename Compare> inline
        bool includes(InputIter1 first, InputIter1 last, InputIter2 searchFirst, InputIter2 searchLast, Compare comp)
        {
            for(; searchFirst != searchLast; ++first){
                if(first == last || comp(*searchFirst, *first))
                    return false;
                if(!comp(*first, *searchFirst))
                    ++searchFirst;
            }
            return true;
        }

     template<typename InputIter1, typename InputIter2> inline
        bool includes(InputIter1 first, InputIter1 last, InputIter2 searchFirst, InputIter2 searchLast)
        {
            typedef typename iterator_traits<InputIter1>::value_type v1;
            typedef typename iterator_traits<InputIter2>::value_type v2;
            return includes(first, last, searchFirst, searchLast, __less<v1, v2>());
        }
    
    ///~ ----------------------------- end finding sorted -----------------------------------------


 	///: ------------------- remove ---------------------------------------------
	// remove
	template<typename ForwardIter, typename T> inline
		ForwardIter remove(ForwardIter first, ForwardIter last, const T& val)
	{
		while (first != last && *first != val)
			++first;
		if (first != last){
			ForwardIter next = first;
			while (++next != last){
				if (*next != val){
					*first = std::move(*next);
					++first;
				}
			}
		}
		return first;
	}
	// remove_copy
	template<typename ForwardIter, typename OutputIter, typename T> inline
		OutputIter remove_copy(ForwardIter first, ForwardIter last, OutputIter result, const T& val)
	{
	    for(; first != last; ++first){
			if (*first != val){
				*result = *first;
				++result;
			}
		}
		return result;
	}
	// remove_if
	template<typename ForwardIter, typename UnaryPred> inline
		ForwardIter remove_if(ForwardIter first, ForwardIter last, UnaryPred pred)
	{
		while (first != last && !pred(*first))
			++first;
		if (first != last){
			ForwardIter next = first;
			while (++next != last){
				if (!pred(*next)){
					*first = std::move(*next);
					++first;
				}
			}
		}
		return first;
	}
	// remove_copy_if
	template<typename ForwardIter, typename OutputIter, typename UnaryPred> inline
		OutputIter remove_copy_if(ForwardIter first, ForwardIter last, OutputIter result, UnaryPred pred)
	{
		for (; first != last; ++first){
			if (!(pred(*first))){
				*result = *first;
				++result;
			}
		}
		return result;
	}

	//unique
	template<typename ForwardIter, typename BinPred> inline
		ForwardIter unique(ForwardIter first, ForwardIter last, BinPred binaryPred)
	{
		first = adjacent_find<ForwardIter, typename std::add_lvalue_reference<BinPred>::type>
			(first, last, binaryPred);
		if (first != last){
			ForwardIter next = first;
			for (++next; ++next != last;){
				if (!binaryPred(*first, *next))
					*(++first) = std::move(*next);
			}
			return ++first;
		}
		return last;
	}
	template<typename ForwardIter> inline
		ForwardIter unique(ForwardIter first, ForwardIter last)
	{
		typedef typename iterator_traits<ForwardIter>::value_type v;
		return unique(first, last, __equal_to<v>());
	}
	//unique_copy
	template<typename BinPred, typename ForwardIter, typename OutputIter> inline
		OutputIter __unique_copy(ForwardIter first, ForwardIter last, OutputIter result, BinPred pred, forward_iterator_tag)
	{
		if (first != last){
			*result = *first;
			ForwardIter next = first;
			while (++next != last){
				if (!(pred(*first, *next))){
					*(++result) = *next;
					first = next;
				}
			}
			++result;
		}
		return result;
	}
	template<typename BinPred, typename InputIter, typename OutputIter> inline
		OutputIter __unique_copy(InputIter first, InputIter last, OutputIter result, BinPred pred, input_iterator_tag)
	{
		if (first != last){
			typename iterator_traits<InputIter>::value_type firstVal(*first);
			*result = firstVal;
			++result;
			while (++first != last){
				typename iterator_traits<InputIter>::value_type secondVal(*first);
				if (!(pred(firstVal, secondVal))){
					*result = secondVal;
					++result;
					firstVal = secondVal;
				}
			}
		}
		return result;
	}
	
	template<typename InputIter, typename OutputIter, typename BinPred> inline
		OutputIter unique_copy(InputIter first, InputIter last, OutputIter result, BinPred pred)
	{
		return __unique_copy<typename std::add_lvalue_reference<BinPred>::type>
			(first, last, result, pred, iterator_traits<InputIter>::iterator_category());
	}

	template<typename InputIter, typename OutputIter> inline
		OutputIter unique_copy(InputIter first, InputIter last, OutputIter result)
	{
		typedef typename iterator_traits<InputIter>::value_type v;
		return unique_copy(first, last, result, __equal_to<v>());
	}


    ///~ ---------------------- end of removing ------------------------------------
    
    
    ///~ -------------------- pattern matching --------------------------------------
    //search
    template<typename BinPred, typename ForwardIter1, typename ForwardIter2> inline
        ForwardIter1 __search(ForwardIter1 first, ForwardIter1 last, ForwardIter2 searchFirst, ForwardIter2 searchLast, BinPred pred,
                forward_iterator_tag, forward_iterator_tag)
        {
            if(searchFirst == searchLast)
                return first;
            while(true){
                // find first element equal to *searchFirst
                while(true){
                    if(first == last)   // no element match searchFirst
                        return last;
                    if(pred(*first, *searchFirst))
                        break;
                    ++first;
                }
                // then match the left elements
                ForwardIter1 pos = first;
                ForwardIter2 searchPos = searchFirst;
                while(true){
                    if(++searchPos == searchLast)   // all match
                        return first;
                    if(++pos == last)  // source exhaused, not match
                        return last;
                    if(!pred(*pos, *searchPos)){  // if the left is not matched, continue the loop
                        ++first;
                        break;
                    }
                }
            }
        }

    template<typename BinPred, typename RandomIter1, typename RandomIter2> inline
        RandomIter1 __search(RandomIter1 first, RandomIter1 last, RandomIter2 searchFirst, RandomIter2 searchLast, BinPred pred,
                random_access_iterator_tag, random_access_iterator_tag)
        {
            typedef typename iterator_traits<RandomIter1>::difference_type DType1;
            typedef typename iterator_traits<RandomIter2>::difference_type DType2;

            DType1 len1 = last - first;
            DType2 len2 = searchLast - searchFirst;
            if(len2 == 0)
               return first;
            if(len1 < len2)
               return last;
            const RandomIter1 s = last - (len2 - 1);    // start of pattern match can't go beyond here
            while(true){
                while(true){
                    if(first == s)    // no match
                       return last;
                    if(pred(*first, *searchFirst))
                        break;
                    ++first;
                }
                RandomIter1 pos = first;
                RandomIter2 searchPos = searchFirst;
                while(true){
                    if(++searchPos == searchLast)   // find
                        return first;
                    ++pos;     // no need to check range on pos, because s guarantees we have enough source
                    if(!pred(*pos, *searchPos)){
                        ++first;
                        break;
                    }
                }
            } 
        }
        
    template<typename ForwardIter1, typename ForwardIter2, typename BinPred> inline
        ForwardIter1 search(ForwardIter1 first, ForwardIter1 last, ForwardIter2 searchFirst, ForwardIter2 searchLast, BinPred pred)
        {
            return __search<typename std::add_lvalue_reference<BinPred>::type>
                (first, last, searchFirst, searchLast, pred, 
                 typename iterator_traits<ForwardIter1>::iterator_category(),
                 typename iterator_traits<ForwardIter2>::iterator_category());
        }

    template<typename ForwardIter1, typename ForwardIter2> inline
        ForwardIter1 search(ForwardIter1 first, ForwardIter1 last, ForwardIter2 searchFirst, ForwardIter2 searchLast)
        {
            typedef typename iterator_traits<ForwardIter1>::value_type v1;
            typedef typename iterator_traits<ForwardIter2>::value_type v2;
            return search(first, last, searchFirst, searchLast, __equal_to<v1, v2>());
        }

    // search_n
    template<typename BinPred, typename ForwardIter, typename Size, typename T> inline
        ForwardIter __search_n(ForwardIter first, ForwardIter last, Size n, const T& val, BinPred pred,
                forward_iterator_tag)
        {
            typedef decltype(std::__convert_to_integral(n)) Integral;
            Integral sz(n);
            if(sz <= 0)
                return first;
            while(true){
                // find first element equal to val
                while(true){
                    if (first == last) {
                        return last;
                    }
                    if(pred(*first, val))
                        break;
                    ++first;
                }
                ForwardIter pos = first;
                Integral count(0);
                while(true){
                    if(++count == sz)   // found!
                        return first;
                    if(++pos == last)   // not match
                        return last;
                    if(!pred(*pos, val)){
                        first = pos;
                        ++first;
                        break;
                    }
                }
            }
        }

    template<typename BinPred, typename RandomIter, typename Size, typename T> inline
        RandomIter __search_n(RandomIter first, RandomIter last, Size n, const T& val, BinPred pred,
                random_access_iterator_tag)
        {
            typedef decltype(std::__convert_to_integral(n)) Integral;
            Integral sz(n);
            if(sz <= 0)
                return last;
            Integral len = static_cast<Integral>(last - first);
            if(len < sz)
                return last;
            const RandomIter s = last - (sz - 1);
            while(true){
                while(true){
                    if(first == s)
                        return last;
                    if(pred(*first, val))
                        break;
                    ++first;
                }
                RandomIter pos = first;
                Integral count(0);
                while(true){
                    if(++count == sz)
                        return first;
                    ++pos;
                    if(!pred(*pos, val)){
                        first = pos;
                        ++first;
                        break;
                    }
                }
            }
        }

    template<typename ForwardIter, typename Size, typename T, typename BinPred> inline
        ForwardIter search_n(ForwardIter first, ForwardIter last, Size n, const T& val, BinPred pred)
        {
            return __search_n<std::add_lvalue_reference<BinPred>::type>
                (first, last, n, val, pred, typename iterator_traits<ForwardIter>::iterator_category());
        }

    template<typename ForwardIter, typename Size, typename T> inline
        ForwardIter search_n(ForwardIter first, ForwardIter last, Size n, const T& val)
        {
            typedef typename iterator_traits<ForwardIter>::value_type v;
            return search_n(first, last, n, val, __equal_to<v, T>());
        }

    // find_end
    template<typename BinPred, typename ForwardIter1, typename ForwardIter2> inline
        ForwardIter1 __find_end(ForwardIter1 first, ForwardIter1 last, ForwardIter2 searchFirst, ForwardIter2 searchLast, BinPred pred,
                forward_iterator_tag, forward_iterator_tag)
        {
            ForwardIter1 ret = last;
            if(searchFirst == searchLast)
                return ret;
            while(true){
                // find first element equal to searchFirst
                while (true){
                    if(first == last)   // not found
                        return ret;
                    if(pred(*first, *searchFirst))
                        break;
                    ++first;
                }
                ForwardIter1 pos = first;
                ForwardIter2 searchPos = searchFirst;
                while(true){
                    if (++searchPos == searchLast) { 
                        // record current subSequence match the pattern.
                        // and continue loop th find if there is another subSequence
                        // left matching the pattern.
                        ret = first;
                        ++first;
                        break;
                    }
                    if(++pos == last){  // all searched, return last matching subSequence or no match.
                        return ret;
                    }
                    if(!pred(*pos, *searchPos)){
                        ++first;
                        break;
                    }
                }
            }
        }
    
    template<typename BinPred, typename BiIter1, typename BiIter2> inline
        BiIter1 __find_end(BiIter1 first, BiIter1 last, BiIter2 searchFirst, BiIter2 searchLast, BinPred pred,
                bidirectional_iterator_tag, bidirectional_iterator_tag)
        {
            if(searchFirst == searchLast)
                return last;
            BiIter1 new_last = last;
            --new_last;
            BiIter2 new_search_last = searchLast;
            --new_search_last;
            while(true){
                // backward. find first element equal to searchLast.
                while(true){
                    if(new_last == first) // no matched
                        return last;
                    if(pred(*new_last, *new_search_last))
                        break;
                    --new_last;
                }
                BiIter1 pos = new_last;
                BiIter2 searchPos = new_search_last;
                while(true){
                    if(searchPos == searchFirst)
                        return pos;
                    if(pos == first)  // no matched
                        return last;
                    if(!pred(*--pos, *--searchPos)){  // if there is a mismatch, restart with a new last.
                        break;
                        --new_last;
                    }
                    // else there is a match, check next elements
                }
            }
        }

    template<typename BinPred, typename RandomIter1, typename RandomIter2> inline
        RandomIter1 __find_end(RandomIter1 first, RandomIter1 last, RandomIter2 searchFirst, RandomIter2 searchLast, BinPred pred,
                random_access_iterator_tag, random_access_iterator_tag)
        {
            typename iterator_traits<RandomIter2>::difference_type len2 = searchLast - searchFirst;
            if(len2 <= 0)
                return last;
            typename iterator_traits<RandomIter1>::difference_type len1 = last - first;
            if(len1 < len2)
                return last;
            const RandomIter1 s = first + (len2 - 1);
            RandomIter1 new_last = last;
            RandomIter2 new_search_last = searchLast;
            --new_search_last;
            while(true){
                while(true){
                    if(new_last == s)    // no matched
                        return last;
                    if(pred(*--new_last, *new_search_last))
                        break;
                }
                RandomIter1 pos = new_last;
                RandomIter2 searchPos = new_search_last;
                while(true){
                    if(searchPos == searchFirst)
                        return pos;
                    if(!pred(*--pos, *--searchPos))
                        break;
                }
            }
        }

    template<typename ForwardIter1, typename ForwardIter2, typename BinPred> inline
        ForwardIter1 find_end(ForwardIter1 first, ForwardIter1 last, ForwardIter2 searchFirst, ForwardIter2 searchLast, BinPred pred)
        {
            return __find_end<std::add_lvalue_reference<BinPred>::type>
                (first, last, searchFirst, searchLast, pred, 
                 typename iterator_traits<ForwardIter1>::iterator_category(),
                 typename iterator_traits<ForwardIter2>::iterator_category());
        }

    template<typename ForwardIter1, typename ForwardIter2> inline
        ForwardIter1 find_end(ForwardIter1 first, ForwardIter1 last, ForwardIter2 searchFirst, ForwardIter2 searchLast)
        {
            typedef typename iterator_traits<ForwardIter1>::value_type v1;
            typedef typename iterator_traits<ForwardIter2>::value_type v2;
            return find_end(first, last, searchFirst, searchLast, __equal_to<v1, v2>());
        }

    // find_first_of
    template<typename InputIter, typename ForwardIter, typename BinPred> inline
        InputIter find_first_of(InputIter first1, InputIter last1, ForwardIter first2, ForwardIter last2, BinPred pred)
        {
            for(; first1 != last1; ++first1)
                for(ForwardIter p = first2; p != last2; ++p)
                    if (pred(*first1, *p)) {
                        return first1;
                    }
            return last1;
        }
    
    template<typename InputIter, typename ForwardIter> inline
        InputIter find_first_of(InputIter first1, InputIter last1, ForwardIter first2, ForwardIter last2)
        {
            typedef typename iterator_traits<InputIter>::value_type v1;
            typedef typename iterator_traits<ForwardIter>::value_type v2;
            return find_first_of(first1, last1, first2, last2, __equal_to<v1, v2>());
        }

    // all_of
    template<typename InputIter, typename UnaryPred> inline
        bool all_of(InputIter first, InputIter last, UnaryPred pred)
        {
            for(; first != last; ++first)
                if(!pred(*first))
                    return false;
            return true;
        }

    // any_of
    template<typename InputIter, typename UnaryPred> inline
        bool any_of(InputIter first, InputIter last, UnaryPred pred)
        {
            for(; first != last; ++first)
                if(pred(*first))
                    return true;
            return false;
        }

    // none_of
    template<typename InputIter, typename UnaryPred> inline
        bool none_of(InputIter first, InputIter last, UnaryPred pred)
        {
            for(; first != last; ++first)
                if(pred(*first))
                    return false;
            return true;
        }
    ///~ ------------------------- end of pattern matching -------------------------------
    
    
    ///: --------------------- reverse ----------------------------
    // reverse
    template<typename BiIter> inline
        void __reverse(BiIter first, BiIter last, bidirectional_iterator_tag)
        {
            while(first != last){
                if(first == --last)
                    break;
                iter_swap(first, last);
                ++first;
            }
        }
    
    template<typename RandomIter> inline
        void __reverse(RandomIter first, RandomIter last, random_access_iterator_tag)
        {
            if(first != last){
                for(; first < --last; ++first)
                    iter_swap(first, last);
            }
        }

    template<typename BiIter> inline
        void reverse(BiIter first, BiIter last)
        {
            return __reverse(first, last, typename iterator_traits<BiIter>::iterator_category());
        }

    // reverse_copy
    template<typename BiIter, typename OutputIter> inline
        OutputIter reverse_copy(BiIter first, BiIter last, OutputIter result)
        {
            for(; first != last; ++result)
                *result = *--last;
            return result;
        }
    ///~ ------------------- end reverse --------------------------
    

    ///: ------------------- rotate ------------------------------
	// rotate
	template<typename ForwardIter> inline
		ForwardIter __rotate_left(ForwardIter beg, ForwardIter end)
	{
		typedef typename iterator_traits<ForwardIter>::value_type value_type;
		value_type temp = std::move(*beg);
		ForwardIter last = move(next(beg), end, beg);
		*last = std::move(temp);
		return last;
	}

	template<typename BiIter> inline
		BiIter __rotate_right(BiIter beg, BiIter end)
	{
		typedef typename iterator_traits<BiIter>::value_type value_type;
		BiIter last = prev(end);
		value_type temp = std::move(*last);
		BiIter first = move_backward(beg, last, end);
		*beg = std::move(temp);
		return first;
	}

	template<typename Integral> inline
		Integral __gcd(Integral x, Integral y)
	{
		while (y != 0){
			Integral t = x % y;
			x = y;
			y = t;
		}
		return x;
	}

	template<typename RandomIter> inline
		RandomIter __rotate_gcd(RandomIter beg, RandomIter new_beg, RandomIter end)
	{
		typedef typename iterator_traits<RandomIter>::value_type value_type;
		typedef typename iterator_traits<RandomIter>::difference_type difference_type;

		difference_type m1 = distance(beg, new_beg);
		difference_type m2 = distance(new_beg, end);
		if (m1 == m2){
			swap_ranges(beg, new_beg, new_beg);
			return new_beg;
		}
		const difference_type m = __gcd(m1, m2);
		RandomIter p = beg + m;
		while (p != beg){
			value_type temp = std::move(*(--p));
			RandomIter p1 = p;
			RandomIter p2 = p1 + m1;
			do{
				*p1 = std::move(*p2);
				p1 = p2;
				if (p2 + m1 < end)
					p2 += m1;
				else
					p2 = beg + m1 - (end - p2);
			} while (p2 != p);
			*p1 = std::move(temp);
		}
		return beg + m2;
	}

	template<typename ForwardIter> inline
		ForwardIter __rotate_forward(ForwardIter beg, ForwardIter new_beg, ForwardIter end)
	{
		ForwardIter second;
		while (true){
			second = new_beg;
			swap(*beg, *second);
			++beg;
			++second;
			if (second == end)
				break;
			if (beg == new_beg)
				new_beg = second;
		}
		ForwardIter retPos = beg;
		if (beg != new_beg){
			second = new_beg;
			while (true){
				swap(*beg, *second);
				++beg;
				++second;
				if (second == end){
					if (beg == new_beg)
						break;
					second = new_beg;
				}
				else if (beg == new_beg)
					new_beg = second;
			}
		}
		return retPos;
	}

	template<typename RandomIter> inline
		RandomIter __rotate(RandomIter beg, RandomIter new_beg, RandomIter end, random_access_iterator_tag)
	{
		typedef typename iterator_traits<RandomIter>::value_type value_type;
		if (std::is_trivially_move_assignable<value_type>::value){
			if (next(beg) == new_beg){
				return __rotate_left(beg, end);
			}
			if (prev(end) == new_beg){
				return __rotate_right(beg, end);
			}
			return __rotate_gcd(beg, new_beg, end);
		}
		return __rotate_forward(beg, new_beg, end);
	}

	template<typename BiIter> inline
		BiIter __rotate(BiIter beg, BiIter new_beg, BiIter end, bidirectional_iterator_tag)
	{
		typedef typename iterator_traits<BiIter>::value_type value_type;
		if (std::is_trivially_move_assignable<value_type>::value){
			if (next(beg) == new_beg){
				return __rotate_left(beg, end);
			}
			if (prev(end) == new_beg){
				return __rotate_right(beg, end);
			}
		}
		return __rotate_forward(beg, new_beg, end);
	}

	template<typename ForwardIter> inline
		ForwardIter __rotate(ForwardIter beg, ForwardIter new_beg, ForwardIter end, forward_iterator_tag)
	{
		typedef typename iterator_traits<ForwardIter>::value_type value_type;
		if (std::is_trivially_move_assignable<value_type>::value){
			if (next(beg) == new_beg){
				return __rotate_left(beg, end);
			}
		}
		return __rotate_forward(beg, end);
	}

	template<typename ForwardIter> inline
		ForwardIter rotate(ForwardIter beg, ForwardIter new_beg, ForwardIter end)
	{
		if (beg == new_beg)
			return end;
		if (new_beg == end)
			return beg;
		return __rotate(beg, new_beg, end, typename iterator_traits<ForwardIter>::iterator_category());
	}

    template<typename ForwardIter, typename OutputIter> inline
        OutputIter rotate_copy(ForwardIter beg, ForwardIter new_beg, ForwardIter end, OutputIter result)
        {
            return copy(beg, new_beg, copy(new_beg, end, result));
        }
    ///~ ----------------------- end rotate ----------------------------------------------
    

    ///: --------------------- partition -----------------------------------------------
    // is_partitioned
    template<typename InputIter, typename UnaryPred> inline
        bool is_partitioned(InputIter first, InputIter last, UnaryPred pred)
        {
            for(; first != last; ++first)
                if(!pred(*first))
                    break;
            if(first == last)
                return true;
            ++first;
            for(; first != last; ++first)
                if(pred(*first))
                    return false;
            return true;
        }
    
    // partition
    template<typename UnaryPred, typename ForwardIter> inline
       ForwardIter __partition(ForwardIter first, ForwardIter last, UnaryPred pred, forward_iterator_tag)
       {
           // find first element make pred(e) false
           while(true){
               if(first == last)
                   return last;
               if(!pred(*first))
                   break;
               ++first;
           }
           // then find first element make pred(e) true and swap
           for(ForwardIter p = first; ++p != last;){
               if(pred(*p)){
                   swap(*first, *p);
                   ++first;
               }
           }
           return first;
       } 

    template<typename UnaryPred, typename BiIter> inline
        BiIter __partition(BiIter first, BiIter last, UnaryPred pred, bidirectional_iterator_tag)
        {
            while(true){
                while(true){
                    if(first == last)
                        return last;
                    if(!pred(*first))
                        break;
                    ++first;
                }
                // backward. find the first elements make pred(e) true
                do{
                    if(first == --last)  // meet in the middle
                        return first;
                }while(!pred(*last));
                swap(*first, *last);
                ++first;
            }
        }

    template<typename ForwardIter, typename UnaryPred> inline
        ForwardIter partition(ForwardIter first, ForwardIter last, UnaryPred pred)
        {
            return __partition<typename std::add_lvalue_reference<UnaryPred>::type>
                (first, last, pred, typename iterator_traits<ForwardIter>::iterator_category());
        }

    //partition_copy
    template<typename ForwardIter, typename OutputIter1, typename OutputIter2, typename UnaryPred> inline
        std::pair<OutputIter1, OutputIter2> 
        partition_copy(ForwardIter first, ForwardIter last, OutputIter1 destTrueFirst, OutputIter2 destFalseFirst, UnaryPred pred)
        {
            for(; first != last; ++first){
                if(pred(*first)){
                    *destTrueFirst = *first;
                    ++destTrueFirst;
                }
                else{
                    *destFalseFirst = *first;
                    ++destFalseFirst;
                }
            }
            return std::make_pair<OutputIter1, OutputIter2>(destTrueFirst, destFalseFirst);
        }
    
    // partition_point
    template<typename ForwardIter, typename UnaryPred> inline
        ForwardIter partition_point(ForwardIter first, ForwardIter last, UnaryPred pred)
        {
            // binary search
            typedef typename iterator_traits<ForwardIter>::difference_type difference_type;
            difference_type len = distance(first, last);
            while(len != 0)
            {
                difference_type sz = len / 2;
                ForwardIter p = first;
                advance(p, sz);
                if(pred(*p)){
                    first = ++p;
                    len = len - (sz - 1);
                }
                else
                    len = sz;
            }
            return first;
        }

    // stable_partition
    // with no usage of temporary buffer
    template<typename UnaryPred, typename ForwardIter, typename Distance> inline
        ForwardIter __stable_partition(ForwardIter first, ForwardIter last, UnaryPred pred, Distance len, forward_iterator_tag unused)
        {
            if(len == 1){              // base case 1
                return first;
            }
            if(len == 2){              // base case 2
                ForwardIter second = first;
                if(pred(*++second)){
                    swap(*first, *second);
                    return second;
                }
                return first;
            }
            // divide (left part)
            ForwardIter p = first;      // p pointer to the new_last of the divided subSequence
            Distance len_left = len / 2;  
            advance(p, len_left); 
            typedef typename std::add_lvalue_reference<UnaryPred>::type  PredRef;
            // conquer left
            ForwardIter first_false = __stable_partition<PredRef>(first, p, pred, len_left, unused);
            
            // divide (right part)
            ForwardIter pr = p;
            ForwardIter second_false = last;
            Distance len_right = len - len_left;
            // skip the begin right part of the right part.
            while(pred(*pr)){
                // right part are all true. just combine(rotate)
                if(++pr == last)
                    return rotate(first_false, p, second_false);
                --len_right;
            }
            // conquer right
            second_false = __stable_partition<PredRef>(pr, last, pred, len_right, unused);

            // combine
            return rotate(first_false, p, second_false);
        }

    template<typename UnaryPred, typename ForwardIter> inline
        ForwardIter __stable_partition(ForwardIter first, ForwardIter last, UnaryPred pred, forward_iterator_tag)
        {
            // either all true and return first
            // or point to the first false
            while(true){
                if(first == last)     // all true
                    return first;
                if(!pred(*first))
                    break;
                ++first;
            }
            typedef typename iterator_traits<ForwardIter>::difference_type difference_type;
            difference_type len = distance(first, last);
            return __stable_partition<std::add_lvalue_reference<UnaryPred>::type>
                (first, last, pred, len, forward_iterator_tag());
        }

    template<typename UnaryPred, typename BiIter, typename Distance> inline
        BiIter __stable_partition(BiIter first, BiIter last, UnaryPred pred, Distance len, bidirectional_iterator_tag unused)
        {
            if(len == 2){   // base case 1
                swap(*first, *last);
                return last;
            }
            if(len == 3){   // base case 2
                BiIter mid = first;
                if(pred(*++mid)){
                    swap(*first, *mid);
                    swap(*mid, *last);
                    return last;
                }
                swap(*mid, *last);
                swap(*mid, *first);
                return mid;
            }
            BiIter mid = first;
            Distance len_half = len / 2;
            advance(mid, len_half);
            // divide left
            BiIter pl = mid;
            Distance len_left = len_half;
            BiIter first_false = first;
            while(!pred(*--pl)){
                if(pl == first)
                    break;
                --len_left;
            }
            typedef typename std::add_lvalue_reference<UnaryPred>::type PredRef;
            if(pl != first){
                first_false = __stable_partition<PredRef>(first, pl, pred, len_left, unused);
            }

            //divide right
            BiIter pr = mid;
            Distance len_right = len - len_half;
            BiIter second_false = last;
            ++second_false;
            while(pred(*pr)){
                if(++pr == last)
                    break;
                --len_right;
            }
            if(pr != last){
                second_false = __stable_partition<PredRef>(pr, last, pred, len_right, unused);
            }

            // combine
            return rotate(first_false, mid, second_false);
        }

    template<typename UnaryPred, typename BiIter> inline
        BiIter __stable_partition(BiIter first, BiIter last, UnaryPred pred, bidirectional_iterator_tag)
        {
            // first point to the first false.
            while(true){
                if(first == last)   // all true
                    return last;
                if(!pred(*first))
                    break;
                ++first;
            }

            do{
                if(first == --last)   // all false
                    return first;
            }while(!pred(*last));
            
            typedef typename iterator_traits<BiIter>::difference_type difference_type;
            difference_type len = distance(first, last) + 1;
            return __stable_partition<std::add_lvalue_reference<UnaryPred>::type>
                (first, last, pred, len, bidirectional_iterator_tag());
        }

    template<typename ForwardIter, typename UnaryPred> inline
        ForwardIter stable_partition(ForwardIter first, ForwardIter last, UnaryPred pred)
        {
            return __stable_partition<ForwardIter, std::add_lvalue_reference<UnaryPred>::type>
                (first, last, pred, typename iterator_traits<ForwardIter>::iterator_category());
        }
    ///~ ----------------------------- end of partition -----------------------------------
    

    ///: ---------------------------- premutation ---------------------------------------
    // is_premutation
    template<typename ForwardIter1, typename ForwardIter2, typename Compare> inline
    bool is_permutation(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2, Compare comp)
    {
        // mismatch
        for(; first1 != last1; ++first1, ++first2)
            if(!comp(*first1, *first2))
                break;

        if(first1 != last1){
            typedef typename iterator_traits<ForwardIter1>::difference_type DType1;
            DType1 len1 = distance(first1, last1);
            if(len1 == static_cast<DType1>(1))     // only one element left but not equal.
                return false;
            ForwardIter2 last2 = first2;
            advance(last2, len1);
            // for each element in two range, compare the times which appears
            for(ForwardIter1 i = first1; i != last1; ++i){
                if(i != find(first1, i, *i))
                    continue;                 // already counted *i before.
                // count *i in [first2, last2)
                DType1 count2(0);
                for(ForwardIter2 j = first2; j != last2; ++j)
                    if(comp(*i, *j))
                        ++count2;
                if(count2 == 0)             // *i not found in [first2, last2)
                    return false; 
                // count *i in the [i, last1), because we have make sure that in
                // [first1, i) has no element equal to *i
                DType1 count1(1);
                for(ForwardIter1 j = next(i); j != last1; ++j)
                    if(comp(*i, *j))
                        ++count1;
                if(count1 != count2)        // if not equal in count.
                    return false;
            }
        }
        return true;   
    }
    
    template<typename ForwardIter1, typename ForwardIter2> inline
        bool is_permutation(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
        {
            typedef typename iterator_traits<ForwardIter1>::value_type v1;
            typedef typename iterator_traits<ForwardIter2>::value_type v2;
            return is_permutation(first1, last1, first2, __equal_to<v1, v2>());
        }

    // next_permutation
    template<typename BiIter, typename Compare> inline
        bool next_permutation(BiIter first, BiIter last, Compare comp)
        {
            BiIter i = last;
            if(first == last || first == --i)
                return false;
            while(true){
                BiIter next_i = i;
                // find the adjacent_pair which i < next_i
                if(comp(*--i, *next_i)){
                    // then find backward the first elem j > i
                    BiIter j = last;
                    while(!comp(*i, *--j));
                    // after found. swap i and j
                    swap(*i, *j);
                    // then reverse the element after i
                    reverse(next_i, last);
                    return true;
                }
                // if not found pair, then all permutation has found.
                // that is all elem desending.
                if(i == first){
                    // reverse all
                    reverse(first, last);
                    return false;
                }
            }
        }

    template<typename BiIter> inline
        bool next_permutation(BiIter first, BiIter last)
        {
            return next_permutation(first, last, __less<typename iterator_traits<BiIter>::value_type>());
        }

    // prev_permutation
    template<typename BiIter, typename Compare> inline
        bool prev_permutation(BiIter first, BiIter last, Compare comp)
        {
            BiIter i = last;
            if(first == last || first == --i)
                return false;
            while(true){
                BiIter next_i = i;
                if(comp(*next_i, *--i)){
                    BiIter j = last;
                    while(!comp(*--j, *i));
                    swap(*i, *j);
                    reverse(next_i, last);
                    return true;
                }
                if(i == first){
                    reverse(first, last);
                    return false;
                }
            }
        }

    template<typename BiIter> inline
        bool prev_permutation(BiIter first, BiIter last)
        {
            return prev_permutation(first, last, __less<typename iterator_traits<BiIter>::value_type>());
        }

    ///~ ------------------------------------ end of permutation ---------------------------------------
    
    
    ///: ------------------------------------ sort ----------------------------------------
    // is_sorted_until
    template<typename ForwardIter, typename Compare> inline
        ForwardIter is_sorted_until(ForwardIter first, ForwardIter last, Compare comp)
        {
            if(first != last){
                ForwardIter next = first;
                while(++next != last){
                    if(comp(*next, *first))
                        return next;
                    first = next;
                }
            }
            return last;
        }

    template<typename ForwardIter> inline
        ForwardIter is_sorted_until(ForwardIter first, ForwardIter last)
        {
            return is_sorted_until(first, last, __less<typename iterator_traits<ForwardIter>::value_type>());
        }

    // is_sorted
    template<typename ForwardIter, typename Compare> inline
        bool is_sorted(ForwardIter first, ForwardIter last, Compare comp)
        {
            return is_sorted_until(first, last, comp) == last;
        }

    template<typename ForwardIter> inline
        bool is_sorted(ForwardIter first, ForwardIter last)
        {
            return is_sorted(first, last, __less<typename iterator_traits<ForwardIter>::value_type>());
        }

    // sort
    
    ///~ ---------------------- end of sort -----------------------------------------------
    
    
    ///: ------------------------ merge ---------------------------------
    //merge
    template<typename InputIter1, typename InputIter2, typename OutputIter, typename Compare> inline
        OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result, Compare comp)
        {
            while(first1 != last1 && first2 != last2){
                // if f1 == f2, first merge f1, then f2.
                if(comp(*first2, *first1)){   // for stable, compare f2 < f1
                    *result = *first2;
                    ++result;
                    ++first2;
                }
                else{
                    *result = *first1;
                    ++result;
                    ++first1;
                }
            }
            if(first1 != last1)
                return copy(first1, last1, result);
            if(first2 != last2)
                return copy(first2, last2, result);
            return result;
        }

    template<typename InputIter1, typename InputIter2, typename OutputIter> inline
        OutputIter merge(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result)
        {
            typedef typename iterator_traits<InputIter1>::value_type v1;
            typedef typename iterator_traits<InputIter2>::value_type v2;
            return merge(first1, last1, first2, last2, result, __less<v1, v2>());
        }

    // inplace_merge
    
    ///~ ---------------------------------- end merge ----------------------------------------
    //
    //
    ///: --------------------------------- set sorted -------------------------------------------
    // set_union
    template<typename InputIter1, typename InputIter2, typename OutputIter, typename Compare> inline
        OutputIter set_union(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result, Compare comp)
        {
            while(first1 != last1 && first2 != last2){
                if(comp(*first2, *first1)){ // if e2 < e1
                    *result = *first2;
                    ++first2;
                    ++result;
                }
                else{                      // if e1 <= e2
                    *result = *first1;
                    ++result;
                    if(!comp(*first1, *first2))   // e1 == e2
                        ++first2;
                    ++first1;
                }
            }
            if(first1 != last1)
                return copy(first1, last1, result);
            if(first2 != last2)
                return copy(first2, last2, result);
            return result;
        }

    template<typename InputIter1, typename InputIter2, typename OutputIter> inline
        OutputIter set_union(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result)
        {
            typedef typename iterator_traits<InputIter1>::value_type v1;
            typedef typename iterator_traits<InputIter2>::value_type v2;
            return set_union(first1, last1, first2, last2, result, __less<v1, v2>());
        }
 
    // set_intersection
    template<typename InputIter1, typename InputIter2, typename OutputIter, typename Compare> inline
        OutputIter set_intersection(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result, Compare comp)
        {
            while(first1 != last1 && first2 != last2)
            {
                if(comp(*first2, *first1))     // e2 < e1
                    ++first2;
                else if(comp(*first1, *first2))  // e1 < e2
                    ++first1;
                else{                       // e1 == e2
                    *result = *first1;
                    ++result;
                    ++first1;
                    ++first2;
                }
            }
            return result;
        }

    template<typename InputIter1, typename InputIter2, typename OutputIter> inline
        OutputIter set_intersection(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result)
        {
            typedef typename iterator_traits<InputIter1>::value_type v1;
            typedef typename iterator_traits<InputIter2>::value_type v2;
            return set_intersection(first1, last1, first2, last2, result, __less<v1, v2>());
        }

    // set_difference
    template<typename InputIter1, typename InputIter2, typename OutputIter, typename Compare> inline
        OutputIter set_difference(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result, Compare comp)
        {
            while(first1 != last1){
                if(first2 == last2)
                    return copy(first1, last1, result);
                if(comp(*first1, *first2)){
                    *result = *first1;
                    ++result;
                    ++first1;
                }
                else{
                    if(!comp(*first2, *first1))    // e1 == e2
                        ++first1;
                    ++first2;
                }
            }
            return result;
        }

    template<typename InputIter1, typename InputIter2, typename OutputIter> inline
        OutputIter set_difference(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result)
        {
            typedef typename iterator_traits<InputIter1>::value_type v1;
            typedef typename iterator_traits<InputIter2>::value_type v2;
            return set_difference(first1, last1, first2, last2, result, __less<v1, v2>());
        }
    
    // set_symmetric_difference
    template<typename InputIter1, typename InputIter2, typename OutputIter, typename Compare> inline
        OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result, Compare comp)
        {
            while(first1 != last1 && first2 != last2){
                if(comp(*first1, *first2)){
                    *result = *first1;
                    ++result;
                    ++first1;
                }
                else if(comp(*first2, *first1)){
                    *result = *first2;
                    ++result;
                    ++first2;
                }
                else{
                    ++first1;
                    ++first2;
                }
            }
            if(first1 != last1)
                return copy(first1, last1, result);
            if(first2 != last2)
                return copy(first2, last2, result);
            return result;
        }

    template<typename InputIter1, typename InputIter2, typename OutputIter> inline
        OutputIter set_symmetric_difference(InputIter1 first1, InputIter1 last1, InputIter2 first2, InputIter2 last2, OutputIter result)
        {
            typedef typename iterator_traits<InputIter1>::value_type v1;
            typedef typename iterator_traits<InputIter2>::value_type v2;
            return set_symmetric_difference(first1, last1, first2, last2, result, __less<v1, v2>());
        }

    ///~ ---------------------------------- end set sorted -----------------------------------------
}

#endif
