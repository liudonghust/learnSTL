#ifndef ITERATOR_H
#define ITERATOR_H
#include <type_traits>
#include <cstddef>        // size_t ptrdiff_t
#include <istream>
#include <ostream>
#include <memory>

namespace learnSTL{
	struct input_iterator_tag {};
	struct output_iterator_tag {};
	struct forward_iterator_tag : public input_iterator_tag {};
	struct bidirectional_iterator_tag : public forward_iterator_tag{};
	struct random_access_iterator_tag : public bidirectional_iterator_tag{};

	// test if param Tp is iterator
	template<typename Tp>
	class __has_iterator_category{
		struct __twoBytes{         // only for test
			char oneByte;
			char twoByte;
		};
		template <class Up> static __twoBytes test(...);
		template <class Up> static char test(typename Up::iterator_category*);
	public:
		static const bool value = sizeof(test<Tp>(nullptr)) == 1;
	};

	// if Iter is iterator
	template<typename Iter, bool> struct __iterator_traits_impl {};
	template<typename Iter>
	struct __iterator_traits_impl<Iter, true>
	{
		typedef typename Iter::value_type          value_type;
		typedef typename Iter::difference_type     difference_type;
		typedef typename Iter::pointer             pointer;
		typedef typename Iter::reference           reference;
		typedef typename Iter::iterator_category   iterator_category;
	};


	// how to predicate if "Iter" is a iterator 
	// even though defined "iterator_category"
	template<typename Iter, bool> struct __iterator_traits {};
	template<typename Iter>
	struct __iterator_traits<Iter, true>
		: __iterator_traits_impl
		< Iter,
		std::is_convertible<typename Iter::iterator_category, input_iterator_tag>::value ||
		std::is_convertible<typename Iter::iterator_category, output_iterator_tag>::value
		>
	{};


	template<typename Iter>
	struct iterator_traits : public __iterator_traits < Iter, __has_iterator_category<Iter>::value >
	{};

	template<typename T>
	class iterator_traits < T* > {
	public:
		typedef typename std::remove_const<T>::type   value_type;
		typedef ptrdiff_t                    difference_type;
		typedef T*                           pointer;
		typedef T&                           reference;
		typedef random_access_iterator_tag   iterator_category;
	};

	template<typename Category, typename T, typename Diffence = ptrdiff_t,
			 typename Pointer = T*, typename Reference = T&>
	struct iterator
	{
		typedef T                  value_type;
		typedef Diffence           difference_type;
		typedef Pointer            pointer;
		typedef Reference          reference;
		typedef Category           iterator_category;
	};


	/* Auxiliary function */
	template<typename InputIter> inline
	void __advance(InputIter& iter, typename iterator_traits<InputIter>::difference_type n, input_iterator_tag)
	{
		while (n > 0){
			++iter;
			--n;
		}
	}
	template<typename InputIter> inline
		void __advance(InputIter& iter, typename iterator_traits<InputIter>::difference_type n, bidirectional_iterator_tag)
	{
		if (n > 0){
			for (; n > 0; --n)
				++iter;
		}
		else{
			for (; n < 0; ++n)
				--iter;
		}
	}
	template<typename InputIter> inline
		void __advance(InputIter& iter, typename iterator_traits<InputIter>::difference_type n, random_access_iterator_tag)
	{
		iter += n;
	}
	template<typename InputIter> inline
	void advance(InputIter& iter, typename iterator_traits<InputIter>::difference_type n)
	{
		__advance(iter, n, typename iterator_traits<InputIter>::iterator_category());
	}

	template<typename InputIter> inline
		typename iterator_traits<InputIter>::difference_type
		__distance(InputIter iter1, InputIter iter2, input_iterator_tag)
	{
		typename iterator_traits<InputIter>::difference_type count = 0;
		for (; iter1 != iter2; ++iter1)
			++count;
		return count;
	}
	template<typename InputIter> inline
		typename iterator_traits<InputIter>::difference_type
		__distance(InputIter iter1, InputIter iter2, random_access_iterator_tag)
	{
		return iter2 - iter1;
	}

	template<typename InputIter> inline
		typename iterator_traits<InputIter>::difference_type
		distance(InputIter iter1, InputIter iter2)
	{
		return __distance(iter1, iter2, typename iterator_traits<InputIter>::iterator_category());
	}


	template<typename Iter, typename InputIter, bool = __has_iterator_category<iterator_traits<Iter> >::value>
	struct __has_iterator_category_and_convertible_to
		: public std::integral_constant<bool, std::is_convertible<typename iterator_traits<Iter>::iterator_category, InputIter>::value>
	{};
	template<typename Iter, typename InputIter>
	struct __has_iterator_category_and_convertible_to<Iter, InputIter, false>
		: public std::false_type {};

