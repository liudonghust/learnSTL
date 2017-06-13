#ifndef MY_UTILITY_H
#define MY_UTILITY_H

#include <initializer_list>
#include <cstddef>
#include <type_traits>
#include "xtuple.h"

namespace learnSTL{
    // ------------------ support for rvalue and move semantic -----------
    // forward
    template<typename T> inline
        T&& forward(typename std::remove_reference<T>::type& t)
        {
            return static_cast<T&&>(t);
        }

    template<typename T> inline
        T&& forward(typename std::remove_reference<T>::type&& t)
        {
            static_assert(!std::is_lvalue_reference<T>::value, "Can not forward an rvalue as an lvalue.");
            return static_cast<T&&>(t);
        }

    // move
    template<typename T> inline
        typename std::remove_reference<T>::type&&
        move(T&& t)
        {
            typedef typename std::remove_reference<T>::type UnRef;
            return static_cast<UnRef&&>(t);
        }

    // ---------------------- generic swap ---------------------------------
	//swap  
	template<typename T> inline
		typename std::enable_if<std::is_move_constructible<T>::value &&
		std::is_move_assignable<T>::value,
		void>::type
		swap(T& x, T& y)
	{
		T temp(move(x));
		x = move(y);
		y = move(temp);
	}

	// swap array  
	template<typename T, size_t N> inline
		void swap(T(&a)[N], T(&b)[N])
	{
		swap_ranges(a, a + N, b);
	}

	// iter_swap  
	template<typename ForwardIter1, typename ForwardIter2> inline
		void iter_swap(ForwardIter1 iter1, ForwardIter2 iter2)
	{
		swap(*iter1, *iter2);
	}

    // ------------------------ rel_ops --------------------------
    namespace rel_ops{
        template<typename T> inline
        bool operator!=(const T& x, const T& y)
        {
            return !(x == y);
        }

        template<typename T> inline
        bool operator>(const T& x, const T& y)
        {
            return y < x;
        }

        template<typename T> inline
        bool operator<=(const T& x, const T& y)
        {
            return !(y < x);
        }

        template<typename T> inline
        bool operator>=(const T& x, const T& y)
        {
            return !(x < y);
        }
    }
    
    // -------------------- pair ----------------------------------------
    template<typename T1, typename T2>
    struct pair{
        typedef T1 first_type;
        typedef T2 second_type;

        first_type first;
        second_type second;

        // constructor
        constexpr pair() : first(), second() {}
        pair(const T1& val1, const T2& val2) : first(val1), second(val2) {}
        template<typename U1, typename U2, typename = typename std::enable_if<std::is_convertible<U1, T1>::value 
                                                               && std::is_convertible<U2, T2>::value, void>::type>
            pair(U1&& rv1, U2&& rv2) : first(learnSTL::forward<U1>(rv1)), second(learnSTL::forward<U2>(rv2)) {}

        // copy constructor
        template<typename U1, typename U2, typename = typename std::enable_if<std::is_convertible<U1, T1>::value &&
                                                      std::is_convertible<U2, T2>::value, void>::type> 
            pair(const pair<U1, U2>& p2) : first(p2.first), second(p2.second) {}
        pair(const pair& p) = default;

        // move constructor
        template<typename U1, typename U2,  typename = typename std::enable_if<std::is_convertible<U1, T1>::value &&
                                                       std::is_convertible<U2, T2>::value, void>::type>
            pair(pair<U1, U2>&& rp) : first(learnSTL::forward<U1>(rp.first)), second(learnSTL::forward<U2>(rp.second)) {}
        pair(pair&& rp) = default;

        // copy assignment
        template<typename U1, typename U2,  typename = typename std::enable_if<std::is_convertible<U1, T1>::value &&
                                                       std::is_convertible<U2, T2>::value, void>::type>
            pair& operator=(const pair<U1, U2>& p2){
                first = p2.first;
                second = p2.second;
                return *this;
            } 
        pair& operator=(const pair&) = default;
        
        // move assignment
        template<typename U1, typename U2,  typename = typename std::enable_if<std::is_convertible<U1, T1>::value &&
                                                       std::is_convertible<U2, T2>::value, void>::type>
            pair& operator=(pair<U1, U2>&& rp){
                first = learnSTL::forward<U1>(rp.first);
                second = learnSTL::forward<U2>(rp.second);
                return *this;
            }
        pair& operator=(pair&& rp) = default;

