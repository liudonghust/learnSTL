#include "iterator.h"

namespace learnSTL{    
	//iota      
	template<typename ForwardIter, typename T> inline
		void iota(ForwardIter first, ForwardIter last, T startVal)
	{
		for (; first != last; ++first, ++startVal)
			*first = startVal;
	}
	
    // accumulate
    template<typename InputIter, typename T, typename BinOP> inline
        T accumulate(InputIter first, InputIter last, T val, BinOP binary_op)
        {
            for(; first != last; ++first)
                val = binary_op(val, *first);
            return val;
        }

    template<typename InputIter, typename T> inline
        T accumulate(InputIter first, InputIter last, T val)
        {
            for(; first != last; ++first)
                val = val + *first;
            return val;
        }

    // inner_product
    template<typename InputIter1, typename InputIter2, typename T> inline
        T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T val)
        {
            for(; first1 != last1; ++first1, ++first2)
                val = val + (*first1 * (*first2));
            return val;
        }

    template<typename InputIter1, typename InputIter2, typename T, typename BinOP1, typename BinOP2> inline
        T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T val, BinOP1 binary_op1, BinOP2 binary_op2)
        {
            for(; first1 != last1; ++first1, ++first2)
                val = binary_op1(val, binary_op2(*first1, *first2));
            return val;
        }

    // partial_sum
    template<typename InputIter, typename OutputIter> inline
        OutputIter partial_sum(InputIter first, InputIter last, OutputIter result)
        {
            if(first != last){
                typename iterator_traits<InputIter>::value_type val(*first);
                *result = val;
                for(++first, ++result; first != last; ++first, ++result){
                    val = val + *first;
                    *result = val;
                }
            }
            return result;
        }

    template<typename InputIter, typename OutputIter, typename BinOP> inline
        OutputIter partial_sum(InputIter first, InputIter last, OutputIter result, BinOP binary_op)
        {
            if(first != last){
                typename iterator_traits<InputIter>::value_type val(*first);
                *result = val;
                for(++first, ++result; first != last; ++first, ++result){
                    val = binary_op(val, *first);
                    *result = val;
                }
            }
            return result;
        }

    // adjacent_difference
    template<typename InputIter, typename OutputIter, typename BinOP> inline
        OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result, BinOP binary_op)
        {
            if(first != last){
                typename iterator_traits<InputIter>::value_type val1(*first);
                *result = val1;
                for(++first, ++result; first != last; ++first, ++result){
                    typename iterator_traits<InputIter>::value_type val2(*first);
                    *result = binary_op(val2, val1);
                    val1 = std::move(val2);
                }
            }
            return result;
        }

    template<typename InputIter, typename OutputIter> inline
        OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter result)
        {
            if(first != last){
                typename iterator_traits<InputIter>::value_type val1(*first);
                *result = val1;
                for(++first, ++result; first != last; ++first, ++result){
                    typename iterator_traits<InputIter>::value_type val2(*first);
                    *result = val2 - val1;
                    val1 = std::move(val2);
                }
            }
            return result;
        
        }
}