	template<typename Iter>
	struct __is_input_iterator : public __has_iterator_category_and_convertible_to < Iter, input_iterator_tag > {};

	template<typename Iter>
	struct __is_forward_iterator : public __has_iterator_category_and_convertible_to < Iter, forward_iterator_tag > {};

	template<typename Iter>
	struct __is_bidirectional_iterator : public __has_iterator_category_and_convertible_to < Iter, bidirectional_iterator_tag > {};

	template<typename InputIter> inline
		InputIter next(InputIter iter, typename std::enable_if<__is_input_iterator<InputIter>::value>::type* = nullptr)
	{
		return ++iter;
	}

	template<typename BidirectIter> inline
		BidirectIter prev(BidirectIter iter, typename std::enable_if<__is_bidirectional_iterator<BidirectIter>::value>::type* = nullptr)
	{
		return --iter;
	}


	// iterator adaptor
	template<typename Iter>
	class reverse_iterator
	{
	protected:
		Iter current;
	public:
		typedef Iter                    iterator_type;
		typedef reverse_iterator<Iter>  self_type;

		typedef typename iterator_traits<Iter>::iterator_category iterator_category;
		typedef typename iterator_traits<Iter>::value_type        value_type;
		typedef typename iterator_traits<Iter>::reference         reference;
		typedef typename iterator_traits<Iter>::pointer           pointer;
		typedef typename iterator_traits<Iter>::difference_type   difference_type;

		reverse_iterator() : current() {}
		explicit reverse_iterator(Iter iter) : current(iter) {}

		template<typename Iter2>
		reverse_iterator(const reverse_iterator<Iter2>& rev_iter2) : current(rev_iter2.base()) {}

		iterator_type base() const{
			return current;
		}

		reference operator*() const {
			iterator_type temp = current;
			return *(--temp);
		}
		pointer operator->() const{
			return std::addressof(operator*());
		}

		self_type& operator++(){
			--current;
			return *this;
		}
		self_type operator++(int) {
			self_type temp(*this);
			--current;
			return temp;
		}
		self_type& operator--() {
			++current;
			return *this;
		}
		self_type operator--(int) {
			self_type temp(*this);
			++current;
			return temp;
		}
		// for random_access_iterator
		self_type operator+(difference_type n) const{
			return reverse_iterator(current - n);
		}
		self_type operator+=(difference_type n) {
			current -= n;
			return *this;
		}
		self_type operator-(difference_type n) const{
			return reverse_iterator(current + n);
		}
		self_type operator-=(difference_type n) {
			current += n;
			return *this;
		}
		reference operator[](difference_type n) const{
			return *(*this + n);
		}
	};

