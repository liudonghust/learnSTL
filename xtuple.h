#ifndef MY_XTUPLE_H
#define MY_XTUPLE_H
#include <cstddef>

namespace learnSTL{
    // tuple_size
    template<typename Tuple> class tuple_size;  

    // tuple_element
    template<size_t Index, typename Tuple> class tuple_element;   // gerneral template

}

#endif