        void swap(pair& p2){
            swap(first, p2.first);
            swap(second, p2.second);
        }
    };

    // compare
    template<typename T1, typename T2> inline
        bool operator==(const pair<T1, T2>& p1, const pair<T1, T2>& p2)
        {
            return p1.first == p2.first && p1.second == p2.second;
        }

    template<typename T1, typename T2> inline
        bool operator!=(const pair<T1, T2>& p1, const pair<T1, T2>& p2)
        {
            return !(p1 == p2);
        }
    template<typename T1, typename T2> inline
        bool operator<(const pair<T1, T2>& p1, const pair<T1, T2>& p2)
        {
            return p1.first < p2.first || (!(p2.first < p1.first) && p1.second < p2.second);
        }
    template<typename T1, typename T2> inline
        bool operator>(const pair<T1, T2>& p1, const pair<T1, T2>& p2)
        {
            return p2 < p1;
        }
    template<typename T1, typename T2> inline
        bool operator<=(const pair<T1, T2>& p1, const pair<T1, T2>& p2)
        {
            return !(p2 < p1);
        }
    template<typename T1, typename T2> inline
        bool operator>=(const pair<T1, T2>& p1, const pair<T1, T2>& p2)
        {
            return !(p1 < p2);
        }      

    template<typename T1, typename T2> inline
        void swap(pair<T1, T2>& p1, pair<T1, T2>& p2)
        {
            p1.swap(p2);
        }
   
    // just for return type of make_pair
    template<typename T>
        struct __make_pair_return{
            typedef T type;
        };

    template<typename T>
        struct __make_pair_return<std::reference_wrapper<T> >
        {
            typedef T& type;
        };

    template<typename T1, typename T2> inline
        pair<typename __make_pair_return<typename std::decay<T1>::type>::type,
             typename __make_pair_return<typename std::decay<T2>::type>::type>
        make_pair(T1&& rv1, T2&& rv2)
        {
            return  pair<typename __make_pair_return<typename std::decay<T1>::type>::type,
                         typename __make_pair_return<typename std::decay<T2>::type>::type>
                        (learnSTL::forward<T1>(rv1), learnSTL::forward<T2>(rv2));
        }
    
    // tuple-like interface
    // tuple_size
    template<typename T1, typename T2>
        class tuple_size<pair<T1, T2>> : public std::integral_constant<size_t, 2> {};

    
    // tuple_element
    template<typename T1, typename T2>
        class tuple_element<0, pair<T1, T2>>{
        public:
            using type = T1;
        };

    template<typename T1, typename T2>
        class tuple_element<1, pair<T1, T2>>{
        public:
            using type = T2;
        };

    // get
    template<size_t> struct __get_pair;

    template<> struct __get_pair<0>{
        template<typename T1, typename T2>
            static T1& get(pair<T1, T2>& p){
                return p.first;
            }
        template<typename T1, typename T2>
            static const T1& get(const pair<T1, T2>& p){
                return p.first;
            }
        template<typename T1, typename T2>
            static T1&& get(pair<T1, T2>&& p){
                return forward<T1>(p.first);
            }
    };

    template<> struct __get_pair<1>{
        template<typename T1, typename T2>
            static T2& get(pair<T1, T2>& p){
                return p.second;
            }
        template<typename T1, typename T2>
            static const T2& get(const pair<T1, T2>& p){
                return p.second;
            }
        template<typename T1, typename T2>
            static T2&& get(pair<T1, T2>&& p){
                return forward<T2>(p.second);
            }
    };

    template<size_t Index, typename T1, typename T2> inline
        typename tuple_element<Index, pair<T1, T2>>::type&
        get(pair<T1, T2>& p){
            return __get_pair<Index>::get(p);
        }

    template<size_t Index, typename T1, typename T2> inline
        typename tuple_element<Index, pair<T1, T2>>::type const&
        get(const pair<T1, T2>& p){
            return __get_pair<Index>::get(p);
        }

    template<size_t Index, typename T1, typename T2> inline
        typename tuple_element<Index, pair<T1, T2>>::typel&&
        get(pair<T1, T2>&& p){
            return __get_pair<Index>::get(learnSTL::move(p));
        }
}

#endif
