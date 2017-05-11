#ifndef MY_TUPLE_H
#define MY_TUPLE_H

#include <memory>
#include <type_traits>
#include "utility.h"

namespace learnSTL{
    template<typename... Types> class tuple;

    template<typename From, typename To> struct __tuple_convertible {};

    template<> struct __tuple_convertible<tuple<>, tuple<>>{
        using type = std::true_type;
    };

    template<typename From0, typename... From, typename To0, typename... To>
        struct __tuple_convertible<tuple<From0, From...>, tuple<To0, To...>>
            : std::conditional<std::is_convertible<From0, To0>::value,
                                __tuple_convertible<tuple<From...>, tuple<To...>>,
                                __tuple_convertible<int, int>
                              >::type
        {};  

    // base case: empty tuple
    template<> class tuple<>
    {
    public:
        constexpr tuple() {}

        template<typename Alloc>
            tuple(std::allocator_arg_t, const Alloc&) {}

        constexpr tuple(const tuple&) {}
        template<typename Alloc>
           tuple(std::allocator_arg_t, const Alloc&, const tuple&) {}

        void swap(tuple&) {}
    };

    template<typename First, typename... Rest>
        class tuple<First, Rest...> : private tuple<Rest...>
        {
        public:
            typedef First                 first_type;
            typedef tuple<First, Rest...> self;
            typedef tuple<Rest...>        base;
            // constructor
            tuple() : base(), __value() {}

            explicit tuple(const First& first_arg, const Rest&... rest_args)
                : base(rest_args...), __value(first_arg) {}

            template<typename UFirst, typename... UTypes,
                typename = typename __tuple_convertible<tuple<UFirst, UTypes...>, self>::type>
                explicit tuple(UFirst&& first_arg, UTypes&&... rest_args) 
                    : base(learnSTL::forward<UTypes>(rest_args)...), __value(learnSTL::forward<UFirst>(first_arg)) {}
            
            // copy constructor
            template<typename... UTypes>
                tuple(const tuple<UTypes...>& t2) : base(t2.__get_rest()), __value(t2.__value) {}
            tuple(const tuple& p2) = default;

            // move_constructor
            template<typename... UTypes>
                tuple(tuple<UTypes...>&& rt)
                    : base(learnSTL::forward<typename tuple<UTypes...>::base>(rt.__get_rest())),
                      __value(learnSTL::forward<typename tuple<UTypes...>::first_type>(rt.__value))
            {}
            tuple(tuple&& rt) = default;

            //constructor from pair
            template<typename T1, typename T2>
                tuple(const pair<T1, T2>& p) : base(tuple<T2>(p.second)), __value(p.first) {}

            template<typename T1, typename T2>
                tuple(pair<T1, T2>&& rp)
                    : base(tuple<T2>(learnSTL::forward<T2>(rp.second))),
                      __value(learnSTL::forward<T1>(rp.first))
            {}

            
            // assignment
            tuple& operator=(const tuple& p2){
                if(this == &p2) return *this;
                __value = p2.__value;
                base::operator=(p2);
                return *this;
            }

            template<typename...UTypes>
                tuple& operator=(const tuple<UTypes...>& p2){
                    if(this == &p2) return *this;
                    __value = p2.__value;
                    base::operator=(p2);
                    return *this;
                }
            
            tuple& operator=(tuple&& rp){
                if(this == &rp) return *this;
                __value = forward<first_type>(rp.__value);
                base::operator=(forward<base>(rp.__get_rest()));
                return *this;
            }

            template<typename... UTypes>
            tuple& operator=(tuple<UTypes...>&& rp){
                __value = forward<typename tuple<UTypes...>::first_type>(rp.__value);
                base::operator=(forward<typename tuple<UTypes...>::base>(rp.__get_rest()));
                return *this;
            }

            // assignment from pair
            template<typename T1, typename T2>
                tuple& operator=(const pair<T1, T2>& p){
                    __value = p.first;
                    base::operator=(tuple<T2>(p.second));
                    return *this;
                }

            template<typename T1, typename T2>
                tuple& operator=(pair<T1, T2>&& rp){
                    __value = forward<T1>(rp.first);
                    base::operator=(tuple<T2>(forward<T2>(rp.second)));
                    return *this;
                }
            
            // swap
            void swap(tuple& t2){
                using learnSTL::swap;
                swap(__value, t2.__value);
                base::swap(t2.__get_rest());
            }

            base& __get_rest(){
                return *this;          // convert to base class, to cut down the derived part, then get rest.
            }

            const base& __get_rest() const{
                return *this;
            }

            first_type __value;
        };
    ///~ ----------------------- end tuple ---------------------------
    

   // swap
    template<typename...Types1, typename... Types2> inline
        void swap(tuple<Types1...>& t1, tuple<Types2...>& t2){
            t1.swap(t2);
        }

    // tuple_size
    template<typename Tuple> class tuple_size;  

    template<typename...Types>
        class tuple_size<tuple<Types...> >
            : public std::integral_constant<size_t, sizeof...(Types)>
            {};

    template<typename Tuple> class tuple_size<const Tuple>
        : public std::integral_constant<size_t, tuple_size<Tuple>::value> {};

    template<typename Tuple> class tuple_size<volatile Tuple>
        : public std::integral_constant<size_t, tuple_size<Tuple>::value> {};

    template<typename Tuple> class tuple_size<const volatile Tuple>
        : public std::integral_constant<size_t, tuple_size<Tuple>::value> {};



    // tuple_element
    template<size_t Index, typename Tuple> class tuple_element;   // gerneral template

