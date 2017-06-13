#ifndef MY_FORWARD_LIST_H
#define MY_FORWARD_LIST_H

#include <type_traits>
#include <initializer_list>
#include <memory>
#include <limits>
#include "iterator.h"
#include "utility.h"
#include "algorithm.h"

namespace learnSTL{
    template<typename NodePtr>
    struct __forward_head_node{
        NodePtr __next;

        __forward_head_node() : __next(nullptr) {}
    };

    template<typename T, typename VoidPtr> struct __forward_list_node;

    template<typename T, typename VoidPtr>
    struct __head_node_of{
        using __NodePtr = typename std::pointer_traits<VoidPtr>::template rebind<__forward_list_node<T, VoidPtr> >;
        using type = __forward_head_node<__NodePtr>;
    };


    template<typename T, typename VoidPtr>
    struct __forward_list_node : __head_node_of<T, VoidPtr>::type{
        using value_type = T;
        T __data;
    };

    template<typename T, typename Allocator> class forward_list;
    template<typename ConstNodePtr> class __forward_list_const_iterator;

    template<typename NodePtr>
    class __forward_list_iterator{
    public:
        using iterator_category = forward_iterator_tag;
        using value_type        = typename std::pointer_traits<NodePtr>::element_type::value_type;
        using difference_type   = typename std::pointer_traits<NodePtr>::difference_type;
        using reference         = value_type&;
        using pointer           = typename std::pointer_traits<NodePtr>:: template rebind<value_type>;
        
        __forward_list_iterator() : __ptr(nullptr) {}

        reference operator*() const { return __ptr->__data; }
        pointer operator->() const { return std::pointer_traits<pointer>::pointer_to(__ptr->__data); }

        __forward_list_iterator& operator++(){
            __ptr = __ptr->__next;
            return *this;
        }
        __forward_list_iterator& operator++(int){
            __forward_list_iterator temp(*this);
            __ptr = __ptr->__next;
            return temp;
        }
        friend bool operator==(const __forward_list_iterator& iter1, const __forward_list_iterator& iter2){
            return iter1.__ptr == iter2.__ptr;
        }
        friend bool operator!=(const __forward_list_iterator& iter1, const __forward_list_iterator& iter2){
            return iter1.__ptr != iter2.__ptr;
        }

    private:
        NodePtr __ptr;

        explicit __forward_list_iterator(NodePtr p) : __ptr(p) {}
        template<typename ,typename> friend class forward_list;      // only for forward_list
        template<typename> friend class __forward_list_const_iterator;
    };

    template<typename ConstNodePtr>
    class __forward_list_const_iterator{
        using __node_type = typename std::remove_const<typename std::pointer_traits<ConstNodePtr>::element_type>::type;
        using __NodePtr = typename std::pointer_traits<ConstNodePtr>:: template rebind<__node_type>;
    public:
        using iterator_category = forward_iterator_tag;
        using value_type        = typename __node_type::value_type;
        using difference_type   = typename std::pointer_traits<ConstNodePtr>::difference_type;
        using reference         = const value_type&;
        using pointer           = typename std::pointer_traits<ConstNodePtr>:: template rebind<const value_type>;
        
        __forward_list_const_iterator() : __ptr(nullptr) {}
        __forward_list_const_iterator(__forward_list_iterator<__NodePtr> iter) : __ptr(iter.__ptr) {}

        reference operator*() const { return __ptr->__data; }
        pointer operator->() const { return std::pointer_traits<pointer>::pointer_to(__ptr->__data); }
        
        __forward_list_const_iterator& operator++(){
            __ptr = __ptr->__next;
            return *this;
        }
        __forward_list_const_iterator& operator++(int){
            __forward_list_const_iterator temp(*this);
            __ptr = __ptr->__next;
            return temp;
        }

        friend bool operator==(const __forward_list_const_iterator& iter1, const __forward_list_const_iterator iter2){
            return iter1.__ptr == iter2.__ptr;
        }
        friend bool operator!=(const __forward_list_const_iterator& iter1, const __forward_list_const_iterator iter2){
            return iter1.__ptr != iter2.__ptr;
        }
        
    private:
        ConstNodePtr __ptr;

