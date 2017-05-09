#ifndef MY_VECTOR_H
#define MY_VECTOR_H
#include <memory>
#include <utility>
#include <initializer_list>
#include <type_traits>
#include "iterator.h"
#include "algorithm.h"

namespace learnSTL{
	template<typename T, typename Allocator>
	class __vector_base{
	public:
		typedef Allocator                                   allocator_type;
		typedef typename std::allocator_traits<Allocator>   __alloc_traits;
		typedef T                                           value_type;
		typedef value_type&                                 reference;
		typedef const value_type&                           const_reference;
		typedef typename __alloc_traits::pointer            pointer;
		typedef typename __alloc_traits::const_pointer      const_pointer;
		typedef typename __alloc_traits::size_type          size_type;
		typedef typename __alloc_traits::difference_type    difference_type;

		typedef pointer                                     iterator;
		typedef const_pointer                               const_iterator;
	protected:
		pointer                             __begin;
		pointer                             __end;
		pointer                             __end_cap;
		allocator_type                      __alloc;

		__vector_base() : __begin(nullptr), __end(nullptr), __end_cap(nullptr), __alloc() {}
		__vector_base(const allocator_type& a) : __begin(nullptr), __end(nullptr), __end_cap(nullptr), __alloc(a) {}

		void __destruct_at_end(pointer start){
			while (start != __end){
				__alloc_traits::destroy(__alloc, --__end);
			}
		}
		void clear(){
			__destruct_at_end(__begin);
		}
		size_type capacity() const{
			return static_cast<size_type>(__end_cap - __begin);
		}

		void __copy_assign_alloc(const __vector_base& c)
		{
			__copy_assign_alloc_impl(c, std::integral_constant<bool, __alloc_traits::propagate_on_container_copy_assignment::value>());
		}


		void __move_assign_alloc(const __vector_base& c)
		{
			__move_assign_alloc_impl(c, std::integral_constant<bool, __alloc_traits::propagate_on_container_move_assignment::value>());
		}

		~__vector_base(){
			if (__begin != nullptr){
				clear();
				__alloc_traits::deallocate(__alloc, __begin, capacity());
			}
		}
	private:
		// if c.allocator can copyed to this, then update this->allocator to c.allocator
		void __copy_assign_alloc_impl(const __vector_base& c, std::true_type)
		{
			// if not equal. then c.__alloc cannot deallocate the momory of this
			// so that we need first to deallocate the momory.
			if (this->__alloc != c.__alloc){
				clear();
				__alloc_traits::deallocate(this->__alloc, this->__begin, capacity());
				this->__begin = this->__end = this->__end_cap = nullptr;
			}
			this->__alloc = c.__alloc;
		}
		// if c.allocator cannot copyed to this, then nothing to do
		void __copy_assign_alloc_impl(const __vector_base&, std::false_type) {}

		void __move_assign_alloc_impl(const __vector_base& c, std::true_type){
			this->__alloc = move(c.__alloc);
		}

		void __move_assign_alloc_impl(const __vector_base&, std::false_type){}
	};  ///~ end of vector_base

	template<typename T, typename Allocator = std::allocator<T> >
	class vector : private __vector_base < T, Allocator >
	{
	private:
		typedef __vector_base<T, Allocator>       __base;
		typedef std::allocator<T>                 __default_allocator_type;
	public:
		typedef vector<T, Allocator>              self;
		typedef Allocator                         allocator_type;
		typedef typename __base::__alloc_traits   __alloc_traits;

		typedef T                                 value_type;
		typedef typename __base::reference        reference;
		typedef typename __base::const_reference  const_reference;
		typedef typename __base::pointer          pointer;
		typedef typename __base::const_pointer    const_pointer;
		typedef typename __base::size_type        size_type;
		typedef typename __base::difference_type  difference_type;

		//iterator
		typedef pointer                               iterator;
		typedef const_pointer                         const_iterator;
		typedef std::reverse_iterator<iterator>       reverse_iterator;
		typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