    template<typename First, typename...Rest>
        class tuple_element<0, tuple<First, Rest...> >        // special
        {
        public:
            typedef First type;

            // for get 
            typedef tuple<First, Rest...> TupleType;
        };

    template<size_t Index, typename First, typename...Rest>
        class tuple_element<Index, tuple<First, Rest...> >
            : public tuple_element<Index-1, tuple<Rest...> >
            {};

    template<size_t Index, typename Tuple>
        class tuple_element<Index, const Tuple>
        {
        public:
            typedef typename std::add_const<typename tuple_element<Index, Tuple>::type>::type type;
        };

    template<size_t Index, typename Tuple>
        class tuple_element<Index, volatile Tuple>
        {
        public:
            typedef typename std::add_volatile<typename tuple_element<Index, Tuple>::type>::type type;
        };
    
    template<size_t Index, typename Tuple>
        class tuple_element<Index, const volatile Tuple>
        {
        public:
            typedef typename std::add_cv<typename tuple_element<Index, Tuple>::type>::type type;
        };

    // get
    template<size_t Index, typename...Types>
        typename tuple_element<Index, tuple<Types...> >::type&
        get(tuple<Types...>& t){
            typedef typename tuple_element<Index, tuple<Types...> >::TupleType TupleType;
            return ((TupleType&)t).__value;
        }

    template<size_t Index, typename...Types>
        typename tuple_element<Index, tuple<Types...> >::type const&
        get(const tuple<Types...>& t){
            typedef typename tuple_element<Index, tuple<Types...> >::TupleType TupleType;
            return ((TupleType&)t).__value;
        }

    template<size_t Index, typename...Types>
        typename tuple_element<Index, tuple<Types...> >::type&&
        get(tuple<Types...>&& t){
            typedef typename tuple_element<Index, tuple<Types...> >::TupleType TupleType;
            return ((TupleType&)t).__value;
        }

    // compair
    template<size_t Index> struct __tuple_equal{
        template<typename Tuple1, typename Tuple2>
            bool operator()(const Tuple1& t1, const Tuple2& t2){
                return __tuple_equal<Index-1>()(t1, t2) && (get<Index - 1>(t1) == get<Index-1>(t2));
            }
    };

    template<> struct __tuple_equal<0>{
        template<typename Tuple1, typename Tuple2>
            bool operator()(const Tuple1&, const Tuple2&){
                return true;
            }
    };

    template<size_t Index> struct __tuple_less{
        template<typename Tuple1, typename Tuple2>
            bool operator()(const Tuple1& t1, const Tuple2& t2){
                const size_t i = tuple_size<Tuple1>::value - Index;
                if(get<i>(t1) < get<i>(t2))
                    return true;
                if(get<i>(t2) < get<i>(t1))
                    return false;
                return __tuple_less<Index - 1>()(t1, t2);
            }
    };

    template<> struct __tuple_less<0>{
        template<typename Tuple1, typename Tuple2>
            bool operator()(const Tuple1&, const Tuple2&){
                return false;
            }
    };

    template<typename...Types1, typename...Types2> inline
        bool operator==(const tuple<Types1...>& t1, const tuple<Types2...>& t2){
            static_assert(sizeof...(Types1) == sizeof...(Types2), "can not compare two tuple with different size.");
            return __tuple_equal<sizeof...(Types1)>()(t1, t2);
        }
    
    template<typename...Types1, typename...Types2> inline
        bool operator!=(const tuple<Types1...>& t1, const tuple<Types2...>& t2){
            return !(t1 == t2);
        }

    template<typename...Types1, typename...Types2> inline
        bool operator<(const tuple<Types1...>& t1, const tuple<Types2...>& t2){
            return __tuple_less<sizeof...(Types1)>()(t1, t2);
        }

    template<typename...Types1, typename...Types2> inline
        bool operator>(const tuple<Types1...>& t1, const tuple<Types2...>& t2){
            return t2 < t1;
        }

    template<typename...Types1, typename...Types2> inline
        bool operator<=(const tuple<Types1...>& t1, const tuple<Types2...>& t2){
            return !(t2 < t1);
        }
    
    template<typename...Types1, typename...Types2> inline
        bool operator>=(const tuple<Types1...>& t1, const tuple<Types2...>& t2){
            return !(t1 < t2);
        }
    
    // ------------------ create tuple functions -------------------------
    // make_tuple
    template<typename T> struct unwrap_refwrapper{
        typedef T type;
    };

    template<typename T>
        struct unwrap_refwrapper<std::reference_wrapper<T>>{
            using type = T&;
        };
    
    template<typename T>
        using make_tuple_return = typename unwrap_refwrapper<typename std::decay<T>::type>::type;

    template<typename...Types> inline
        auto make_tuple(Types&&... args) -> tuple<make_tuple_return<Types>...>{
            return tuple<make_tuple_return<Types>...>(forward<Types>(args)...);
        }

    // tie
    template<typename... Types> inline
        tuple<Types&...> tie(Types&... args){
            return tuple<Types&...>(args...);
        }

    // forward_as_tuple
    template<typename... Types> inline
        tuple<Types&&...> forward_as_tuple(Types&&... args){
            return tuple<Types&&...>(forward<Types>(args)...);
        }

    // ignore
    template<typename T> struct __ignore_t
    {
        template<typename U>
            __ignore_t& operator=(U&&) const { return *this; }
    };

    const __ignore_t<unsigned char> ignore = __ignore_t<unsigned char>();
}
#endif