        explicit __forward_list_const_iterator(ConstNodePtr p) : __ptr(p) {}
        template<typename, typename> friend class forward_list; 
    };

    template<typename T, typename Allocator=std::allocator<T> >
    class __forward_list_base{
    protected:
        using value_type = T;
        using allocator_type = Allocator;

        using void_pointer     = typename std::allocator_traits<allocator_type>::void_pointer;
        using __node_type      = __forward_list_node<T, void_pointer>;
        using __head_node_type = typename __head_node_of<T, void_pointer>::type;
        using __node_alloc     = typename std::allocator_traits<allocator_type>::template rebind_alloc<__node_type>;
        using __node_traits    = std::allocator_traits<__node_alloc>;
        using __node_pointer   = typename __node_traits::pointer;
        using __head_node_alloc = typename std::allocator_traits<allocator_type>::template rebind_alloc<__head_node_type>;
        using __head_node_pointer = typename std::allocator_traits<__head_node_alloc>::pointer;

        using iterator       = __forward_list_iterator<__node_pointer>;
        using const_iterator = __forward_list_const_iterator<__node_pointer>;

        __head_node_type __head;
        __node_alloc __alloc;

        __node_pointer __before_begin(){
            return static_cast<__node_pointer>(std::pointer_traits<__head_node_pointer>::pointer_to(__head));
        }

        __node_pointer __before_begin() const {
            return static_cast<__node_pointer>(std::pointer_traits<__head_node_pointer>::pointer_to(const_cast<__head_node_type&>(__head)));
        }

        // constructor
        __forward_list_base() : __head(), __alloc() {}
        __forward_list_base(const allocator_type& a) : __head(), __alloc(a) {}

        void clear(){
            for(__node_pointer p = __before_begin()->__next; p != nullptr;){
                __node_pointer nextPtr = p->__next;
                __node_traits::destroy(__alloc, std::addressof(p->__data));
                __node_traits::deallocate(__alloc, p, 1);
                p = nextPtr;
            }
            __before_begin()->__next = nullptr;
        }

        ~__forward_list_base(){
            clear();
        }

        void __copy_assign_alloc(const __forward_list_base& f){
            __copy_assign_alloc(f, std::integral_constant<bool, __node_traits::propagate_on_container_copy_assignment::value>());
        }

        void __move_assign_alloc(__forward_list_base& rf){
            __move_assign_alloc(rf, std::integral_constant<bool, __node_traits::propagate_on_container_move_assignment::value>());
        }

        void swap(__forward_list_base& f2){
            __swap_alloc(__alloc, f2.__alloc, std::integral_constant<bool, __node_traits::propagate_on_container_swap::value>());
            learnSTL::swap(__before_begin()->__next, (f2.__before_begin())->__next);
        }
        
    private:
        void __copy_assign_alloc(const __forward_list_base& f, std::true_type){
            if(__alloc != f.__alloc)
                clear();
            __alloc = f.__alloc;
        }
        void __copy_assign_alloc(const __forward_list_base&, std::false_type) {}

        void __move_assign_alloc(__forward_list_base& rf, std::true_type){
            // clear is left to the forward_list
            __alloc = learnSTL::move(rf.__alloc);
        }
        void __move_assign_alloc(__forward_list_base&, std::false_type) {}

        void __swap_alloc(__node_alloc& a1, __node_alloc& a2, std::true_type){
            swap(a1, a2);
        }
        void __swap_alloc(__node_alloc&, __node_alloc, std::false_type) {}
    };

    // ------ forward_list -----------
    template<typename T, typename Allocator = std::allocator<T> >
    class forward_list : private __forward_list_base<T, Allocator>
    {
        using base = __forward_list_base<T, Allocator>;
        using __node_type = typename base::__node_type;
        using __node_pointer = typename base::__node_pointer;
        using __node_alloc = typename base::__node_alloc;
        using __node_traits = typename base::__node_traits;
    public:
        using value_type       = T;
        using allocator_type   = Allocator;
        using size_type        = typename std::allocator_traits<allocator_type>::size_type;
        using difference_type  = typename std::allocator_traits<allocator_type>::difference_type;
        using reference        = value_type&;
        using const_reference  = const value_type&;
        using pointer          = typename std::allocator_traits<allocator_type>::pointer;
        using const_pointer    = typename std::allocator_traits<allocator_type>::const_pointer;
        