		iterator begin(){ return this->__begin; }
		const_iterator begin() const { return this->__begin; }
		iterator end() { return this->__end; }
		const_iterator end() const { return this->__end; }
		const_iterator cbegin() const{ return this->__begin; }
		const_iterator cend() const { return this->__end; }
		reverse_iterator rbegin() { return reverse_iterator(end()); }
        const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
		const_reverse_iterator crbegin() { return const_reverse_iterator(cend()); }
		reverse_iterator rend() { return reverse_iterator(begin()); }
        const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
		const_reverse_iterator crend() { return const_reverse_iterator(cbegin()); }

		//constructors
		vector() {}
		explicit vector(const allocator_type& a) : __base(a) {}
		explicit vector(size_type n){
			if (n > 0){
				allocate(n);
				__construct_at_end(n);
			}
		}
		vector(size_type n, const allocator_type& a) : __base(a)
		{
			if (n > 0){
				allocate(n);
				__construct_at_end(n);
			}
		}
		vector(size_type n, const_reference val)
		{
			if (n > 0){
				allocate(n);
				__construct_at_end(n, val);
			}
		}
		vector(size_type n, const_reference val, const allocator_type& a) : __base(a)
		{
			if (n > 0){
				allocate(n);
				__construct_at_end(n, val);
			}
		}
		template<typename InputIter>
		vector(InputIter first, typename std::enable_if<__is_input_iterator<InputIter>::value &&
														!__is_forward_iterator<InputIter>::value,
														InputIter>::type last)
		{
			for (; first != last; ++first)
				push_back(*first);
		}

		template<typename InputIter>
		vector(InputIter first, typename std::enable_if<__is_input_iterator<InputIter>::value &&
			!__is_forward_iterator<InputIter>::value,
			InputIter>::type last, const allocator_type& a) : __base(a)
		{
			for (; first != last; ++first)
				push_back(*first);
		}

		template<typename ForwardIter> 
		vector(ForwardIter first, typename std::enable_if<__is_forward_iterator<ForwardIter>::value,
                                                          ForwardIter>::type last)
		{
			size_type n = static_cast<size_type>(learnSTL::distance(first, last));
			if (n > 0){
				allocate(n);
				__construct_at_end(first, last);
			}
		}

		template<typename ForwardIter>
		vector(ForwardIter first, typename std::enable_if<__is_forward_iterator<ForwardIter>::value,
                ForwardIter>::type last, const allocator_type& a) : __base(a)
		{
			size_type n = static_cast<size_type>(distance(first, last));
			if (n > 0){
				allocate(n);
				__construct_at_end(first, last);
			}
		}

		vector(std::initializer_list<value_type> init_list){
			if (init_list.size() > 0){
				allocate(init_list.size());
				__construct_at_end(init_list.begin(), init_list.end());
			}
		}
		vector(std::initializer_list<value_type> init_list, const allocator_type& a) : __base(a)
		{
			if (init_list.size() > 0){
				allocate(init_list.size());
				__construct_at_end(init_list.begin(), init_list.end());
			}
		}

		// copy constructor
		vector(const self& v) : __base(v.__alloc)
		{
			size_type n = v.size();
			if (n > 0){
				allocate(n);
				__construct_at_end(v.begin(), v.end());
			}
		}
		vector(const self& v, const allocator_type& a) : __base(a)
		{
			size_type n = v.size();
			if (n > 0){
				allocate(n);
				__construct_at_end(v.begin(), v.end());
			}
		}
		
		// move constructor
		vector(self&& rv) : __base(std::move(rv.__alloc))
		{
			this->__begin = rv.__begin;
			this->__end = rv.__end;
			this->__end_cap = rv.__end_cap;
			rv.__begin = rv.__end = rv.__end_cap = nullptr;
		}
		vector(self&& rv, const allocator_type& a) : __base(a)
		{
			if (a == rv.__alloc)
			{
				this->__begin = rv.__begin;
				this->__end = rv.__end;
				this->__end_cap = rv.__end_cap;
				rv.__begin = rv.__end = rv.__end_cap = nullptr;
			}
			else
			{
				typedef move_iterator<iterator> _Ip;
				assign(_Ip(rv.begin()), _Ip(rv.end()));
			}
		}


		// assignment
		void assign(size_type n, const_reference val){
			if (n < capacity()){
				size_type s = size();
				if (n <= s){
					fill_n(this->__begin, n, val);
					this->__destruct_at_end(this->__begin + n);
				}
				else{
					fill_n(this->__begin, s, val);
					__construct_at_end(n - s, val);
				}
			}
			else{
				deallocate();
				allocate(__grow_to(n));
				__construct_at_end(n, val);
			}
		}

