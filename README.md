learnSTL

implement some basic components of STL based on C++11 when learning C++ generic programming and STL. Reference to libc++, delete some relatively complicated part, and also add some useful data structure.

**Complier**: clang++3.8.0 and above.

- [ ] **memory**
    - [ ] pointer_traits
    - [ ] allocator, allocator_traits
    - [ ] smart pointer
- [x] **iterator**
    - [x] iterator_traits
    - [x] iterator adaptor
- [x] **utility**
    - [x] swap, swap_ranges, iter_swap
    - [x] move, forward
    - [x] rel_ops
    - [x] pair, make_pair
- [x] **tuple**
    - [x] tuple
    - [x] tuple_size, tuple_element, get
    - [x] make_tuple, tie, forward_as_tuple, ignore
- [ ] **algorithm**
    - [x] swap_ranges, for_each
    - [x] *range compare*
        - [x] equal, lexicographical_compare, mismatch
    - [x] *copy, move and assignment*
        - [x] copy, copy_if, copy_n, copy_backward
        - [x] move, move_backward
        - [x] fill, fill_n
        - [x] generate, generate_n
        - [x] transform
    - [x] *replace*
        - [x] replace, replace_copy, replace_if, replace_copy_if
    - [x] *remove*
        - [x] remove, remove_copy, remove_if, remove_copy_if
    - [x] *find*
        - [x] count, count_if (non_sorted)
        - [x] min, max, min_max, min_element, max_element, min_max (non_sorted)
        - [x] find, find_if, find_if_not  (non_sorted)
        - [x] adjacent_find  (non_sorted)
        - [x] lower_bound, upper_bound, equal_range, binary_search (sorted)
    - [x] *pattern-matching*
        - [x] search, search_n, find_end, find_first_of
        - [x] includes, all_of, any_of, none_of
    - [x] *mutaing*
        - [x] reverse, reverse_copy
        - [x] rotate, rotate_copy
        - [x] is_partition, partition, partition_copy, partition_pointer, stable_partition
        - [x] is_permutation, next_permutation, prev_permutation
    - [ ] *sorting*
    - [ ] *shuffle*
    - [x] *set (sorted)*
        - [x] merge
        - [x] set_union, set_intersection, set_difference, set_symmetric_difference
- [x] numeric
    - [x] iota
    - [x] accumulate, inner_product
    - [x] partial_sum, adjacent_difference
- [ ] **container**
   - [x] array
   - [x] vector
   - [ ] deque
   - [ ] forward_list
   - [ ] list
   - [ ] set
   - [ ] multiset
   - [ ] map
   - [ ] multimap
   - [ ] unordered_set
   - [ ] unordered_multiset
   - [ ] unordered_map
   - [ ] unordered_multimap
   - [ ] stack
   - [ ] queue
   - [ ] priority_queue
 
   
   