        using iterator = typename base::iterator;
        using const_iterator = typename base::const_iterator;

        // iterator
        iterator before_begin() { return iterator(base::__before_begin()); }
        const_iterator before_begin() const { return const_iterator(base::__before_begin()); }
        const_iterator cbefore_begin() const { return const_iterator(base::__before_begin()); }

        iterator begin() { return iterator(base::__before_begin()->__next); }
        const_iterator begin() const { return const_iterator(base::__before_begin()->__next); }
        const_iterator cbegin() const { return const_iterator(base::__before_begin()->__next); }

        iterator end() { return iterator(nullptr); }
        const_iterator end() const { return const_iterator(nullptr); }
        const_iterator cend() const { return const_iterator(nullptr); }

        allocator_type get_allocator() const { return this->__alloc; }

        reference front() {
            return base::__before_begin()->__next->__data;
        }

        const_reference front() const {
            return base::__before_begin()->__next->__data;
        }

        // empty
        bool empty() const {
            return base::__before_begin()->__next == nullptr;
        }
        // max_size
        size_type max_size() const {
            return std::numeric_limits<size_type>::max();
        }

        // resize
        void resize(size_type n){
            iterator bb = before_begin();
            iterator b = begin();
            iterator e = end();
            size_type sz = 0;
            for(; b != e && sz < n; ++bb, ++b, ++sz);
            if(b != e)       // size() > count
                erase_after(bb, e);
            else{
                n -= sz;
                insert_after(bb, n, value_type());
            }
        }

        void resize(size_type n, const_reference val){
            iterator bb = before_begin();
            iterator b = begin();
            iterator e = end();
            size_type sz = 0;
            for(; b != e && sz < n; ++bb, ++b, ++sz);
            if(b != e)       // size() > count
                erase_after(bb, e);
            else{
                n -= sz;
                insert_after(bb, n, val);
            }
        }

        // constructor
        forward_list() = default;     // default
        explicit forward_list(const allocator_type& a) : base(a) {}

        explicit forward_list(size_type n){
            insert_after(before_begin(), n, value_type());
        }
        forward_list(size_type n, const allocator_type& a) : base(a)
        {
            insert_after(before_begin(), n, value_type());
        }

        forward_list(size_type n, const_reference val){
            insert_after(before_begin(), n, val);
        }
        forward_list(size_type n, const_reference val, const allocator_type& a) : base(a)
        {
            insert_after(before_begin(), n, val);
        }

        template<typename InputIter>
            forward_list(InputIter first, typename std::enable_if<__is_input_iterator<InputIter>::value, InputIter>::type last){
               insert_after(before_begin(), first, last); 
            }
         template<typename InputIter>
            forward_list(InputIter first, 
                    typename std::enable_if<__is_input_iterator<InputIter>::value, InputIter>::type last, const allocator_type& a)
            : base(a) {
                insert_after(before_begin(), first, last);
            }

        forward_list(std::initializer_list<value_type> initlist){
            insert_after(before_begin(), initlist.begin(), initlist.end());
        }
        forward_list(std::initializer_list<value_type> initlist, const allocator_type& a) : base(a)
        {
            insert_after(before_begin(), initlist.begin(), initlist.end());
        }

        // copy-constructor
        forward_list(const forward_list& f2) 
            : base(__node_traits::select_on_container_copy_construction(f2.__alloc))
        {
            insert_after(before_begin(), f2.begin(), f2.end());
        }
        forward_list(const forward_list& f2, const allocator_type& a) : base(a)
        {
            insert_after(before_begin(), f2.begin(), f2.end());
        }

        // move-xonstructor
        forward_list(forward_list&& rf) : base(learnSTL::move(rf.__alloc))
        {
            base::__before_begin()->__next = rf.__before_begin()->__next;   
            rf.__before_begin()->__next = nullptr;
        }
        forward_list(forward_list&& rf, const allocator_type& a) : base(a)
        {
            if(this->__alloc != rf.__alloc){
                using moveIter = move_iterator<iterator>;
                insert_after(before_begin(), moveIter(rf.begin()), moveIter(rf.end()));
            }
        }