		template<typename InputIter>
		typename std::enable_if<__is_input_iterator<InputIter>::value &&
								!__is_forward_iterator<InputIter>::value,
							    void>::type
		assign(InputIter first, InputIter last)
		{
			clear();
			for (; first != last; ++first)
			{
				push_back(*first);
			}
		}

		template<typename ForwardIter>
		typename std::enable_if<__is_forward_iterator<ForwardIter>::value, void>::type
			assign(ForwardIter first, ForwardIter last)
		{
			size_type new_size = static_cast<size_type>(distance(first, last));
			if (new_size <= capacity()){
				size_type sz = size();
				if (new_size <= sz){
					pointer endp = copy(first, last, this->__begin);
					this->__destruct_at_end(endp);
				}
				else{
					ForwardIter iter = first;
					advance(iter, sz);
					copy(first, iter, this->__begin);
					__construct_at_end(iter, last);
				}
			}
			else{
				deallocate();
				allocate(__grow_to(new_size));
				__construct_at_end(first, last);
			}
		}

		void assign(std::initializer_list<value_type> initlist)
		{
			assign(initlist.begin(), initlist.end());
		}

		self& operator=(const self& v){
			if (this != &v){
				__base::__copy_assign_alloc(v);
				assign(v.begin(), v.end());
			}
			return *this;
		}
		self& operator=(self&& rv){
			if (this != &rv){
				__move_assign(rv, std::integral_constant<bool, __alloc_traits::propagate_on_container_move_assignment::value>());
			}
			return *this;
		}
		self& operator=(std::initializer_list<value_type> initlist){
			assign(initlist.begin(), initlist.end());
			return *this;
		}

		void swap(self& v2){
			learnSTL::swap(this->__begin, v2.__begin);
			learnSTL::swap(this->__end, v2.__end);
			learnSTL::swap(this->__end_cap, v2.__end_cap);
			__swap_allocator(this->__alloc, v2.__alloc, 
				std::integral_constant<bool, __alloc_traits::propagate_on_container_swap::value>());
		}


		// access elements
		reference  operator[](size_type n) {
			return *(this->__begin + n);
		}
		const_reference operator[](size_type n) const {
			return *(this->__begin + n);
		}
		reference at(size_type n){
			if (n > size()){
				//this->__throw_out_of_range();
			}
			return *(this->__begin + n);
		}
		const_reference at(size_type n) const {
			if (n > size()){
				//this->__throw_out_of_range();
			}
			return *(this->__begin + n);
		}
		reference front(){
			return *this->__begin;
		}
		const_reference front() const {
			return *this->__begin;
		}
		reference back(){
			return *(this->__end - 1);
		}
		const_reference back() const {
			return *(this->__end - 1);
		}
		value_type* data() {
			return this->__begin;
		}
		const value_type* data() const {
			return this->__begin;
		}