	template<typename Iter1, typename Iter2> inline
		bool operator==(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() == y.base();
	}
	template<typename Iter1, typename Iter2> inline
		bool operator!=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() != y.base();
	}
	template<typename Iter1, typename Iter2> inline
		bool operator<(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() > y.base();
	}
	template<typename Iter1, typename Iter2> inline
		bool operator<=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() >= y.base();
	}
	template<typename Iter1, typename Iter2> inline
		bool operator>(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() < y.base();
	}
	template<typename Iter1, typename Iter2> inline
		bool operator>=(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return x.base() <= y.base();
	}
	//iter1 - iter2
	template<typename Iter1, typename Iter2> inline
		typename iterator_traits<Iter1>::difference_type 
		operator-(const reverse_iterator<Iter1>& x, const reverse_iterator<Iter2>& y)
	{
		return y.base() - x.base();
	}
	// n + iter
	template<typename Iter> inline
		reverse_iterator<Iter> 
		operator+(typename iterator_traits<Iter>::difference_type n, const reverse_iterator<Iter>& x)
	{
		return reverse_iterator<Iter>(x.base() - n);
	}

	template<typename Iter> inline
		reverse_iterator<Iter> make_reverse_iterator(Iter iter)
	{
		return reverse_iterator<Iter>(iter);
	}
	///~ end reverse_iterator

	///: back_insert_iterator
	template<typename Container>
	class back_insert_iterator
		: public iterator < output_iterator_tag, void, void, void, void >
	{
	protected:
		Container* containerPtr;
	public:
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::reference reference;

		explicit back_insert_iterator(Container& cont) : containerPtr(std::addressof(cont)) {}
		back_insert_iterator& operator=(const typename Container::value_type& value){
			containerPtr->push_back(value);
			return *this;
		}
		back_insert_iterator& operator=(const typename Container::value_type&& value){
			containerPtr->push_back(std::move(value));
			return *this;
		}

		back_insert_iterator& operator*() { return *this; }
		back_insert_iterator& operator++() { return *this; }
		back_insert_iterator  operator++(int) { return *this; }
	};
	
	template<typename Container> inline
		back_insert_iterator<Container> back_inserter(Container& cont)
	{
		return back_insert_iterator<Container>(cont);
	}
	///~ end back_insert_iterator

	///: front_insert_iterator
	template<typename Container>
	class front_insert_iterator
		: public iterator < output_iterator_tag, void, void, void, void >
	{
	protected:
		Container* containerPtr;
	public:
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::reference reference;

		explicit front_insert_iterator(Container& cont) : containerPtr(std::addressof(cont)) {}
		front_insert_iterator& operator=(const typename Container::value_type& value){
			containerPtr->push_front(value);
			return *this;
		}
		front_insert_iterator& operator=(const typename Container::value_type&& value){
			containerPtr->push_front(std::move(value));
			return *this;
		}

		front_insert_iterator& operator*() { return *this; }
		front_insert_iterator& operator++() { return *this; }
		front_insert_iterator  operator++(int) { return *this; }
	};
	template<typename Container> inline
		front_insert_iterator<Container> front_inserter(Container& cont)
	{
		return front_insert_iterator<Container>(cont);
	}
	///~ end front_insert_iterator

	///: insert_iterator
	template<typename Container>
	class insert_iterator : public iterator < output_iterator_tag, void, void, void, void >
	{
	protected:
		Container* containerPtr;
		typename Container::iterator iter;
	public: 
        typedef Container container_type;
        typedef typename Container::value_type value_type;
        typedef typename Container::reference reference;

		explicit insert_iterator(Container& cont, typename Container::iterator it) 
			: containerPtr(std::addressof(cont)),
			  iter(it) {}
		insert_iterator& operator=(typename Container::value_type& value){
			containerPtr->insert(iter, value);
			++iter;
			return *this;
		}
		insert_iterator& operator=(typename Container::value_type&& value){
			containerPtr->insert(iter, std::move(value));
			++iter;
			return *this;
		}

		insert_iterator& operator*()       { return *this; }
		insert_iterator& operator++()      { return *this; }
		insert_iterator  operator++(int)   { return *this; }
	};
	template<typename Container> inline
		insert_iterator<Container> inserter(Container& cont, typename Container::iterator iter)
	{
		return insert_iterator<Container>(cont, iter);
	}
	///~ end insert_iterator

	///: ostream iterator
	template<typename T, typename CharT = char, class Traits = std::char_traits<CharT> >
	class ostream_iterator : public iterator < output_iterator_tag, void, void, void, void >
	{
	public:
		typedef CharT char_type;
        typedef Traits traits_type;
		typedef std::basic_ostream<CharT, Traits> ostream_type;
	private:
		ostream_type* output_stream;
		const char_type* delim;
	public:
		explicit ostream_iterator(ostream_type& os) : output_stream(std::addressof(os)), delim(0) {}
		ostream_iterator(ostream_type& os, const char_type* delimiter)
			: output_stream(std::addressof(os)),
			  delim(delimiter)
		{}
		ostream_iterator& operator=(const T& value){
			(*output_stream) << value;
			if (delim){
				(*output_stream) << delim;
			}
			return *this;
		}

		ostream_iterator& operator*()   { return *this; }
		ostream_iterator& operator++()  { return *this; }
		ostream_iterator& operator++(int) { return *this; }
	};///~ end ostream_iterato

	///: istream_iterator
	template<typename T, typename CharT = char, typename Traits = std::char_traits<CharT>, typename Distance = ptrdiff_t >
	class istream_iterator : public iterator < input_iterator_tag, T, Distance, const T*, const T& >
	{
	public:
		typedef CharT char_type;
        typedef Traits traits_type;
		typedef std::basic_istream<CharT, Traits> istream_type;
	private:
		istream_type* input_stream;
		T value;
	public:
		istream_iterator() : input_stream(nullptr), value(){}
		explicit istream_iterator(istream_type& is) : input_stream(std::addressof(is)){
			if (!(*input_stream >> value))
				input_stream = nullptr;
		}

		const T& operator*() const { return value; }
		const T* operator->() const { return std::addressof(operator*()); }
		istream_iterator& operator++(){
			if (!(*input_stream >> value))
				input_stream = nullptr;
			return *this;
		}
		istream_iterator operator++(int){
			istream_iterator temp(*this);
			++(*this);
			return temp;
		}
		friend bool operator==(const istream_iterator& x, const istream_iterator& y){
			return x.input_stream == y.input_stream;
		}
		friend bool operator!=(const istream_iterator& x, const istream_iterator& y){
			return x.input_stream != y.input_stream;
		}
	};///~ end istream_iterator
	
	///: move_iterator
	template<typename Iter>
	class move_iterator{
	public:
		typedef Iter                   iterator_type;

		typedef typename iterator_traits<Iter>::iterator_category  iterator_category;
		typedef typename iterator_traits<Iter>::value_type         value_type;
		typedef typename iterator_traits<Iter>::difference_type    difference_type;
		//typedef iterator_traits<Iter>::reference          reference;
		typedef value_type&&                              reference;
		typedef typename iterator_traits<Iter>::pointer            pointer;

	private:
		iterator_type __iter;
	public:
		move_iterator() : __iter() {}
		explicit move_iterator(const iterator_type& iter) : __iter(iter) {}
		template<typename Iter2> move_iterator(const move_iterator<Iter2>& mi) : __iter(mi.base()) {}

		iterator_type base() const { return __iter; }
		reference operator*() const{
			return static_cast<reference>(*__iter);
		}
		pointer operator->() const{
			typename iterator_traits<iterator_type>::reference ref = *__iter;
			return std::addressof(ref);
		}
		move_iterator& operator++() { ++__iter; return *this; }
		move_iterator  operator++(int)
		{
			move_iterator __tmp(*this); ++__iter; return __tmp;
		}
		move_iterator& operator--() { --__iter; return *this; }
		move_iterator  operator--(int)
		{
			move_iterator __tmp(*this); --__iter; return __tmp;
		}
		move_iterator  operator+ (difference_type n) const
		{
			return move_iterator(__iter + n);
		}
		move_iterator& operator+=(difference_type n)
		{
			__iter += n; return *this;
		}
		move_iterator  operator- (difference_type n) const
		{
			return move_iterator(__iter - n);
		}
		move_iterator& operator-=(difference_type n)
		{
			__iter -= n; return *this;
		}
		reference operator[](difference_type n) const
		{
			return static_cast<reference>(__iter[n]);
		}
	};

	template <class Iter1, class Iter2>inline 
	bool operator==(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() == y.base();
	}
	template <class Iter1, class Iter2> inline
	bool operator!=(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() != y.base();
	}
	template <class Iter1, class Iter2> inline 
	bool operator<(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() < y.base();
	}
	template <class Iter1, class Iter2> inline 
	bool operator>(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() > y.base();
	}
	template <class Iter1, class Iter2> inline 
	bool operator>=(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() >= y.base();
	}
	template <class Iter1, class Iter2> inline 
	bool operator<=(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() <= y.base();
	}
	template <class Iter1, class Iter2> inline
	typename move_iterator<Iter1>::difference_type
	operator-(const move_iterator<Iter1>& x, const move_iterator<Iter2>& y)
	{
		return x.base() - y.base();
	}
	template <class Iter> inline
	move_iterator<Iter>
	operator+(typename move_iterator<Iter>::difference_type n, const move_iterator<Iter>& x)
	{
		return move_iterator<Iter>(x.base() + n);
	}
	template <class Iter> inline
	move_iterator<Iter>
	make_move_iterator(Iter i)
	{
		return move_iterator<Iter>(i);
	}
}