        // assignment
        forward_list& operator=(const forward_list& f2){
            if(this == &f2)
                return *this;
            base::__copy_assign_alloc(f2);
            assign(f2.begin(), f2.end());
        }

        forward_list& operator=(forward_list&& rf){
            __move_assign(rf, std::integral_constant<bool, __node_traits::propagate_on_container_move_assignment::value>());
            return *this;
        }

        forward_list& operator=(std::initializer_list<value_type> initlist){
            assign(initlist.begin(), initlist.end());
            return *this;
        }

        void assign(size_type n, const_reference val){
            iterator bb = before_begin();
            iterator b = begin();
            iterator e = end();
            for(; b != e && n > 0; --n, ++b, ++bb)
                *b = val;
            if(b != e)   // size() > n
                erase_after(bb, e);
            if(n > 0)
                insert_after(bb, n, val);
        }

        template<typename InputIter>
        void assign(InputIter first, typename std::enable_if<__is_input_iterator<InputIter>::value, InputIter>::type last)
        {
            iterator bb = before_begin();
            iterator b = begin();
            iterator e = end();
            for(; b != e && first != last; ++bb, ++b, ++first)
                *b = *first;
            if( b != e) // size() > (last - first)
                erase_after(bb, e);
            if( first != last) 
                insert_after(bb, first, last);
        }

        void assign(std::initializer_list<value_type> initlist){
            assign(initlist.begin(), initlist.end());
        }

        // push_front
        void push_front(const_reference val){
            std::unique_ptr<__node_type> node_up(__node_traits::allocate(this->__alloc, 1));
            __node_traits::construct(this->__alloc, std::addressof(node_up->__data), val);
            node_up->__next = base::__before_begin()->__next;
            base::__before_begin()->__next = node_up.release();
        }

        void push_front(value_type&& val){
            std::unique_ptr<__node_type> node_up(__node_traits::allocate(this->__alloc, 1));
            __node_traits::construct(this->__alloc, std::addressof(node_up->__data), move(val));
            node_up->__next = base::__before_begin()->__next;
            base::__before_begin()->__next = node_up.release();
        }

        template<typename ...Args>
            void emplace_front(Args&&... args){
                std::unique_ptr<__node_type> node_up(__node_traits::allocate(this->__alloc, 1));
                __node_traits::construct(this->__alloc, std::addressof(node_up->__data), learnSTL::forward<Args>(args)...);
                node_up->__next = base::__before_begin()->__next;
                base::__before_begin()->__next = node_up.release();
            }

        // insert_after
        iterator insert_after(const_iterator pos, const_reference val){
            __node_pointer p = pos.__ptr;
            std::unique_ptr<__node_type> node_up(__node_traits::allocate(this->__alloc, 1));
            __node_traits::construct(this->__alloc, std::addressof(node_up->__data), val);
            node_up->__next = p->__next;
            p->__next = node_up.release();
            return iterator(p->__next);
        }

        iterator insert_after(const_iterator pos, value_type&& rval){
            __node_pointer p = pos.__ptr;
            std::unique_ptr<__node_type> node_up(__node_traits::allocate(this->__alloc, 1));
            __node_traits::construct(this->__alloc, std::addressof(node_up->__data), learnSTL::move(rval));
            node_up->__next = p->__next;
            p->__next = node_up.release();
            return iterator(p->__next);
        }

        iterator insert_after(const_iterator pos, size_type n, const_reference val){
            __node_pointer p = pos.__ptr;
            if(n > 0){
                std::unique_ptr<__node_type> node_up(__node_traits::allocate(this->__alloc, 1));
                __node_traits::construct(this->__alloc, std::addressof(node_up->__data), val);
                __node_pointer first = node_up.release();
                __node_pointer last = first;
                for(--n; n > 0; --n, last = last->__next){
                    node_up.reset(__node_traits::allocate(this->__alloc, 1));
                    __node_traits::construct(this->__alloc, std::addressof(node_up->__data), val);
                    last->__next = node_up.release();
                }
                last->__next = p->__next;
                p->__next = first;
                p = last;
            }
            return iterator(p);
        }