		//insert
		void push_back(const_reference val){
			if (this->__end != this->__end_cap){
				__alloc_traits::construct(this->__alloc, this->__end, val);
				++this->__end;
			}
			else{
				vector<value_type> temp;
				temp.__alloc = this->__alloc;
				temp.allocate(__grow_to(size() + 1));
				temp.__construct_at_end(this->__begin, this->__end);
				__alloc_traits::construct(temp.__alloc, temp.__end, val);
				++temp.__end;
				swap(temp);
			}
		}
		void push_back(value_type&& rval){
			if (this->__end != this->__end_cap){
				__alloc_traits::construct(this->__alloc, this->__end, std::move(rval));
				++this->__end;
			}
			else{
				vector<value_type> temp;
				temp.__alloc = this->__alloc;
				temp.allocate(__grow_to(size() + 1));
				temp.__construct_at_end(this->__begin, this->__end);
				__alloc_traits::construct(temp.__alloc, temp.__end, std::move(rval));
				++temp.__end;
				swap(temp);
			}
		}
		iterator insert(const_iterator pos, const_reference val)
		{
			iterator p = this->__begin + (pos - this->__begin);  // remove const
			if (this->__end < this->__end_cap){
				if (p == this->__end){
					__alloc_traits::construct(this->__alloc, this->__end, val);
					++this->__end;
				}
				else{
					__move_range(p, this->__end, p + 1);
					const_pointer valPtr = std::pointer_traits<const_pointer>::pointer_to(val);
					// if before move_range, val lives between [p, __end), 
					// then the val to be inserted is move to next position.
					if (p <= valPtr && valPtr < this->__end)
						++valPtr;
					*p = *valPtr;
				}
			}
			else{
				vector<value_type, allocator_type> temp;
				temp.allocate(__grow_to(size() + 1));
				temp.__construct_at_end(this->__begin, p);
				temp.push_back(val);
				temp.__construct_at_end(p, this->__end);
				swap(temp);
			}
			return p;
		}
		iterator insert(const_iterator pos, value_type&& val)
		{
			iterator p = this->__begin + (pos - this->__begin);  // remove const
			if (this->__end < this->__end_cap){
				if (p == this->__end){
					__alloc_traits::construct(this->__alloc, this->__end, std::move(val));
					++this->__end;
				}
				else{
					__move_range(p, this->__end, p + 1);
					const_pointer valPtr = std::pointer_traits<const_pointer>::pointer_to(val);
					// if before move_range, val lives between [p, __end), 
					// then the val to be inserted is move to next position.
					if (p <= valPtr && valPtr < this->__end)
						++valPtr;
					*p = std::move(*valPtr);
				}
			}
			else{
				vector<value_type, allocator_type> temp;
				temp.allocate(__grow_to(size() + 1));
				temp.__construct_at_end(this->__begin, p);
				temp.push_back(std::move(val));
				temp.__construct_at_end(p, this->__end);
				swap(temp);
			}
			return p;
		}
		iterator insert(const_iterator pos, size_type n, const_reference val){
			iterator p = this->__begin + (pos - this->__begin);
			size_type sz = size();
			if (n > 0){
				if (sz + n <= capacity()){
					iterator old_last = this->__end;
					size_type old_n = n;
					if (n > static_cast<size_type>(this->__end - p)){
						size_type beyondN = n - (this->__end - p);
						__construct_at_end(n, val);
						n -= beyondN;
					}
					if (n > 0){
						__move_range(p, old_last, p + old_n);
						const_pointer valPtr = std::pointer_traits<const_pointer>::pointer_to(val);
						if (p <= valPtr && valPtr < old_last)
							valPtr += old_n;
						fill_n(p, n, *valPtr);
					}
				}
				else{
					vector<value_type, allocator_type> temp;
					temp.allocate(__grow_to(size() + n));
					temp.__construct_at_end(this->__begin, p);
					temp.__construct_at_end(n, val);
					temp.__construct_at_end(p, this->__end);
					swap(temp);
				}
			}
			return p;
		}
		
		template<typename InputIter>
		typename std::enable_if<__is_input_iterator<InputIter>::value &&
			!__is_forward_iterator<InputIter>::value,
			iterator>::type
			insert(const_iterator pos, InputIter first, InputIter last)
		{
			iterator old_last = this->__end;
			iterator p = this->__begin + (pos - this->__begin);
			for (; this->__end != this->__end_cap && first != last; ++first)
			{
				__alloc_traits::construct(this->__alloc, this->__end, *first);
				++this->__end;
			}
			// if end == end_cap but first != last, means
			// capacity is not enough to hold all elements
			vector<value_type> temp;
			if (first != last){
				temp.assign(first, last);
				// record size and off as reallocate will invalid old iterator
				difference_type old_size = old_last - this->__begin;
				difference_type old_off = p - this->__begin;
				reserve(__grow_to(size() + temp.size()));
				old_last = this->__begin + old_size;
				p = this->__begin + old_off;
			}
			p = rotate(p, old_last, this->__end);
			insert(p, make_move_iterator(temp.begin()), make_move_iterator(temp.end()));

			return this->__begin + (pos - this->__begin);
		}

