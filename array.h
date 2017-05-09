#ifndef MY_ARRAY_H
#define MY_ARRAY_H
#include <cstddef>
#include <initializer_list>
#include <type_traits>
#include <tuple>
#include "iterator.h"
#include "algorithm.h"

namespace learnSTL{
	template<typename T, size_t Size>
	struct array{
		typedef array              self;
		typedef T                  value_type;
		typedef T*                 pointer;
		typedef const T*           const_pointer;
		typedef T&                 reference;
		typedef const T&           const_reference;
		typedef ptrdiff_t          difference_type;
		typedef size_t             size_type;

		typedef pointer                            iterator;
		typedef const_pointer                      const_iterator;
		typedef learnSTL::reverse_iterator<iterator>         reverse_iterator;
		typedef learnSTL::reverse_iterator<const_iterator>   const_reverse_iterator;

		value_type __elems[Size > 0 ? Size : 1];

		// iterator
		iterator       begin()          { return __elems; }
		const_iterator begin() const    { return const_iterator(__elems); }
		iterator       end()            { return __elems + Size; }
		const_iterator end() const      { return const_iterator(__elems + Size); }
		const_iterator cbegin() const   { return begin(); }
		const_iterator cend() const     { return end(); }
		reverse_iterator rbegin()       { return reverse_iterator(end()); }
		const_reverse_iterator rbegin() const { return const_reverse_iterator(cend()); }
		reverse_iterator rend()          { return reverse_iterator(begin()); }
		const_reverse_iterator rend() const { return const_reverse_iterator(cbegin()); }
		const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
		const_reverse_iterator crend() const   { return const_reverse_iterator(cbegin()); }

		// implicitly declared constructor, copy constructor, move constructor and operator=
		// array();
        // array(const array<T, Size>& a2);
        // array(array<T, Size>&& ar);
        // array<T, Size>& operator=(const array<T, Size>& a2);
        

		void fill(const value_type& val){
            learnSTL::fill_n(__elems, Size, val);
		}
		void swap(array& a2){
			learnSTL::swap_ranges(begin(), end(), a2.begin());
		}

		//access elem
		reference operator[](size_type n) { return __elems[n]; }
		const_reference operator[](size_type n) const { return __elems[n]; }
		reference at(size_type n)
		{
			return __elems[n];
		}
		const_reference at(size_type n) const
		{
			return __elems[n];
		}
		reference front() { return __elems[0]; }
		const_reference front() const { return __elems[0]; }
		reference back() { return __elems[Size > 0 ? Size - 1 : 0]; }
		const_reference back() const { return __elems[Size > 0 ? Size - 1 : 0]; }
		pointer data() { return __elems; }
		const_pointer data() const { return __elems; }
		
		//size
		bool empty() const {
			return Size == 0;
		}
		size_type size() const { return Size; }
		size_type max_size() const { return Size; }

	};

	//compare
	template<typename T, size_t Size> bool
		operator==(const array<T, Size>& a1, const array<T, Size>& a2)
	{
		return learnSTL::equal(a1.begin(), a1.end(), a2);
	}
	template<typename T, size_t Size> bool
		operator!=(const array<T, Size>& a1, const array<T, Size>& a2)
	{
		return !(a1 == a2);
	}
	template<typename T, size_t Size> bool
		operator<(const array<T, Size>& a1, const array<T, Size>& a2)
	{
		return learnSTL::lexicographical_compare(a1.begin(), a1.end(), a2.begin(), a2.end());
	}
	template<typename T, size_t Size> bool
		operator>(const array<T, Size>& a1, const array<T, Size>& a2)
	{
		return a2 < a1;
	}
	template<typename T, size_t Size> bool
		operator<=(const array<T, Size>& a1, const array<T, Size>& a2)
	{
		return !(a2 < a1);
	}
	template<typename T, size_t Size> bool
		operator>=(const array<T, Size>& a1, const array<T, Size>& a2)
	{
		return !(a1 < a2);
	}
	template<typename T, size_t Size> void
		swap(array<T, Size>& a1, array<T, Size>& a2)
	{
		a1.swap(a2);
	}
}
 
#endif