        template<typename InputIter>
            typename std::enable_if<__is_input_iterator<InputIter>::value, iterator>::type 
            insert_after(const_iterator pos, InputIter first, InputIter last){
                __node_pointer p = pos.__ptr;
                if(first != last){
                    std::unique_ptr<__node_type> node_up(__node_traits::allocate(this->__alloc, 1));
                    __node_traits::construct(this->__alloc, std::addressof(node_up->__data), *first);
                    __node_pointer first_node_p = node_up.release();
                    __node_pointer last_node_p = first_node_p;
                    for(++first; first != last; ++first, last_node_p = last_node_p->__next){
                        node_up.reset(__node_traits::allocate(this->__alloc, 1));
                        __node_traits::construct(this->__alloc, std::addressof(node_up->__data), *first);
                        last_node_p->__next = node_up.release();
                    }
                    last_node_p->__next = p->__next;
                    p->__next = first_node_p;
                    p = last_node_p;
                }
                return iterator(p);
            }

        iterator insert_after(std::initializer_list<value_type> initlist){
            return insert_after(initlist.begin(), initlist.end());
        }

        // emplace_after
        template<typename ...Args>
            iterator emplace_after(const_iterator pos, Args&&... args){
                __node_pointer p = pos.__ptr;
                std::unique_ptr<__node_type> node_up(__node_traits::allocate(this->__alloc, 1));
                __node_traits::construct(this->__alloc, std::addressof(node_up->__data), learnSTL::forward<Args>(args)...);
                node_up->__next = p->__next;
                p->__next = node_up.release();
                return iterator(p->__next);
            }

        // clear()
        void clear(){
            base::clear();
        }

        // pop_front()
        void pop_front(){
            __node_pointer firstPtr = base::__before_begin()->__next;
            base::__before_begin()->__next = firstPtr->__next;
            __node_traits::destroy(this->__alloc, std::addressof(firstPtr->__data));
            __node_traits::deallocate(this->__alloc, firstPtr, 1);
        }

        // erase_after
        iterator erase_after(const_iterator pos){
            __node_pointer p = pos.__ptr;
            __node_pointer nextPtr = p->__next;
            p->__next = nextPtr->__next;
            __node_traits::destroy(this->__alloc, std::addressof(nextPtr->__data));
            __node_traits::deallocate(this->__alloc, nextPtr, 1);
            return iterator(p->__next);
        }

        iterator erase_after(const_iterator first, const_iterator last){
            __node_pointer e = last.__ptr;
            if(first != last){
                __node_pointer p = first.__ptr;
                __node_pointer nextPtr = p->__next;
                if(nextPtr != e){
                    // modify the link
                    p->__next = e;
                    // deallocate
                    do{
                        p = nextPtr->__next;
                        __node_traits::destroy(this->__alloc, std::addressof(nextPtr->__data));
                        __node_traits::deallocate(this->__alloc, nextPtr, 1);
                        nextPtr = p;
                    }while(nextPtr != e);
                }
            }
            return iterator(e);
        }

        void swap(forward_list& f2){
            base::swap(f2);
        }

        // merge
        template<typename Compare>
        void merge(forward_list& f2, Compare comp)
        {
            this->__before_begin()->__next =
                __merge(this->__before_begin()->__next,
                        f2.__before_begin()->__next,
                        comp);
            f2.__before_begin()->__next = nullptr;
        }

        template<typename Compare>
        void merge(forward_list&& f2, Compare comp){
            merge(f2, std::move(comp));         // using mve semantics to avoid copy comp
        }

        void merge(forward_list& f2){
            merge(f2, learnSTL::__less<value_type>());
        }

        void merge(forward_list&& f2){
            merge(f2, learnSTL::__less<value_type>());
        }