		template<typename ForwardIter>
		typename std::enable_if<__is_forward_iterator<ForwardIter>::value, iterator>::type
			insert(const_iterator pos, ForwardIter first, ForwardIter last)
		{
			difference_type n = distance(first, last);
			iterator p = this->__begin + (pos - this->__begin);
			if (n > 0)
			{
				size_type sz = size();
				if (n + sz <= capacity()){
					iterator old_last = this->__end;
					size_type old_n = n;
					ForwardIter m = last;
					difference_type diff = this->__end - p;
					if (n > diff){
						m = first;
						advance(m, diff);
						__construct_at_end(m, last);
						n -= diff;
					}
					if (n > 0){
						__move_range(p, old_last, p + old_n);
						copy(first, m, p);
					}
				}
				else{
					vector<value_type, allocator_type> temp;
					temp.allocate(__grow_to(size() + n));
					temp.__construct_at_end(this->__begin, p);
					temp.__construct_at_end(first, last);
					temp.__construct_at_end(p, this->__end);
					swap(temp);
				}
			}
			return p;
		}

		iterator insert(const_iterator pos, std::initializer_list<value_type> initlist)
		{
			return insert(pos, initlist.begin(), initlist.end());
		}

		template<typename... Args>
		iterator emplace(const_iterator pos, Args&&... args)
		{
			iterator p = this->__begin + (pos - this->__begin);
			if (this->__end != this->__end_cap){
				if (p == this->__end){
					__alloc_traits::construct(this->__alloc, p, std::forward<Args>(args)...);
					++this->__end;
				}
				else{
					value_type temp(std::forward<Args>(args)...);
					__move_range(p, this->__end, p + 1);
					*p = std::move(temp);
				}
			}
			else{
				vector<value_type, allocator_type> temp;
				temp.allocate(__grow_to(size() + 1));
				temp.__construct_at_end(this->__begin, p);
				temp.emplace_back(std::forward<Args>(args)...);
				temp.__construct_at_end(p, this->__end);
				swap(temp);
			}
			return p;
		}

		template<typename... Args>
		void emplace_back(Args&&... args)
		{
			if (this->__end != this->__end_cap){
				__alloc_traits::construct(this->__alloc, this->__end, std::forward<Args>(args)...);
				++this->__end;
			}
			else{
				vector<value_type, allocator_type> temp;
				temp.allocate(__grow_to(size() + 1));
				temp.__construct_at_end(this->__begin, this->__end);
				__alloc_traits::construct(temp.__alloc, temp.__end, std::forward<Args>(args)...);
				++temp.__end;
				swap(temp);
			}
		}


		// delete
		void pop_back(){
			__alloc_traits::destroy(this->__alloc, --this->__end);
		}

		iterator erase(const_iterator pos){
			difference_type off = pos - cbegin();
			pointer p= this->__begin + off;
			iterator ret = this->__begin + off;
			this->__destruct_at_end(learnSTL::move(p + 1, this->__end, p));
			return ret;
		}

		iterator erase(const_iterator first, const_iterator last)
		{
			difference_type n = last - first;
			difference_type off = first - cbegin();
			pointer p = this->__begin + off;
			iterator ret = this->__begin + off;
			this->__destruct_at_end(learnSTL::move(p + n, this->__end, p));
			return ret;
		}

		// some func with size and capacity
        allocator_type get_allocator() const{
            return this->__alloc;
        }
		bool empty() const {
			return this->__begin == this->__end;
		}
		size_type size() const{
			return static_cast<size_type>(this->__end - this->__begin);
		}
		size_type capacity() const{
			return __base::capacity();
		}
		size_type max_size() const {
			return static_cast<size_type>(__alloc_traits::max_size(this->__alloc));
		}
		void reserve(size_type n){
			if (n > capacity()){
				vector v(this->__alloc);
				v.allocate(n);
				v.__construct_at_end(this->__begin, this->__end);
				swap(v);
			}
		}
		void resize(size_type n, const_reference val)
		{
			size_type sz = size();
			if (n <= capacity()){
				if (n < sz){
					this->__destruct_at_end(this->__begin + n);
				}
				else if (n > sz){
					__construct_at_end(n - sz, val);
				}
			}
			else{
				vector<value_type> temp;
				temp.allocate(__grow_to(n));
				temp.__construct_at_end(this->__begin, this->__end);
				temp.__construct_at_end(n - sz, val);
				swap(temp);
			}
		}
		void resize(size_type n){
			resize(n, T());
		}