template<typename T, size_t sz>
T* begin(T (&arr)[sz]) { return arr; }

template<typename T, size_t sz>
T* end(T (&arr)[sz]) { return arr + sz; }

template<typename Container>
auto begin(Container& c) -> decltype(c.begin())
{
    return c.begin();
}

template<typename Container>
auto begin(const Container& c) -> decltype(c.begin()) { return c.begin(); }

template<typename Container>
auto end(Container& c) -> decltype(c.end()) { return c.end(); }

template<typename Container>
auto end(const Container& c) -> decltype(c.end()) { return c.end(); }

template<typename Container>
auto cbegin(const Container& c) -> decltype(begin(c)) { return begin(c); }

template<typename Container>
auto cend(const Container& c) -> decltype(end(c)) { return end(c); }

template<typename Container>
auto rbegin(Container& c) -> decltype(c.rbegin()) { return c.rbegin(); }

template<typename Container>
auto rbegin(const Container& c) -> decltype(c.rbegin()) { return c.rbegin(); }

template<typename Container>
auto rend(Container& c) -> decltype(c.rend()) { return c.rend(); }

template<typename Container>
auto rend(const Container& c) -> decltype(c.rend()) { return c.rend(); }

template<typename Container>
auto crbegin(const Container& c) -> decltype(rbegin(c)) { return rbegin(c); }

template<typename Container>
auto crend(const Container& c) -> decltype(rend(c)) { return rend(c); }



#endif