        // splice_after
        void splice_after(const_iterator pos, forward_list& f2){
            if(this != &f2){
                if(!f2.empty()){
                    __node_pointer p = pos.__ptr;
                    if(p->__next != nullptr){
                        // find the last elem in f2
                        __node_pointer p2 = f2.__before_begin()->__next;
                        while(p2->__next != nullptr)
                            p2 = p2->__next;
                        p2->__next = p->__next;
                    }
                    p->__next = f2.__before_begin()->__next;
                    f2.__before_begin()->__next = nullptr;
                }
            }
        }

        void splice_after(const_iterator pos, forward_list&& f2){
            splice_after(pos, f2);
        }

        void splice_after(const_iterator pos, forward_list& f2, const_iterator pos2){
            if(pos != pos2 && pos != learnSTL::next(pos2) && learnSTL::next(pos2) != f2.end()){
                __node_pointer n = pos2.__ptr->__next;
                pos2.__ptr->__next = n->__next;
                n->__next = pos.__ptr->__next;
                pos.__ptr->__next = n;
            }
        }

        void splice_after(const_iterator pos, forward_list&& f2, const_iterator pos2){
            splice_after(pos, f2, pos2);
        }

        void splice_after(const_iterator pos, forward_list& f2, const_iterator first, const_iterator last){
            // assume pos in not in range (first, last)
            if(first != last && pos != first){
                // find previous node of last
                __node_pointer p = first.__ptr;
                while(p->__next != last.__ptr)
                    p = p->__next;
                if(first.__ptr != p)  // first->next = last, no elem to move
                {
                    p->__next = pos.__ptr->__next;
                    pos.__ptr->__next = first.__ptr->__next;
                    first.__ptr->__next = last.__ptr;   // relink in f2;
                }
            }
        }
        void splice_after(const_iterator pos, forward_list&& f2, const_iterator first, const_iterator last){
            splice_after(pos, f2, first, last);
        }

        // remove
        void remove(const_reference val){
            auto e = end();
            for(auto iter1 = before_begin(); learnSTL::next(iter1) != e; ){
                if(*learnSTL::next(iter1) == val){
                    auto iter2 = iter1;
                    learnSTL::advance(iter2, 2);
                    for(; iter2 != e && *iter2 == val; ++iter2);
                    erase_after(iter1, iter2);
                    if(iter2 == e)
                        break;
                    iter1 = iter2;
                }
                else
                    ++iter1;
            }
        }

        // remove_if
        template<typename UnaryPred>
            void remove_if(UnaryPred pred){
                auto e = end();
                for(auto iter1 = before_begin(); learnSTL::next(iter1) != e; ){
                    if(pred(*learnSTL::next(iter1))){
                        auto iter2 = iter1;
                        learnSTL::advance(iter2, 2);
                        for(; iter2 != e && pred(*iter2); ++iter2);
                        erase_after(iter1, iter2);
                        if(iter2 == e)
                            break;
                        iter1 = iter2;
                    }
                    else
                        ++iter1;
                }
            }

        // unique
        template<typename BinPred>
            void unique(BinPred binaryPred){
                for(auto i = begin(), e = end(); i != e; ){
                    auto j = learnSTL::next(i);
                    for(; j != e && binaryPred(*i, *j); ++j);
                    erase_after(i, j);
                    i = j;
                }
            }

        void unique(){
            unique(__equal_to<value_type>());
        }

        // reverse
        void reverse(){
            __node_pointer f = base::__before_begin()->__next;
            if(f){
                __node_pointer n = f->__next;
                f->__next = nullptr;
                while(n){
                    __node_pointer r = n->__next;
                    n->__next = f;
                    f = n;
                    n = r;
                }
                base::__before_begin()->__next = f;
            }
        }

        // sort
        template<typename Compare>
            void sort(Compare comp){
                base::__before_begin()->__next = 
                    __sort(base::__before_begin()->__next, learnSTL::distance(begin(), end()), comp);
            }

        void sort(){
            sort(__less<value_type>());
        }

        ~forward_list() = default;
    private:
        void __move_assign(forward_list& f2, typename std::true_type){
            clear();
            base::__move_assign_alloc(f2);
            base::__before_begin()->__next = f2.__before_begin()->__next;
            f2.__before_begin()->__next = nullptr;
        }
        void __move_assign(forward_list& f2, typename std::false_type) {
            if(f2.__alloc == this->__alloc)
                __move_assign(f2, std::true_type());
            else{
                using moveIter = move_iterator<iterator>;
                assign(moveIter(f2.begin()), moveIter(f2.end()));
            }
        }
        