		void clear(){
			__base::clear();
		}
		~vector(){}
	private:
		void allocate(size_type n){
			/*
			if (n > max_size()){
				//throw_length_error
			}
			*/
			this->__begin = this->__end = __alloc_traits::allocate(this->__alloc, n);
			this->__end_cap = this->__begin + n;
		}
		void deallocate()
		{
			if (this->__begin != nullptr)
			{
				clear();
				__alloc_traits::deallocate(this->__alloc, this->__begin, capacity());
				this->__begin = this->__end = this->__end_cap = nullptr;
			}
		}
		
		void __construct_at_end(size_type n){
			while (n > 0){
				__alloc_traits::construct(this->__alloc, this->__end);
				++this->__end;
				--n;
			}
		}

		void __construct_at_end(size_type n, const_reference val)
		{
			while (n > 0){
				__alloc_traits::construct(this->__alloc, this->__end, val);
				++this->__end;
				--n;
			}
		}

		template<typename ForwardIter> 
		void __construct_at_end(ForwardIter first, typename std::enable_if<__is_forward_iterator<ForwardIter>::value, ForwardIter>::type last)
		{
			for (; first != last; ++first, ++this->__end){
				__alloc_traits::construct(this->__alloc, this->__end, *first);
			}
		}

		// when calling this func, we assume new_size greater than capacity()
		size_type __grow_to(size_type new_size) const{
			const size_type ms = max_size();
			/*
			if (new_size > ms)
			*/
			const size_type cap = capacity();
			// if cannot grow 2 times
			if (cap * 2 >= ms)
				return ms;
			return (2 * cap > new_size ? 2 * cap : new_size);
		}

		void __move_range(iterator from_start, iterator from_end, iterator to){
			iterator old_last = this->__end;
			difference_type n = old_last - to;
			for (iterator i = from_start + n; i < from_end; ++i, ++this->__end){
				__alloc_traits::construct(this->__alloc, this->__end, std::move(*i));
			}
			learnSTL::move_backward(from_start, from_start + n, old_last);
		}

		void __move_assign(self& v, std::true_type)
		{
			deallocate();
			__base::__move_assign_alloc(v);
			this->__begin = v.__begin;
			this->__end = v.__end;
			this->__end_cap = v.__end_cap;
			v.__begin = v.__end = v.__end_cap = nullptr;
		}
		void __move_assign(self& v, std::false_type)
		{
			if (this->__alloc != v.__alloc){
				move_iterator<iterator> moveIter1(v.begin());
				move_iterator<iterator> moveIter2(v.end());
				assign(moveIter1, moveIter2);
			}
			else
				__move_assign(v, std::true_type());
		}

		template<typename Alloc>
		void __swap_allocator(Alloc& a1, Alloc& a2, std::true_type)
		{
			learnSTL::swap(a1, a2);
		}
		template<typename Alloc>
		void __swap_allocator(Alloc&, Alloc&, std::false_type) {}
	};

	// vector compare
	template<typename T, typename Allocator> inline
		bool operator==(const vector<T, Allocator>& v1, const vector<T, Allocator>& v2)
	{
		const typename vector<T, Allocator>::size_type sz = v1.size();
		return sz == v2.size() && equal(v1.begin(), v1.end(), v2.begin());
	}
	template<typename T, typename Allocator> inline
		bool operator!=(const vector<T, Allocator>& v1, const vector<T, Allocator>& v2)
	{
		return !(v1 == v2);
	}
	template<typename T, typename Allocator> inline
		bool operator<(const vector<T, Allocator>& v1, const vector<T, Allocator>& v2)
	{
		return lexicographical_compare(v1.begin(), v1.end(), v2.begin(), v2.end());
	}
	template<typename T, typename Allocator> inline
		bool operator>(const vector<T, Allocator>& v1, const vector<T, Allocator>& v2)
	{
		return v2 < v1;
	}
	template<typename T, typename Allocator> inline
		bool operator<=(const vector<T, Allocator>& v1, const vector<T, Allocator>& v2)
	{
		return !(v2 < v1);
	}
	template<typename T, typename Allocator> inline
		bool operator>=(const vector<T, Allocator>& v1, const vector<T, Allocator>& v2)
	{
		return !(v1 < v2);
	}

	// vector swap
	template<typename T, typename Allocator> inline
		void swap(vector<T, Allocator>& v1, vector<T, Allocator>& v2)
	{
		v1.swap(v2);
	}
}

#endif