        template<typename Compare>
        __node_pointer __merge(__node_pointer f1, __node_pointer f2, Compare& comp)
        {
            if(f1 == nullptr)
                return f2;
            if(f2 == nullptr)
                return f1;
            __node_pointer ret = nullptr;
            // step 1
            if(comp(f2->__data, f1->__data)){
                __node_pointer t2 = f2;
                while(t2->__next && comp(t2->__next->__data, f1->__data)) // traverse f2
                    t2 = t2->__next;
                ret = f2;
                f2 = t2->__next;    // store the first elem bigger than f1
                t2->__next = f1;
            }
            else // first elem in f2 bigger than f1
                ret = f1;
            // step 2, 3, 4, 5
            __node_pointer t1 = f1->__next; // step 2
            while(t1 != nullptr && f2 != nullptr){
                if(comp(f2->__data, t1->__data)){  // step 3 found
                    __node_pointer t2 = f2;
                    while(t2->__next && comp(t2->__next->__data, t1->__data))
                        t2 = t2->__next;
                    // step 4 relink
                    f1->__next = f2;  // merge f2 to f1;
                    f2 = t2->__next;  // store the first elem bigger than t1
                    t2->__next = t1;
                }
                // step 3 not found
                f1 = t1;
                t1 = t1->__next;
                // step 5 repeat
            }
            if(f2 != nullptr)   // step 6
                f1->__next = f2;
            return ret;
        }

        template<typename Compare>
            __node_pointer __sort(__node_pointer f1, difference_type sz, Compare& comp)
            {
                switch(sz){    // base case
                case 0:
                case 1:
                    return f1;
                case 2:
                    if(comp(f1->__next->__data, f1->__data)){
                        __node_pointer temp = f1->__next;
                        temp->__next = f1;
                        f1->__next = nullptr;
                        f1 = temp;
                    }
                    return f1;
                }

                auto sz1 = sz / 2;
                auto sz2 = sz - sz1;
                auto e1 = f1;
                // find end in left_half
                for(auto n = sz1 -1; n > 0; e1 = e1->__next, --n);
                __node_pointer f2 = e1->__next;
                e1->__next = nullptr;
                return __merge(__sort(f1, sz1, comp), __sort(f2, sz2, comp), comp);
            }
    };

    // swap
    template<typename T, typename Alloc>
        void swap(forward_list<T, Alloc>& f1, forward_list<T, Alloc>& f2){
            f1.swap(f2);
        }

    // compare
    template<typename T, typename Alloc>
        bool operator==(const forward_list<T, Alloc>& f1, const forward_list<T, Alloc>& f2)
        {
            auto b1 = f1.begin();
            auto e1 = f1.end();
            auto b2 = f2.begin();
            auto e2 = f2.end();
            for(; b1 != e1 && b2 != e2; ++b1, ++b2)
                if(*b1 != *b2)
                    return false;
            return (b1 == e1) == (b2 == e2);
        }

    template<typename T, typename Alloc>
        bool operator!=(const forward_list<T, Alloc>& f1, const forward_list<T, Alloc>& f2){
            return !(f1 == f2);
        }

    template<typename T, typename Alloc>
        bool operator<(const forward_list<T, Alloc>& f1, const forward_list<T, Alloc>& f2)
        {
            return learnSTL::lexicographical_compare(f1.begin(), f1.end(), f2.begin(), f2.end());
        }

    template<typename T, typename Alloc>
        bool operator>(const forward_list<T, Alloc>& f1, const forward_list<T, Alloc>& f2)
        {
            return f2 < f1;
        }

    template<typename T, typename Alloc>
        bool operator<=(const forward_list<T, Alloc>& f1, const forward_list<T, Alloc>& f2)
        {
            return !(f2 < f1);
        }

    template<typename T, typename Alloc>
        bool operator>=(const forward_list<T, Alloc>& f1, const forward_list<T, Alloc>& f2)
        {
            return !(f1 < f2);
        }
}
#endif
