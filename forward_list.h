#ifndef MY_FORWARD_LIST_H
#define MY_FORWARD_LIST_H

#include <type_traits>
#include <initializer_list>
#include <memory>
#include "iterator.h"

namespace learnSTL{
    template<typename NodePtr>
    struct __forward_head_node{
        NodePtr __next;

        __forward_head_node() : __next(nullptr) {}
    };

    template<typename T, typename VoidPtr> struct __forward_list_node;

    template<typename T, typename VoidPtr>
    struct __head_node_of{
        typedef typename std::pointer_traits<VoidPtr>::template rebind<__forward_list_node<T, VoidPtr> > nodePtr;
        typedef __forward_head_node<nodePtr> type;
    };


    template<typename T, typename VoidPtr>
    struct __forward_list_node : __head_node_of<T, VoidPtr>::type{
        typedef T value_type;
        T __data;
    };

    template<typename T, typename Allocator> class forward_list;
    template<typename ConstNodePtr> class __forward_list_const_iterator;

    template<typename NodePtr>
    class __forward_list_iterator{
    public:
        typedef forward_iterator_tag                                                iterator_category;
        typedef typename std::pointer_traits<NodePtr>::element_type::value_type     value_type;
        typedef value_type&                                                         reference;
        typedef typename std::pointer_traits<NodePtr>::difference_type              difference_type;
        typedef typename std::pointer_traits<NodePtr>::template rebind<value_type>  pointer;
        
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
        typedef typename std::remove_const<typename std::pointer_traits<ConstNodePtr>::element_type>::type __node;
        typedef typename std::pointer_traits<ConstNodePtr>::template rebind<__node> NodePtr;
    public:
        typedef forward_iterator_tag                                                           iterator_category;
        typedef typename __node::value_type                                                    value_type;
        typedef typename std::pointer_traits<ConstNodePtr>::difference_type                    difference_type;
        typedef const value_type&                                                              reference;
        typedef typename std::pointer_traits<ConstNodePtr>::template rebind<const value_type>  pointer;
        
        __forward_list_const_iterator() : __ptr(nullptr) {}
        __forward_list_const_iterator(__forward_list_iterator<NodePtr> iter) : __ptr(iter.__ptr) {}

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
        friend bool operator==(const __forward_list_const_iterator& iter1, const __forward_list_const_iterator iter2){
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
        typedef T value_type;
        typedef Allocator allocator_type;

        typedef typename std::allocator_traits<allocator_type>::void_pointer void_pointer;
        typedef __forward_list_node<T, void_pointer> __node;
        typedef typename __head_node_of<T, void_pointer>::type __head_node;
        typedef typename std::allocator_traits<allocator_type>::template rebind_alloc<__node>::type __node_alloc;
        typedef std::allocator_traits<__node_alloc> __node_traits;
        typedef typename __node_traits::pointer __node_pointer;

        typedef __forward_list_iterator<__node_pointer>           iterator;
        typedef __forward_list_const_iterator<__node_pointer>     const_iterator;
    
        __head_node __head;
        __node_alloc __alloc;

        // constructor
        __forward_list_base() : __head(), __alloc() {}
        __forward_list_base(const allocator_type& a) : __head(), __alloc(a) {}
    
        void clear(){
            for(__node_pointer p = __head->__next; p != nullptr; p = p->__next){
                __node_traits::destroy(__alloc, std::addressof(p->__data));
                __node_traits::deallocate(__alloc, p, 1);
            }
            __head->__next = nullptr;
        }

        ~__forward_list_base(){
            clear();
        }

    };

    // ------ forward_list -----------
    template<typename T, typename Allocator = std::allocator<T> >
    class forward_list : private __forward_list_base<T, Allocator>
    {
        typedef __forward_list_base<T, Allocator> base;
        typedef typename base::__node_alloc __node_alloc;
        typedef typename base::__node_traits __node_traits;
        typedef typename base::__node_pointer __node_pointer;
        typedef typename base::__node __node;
    public:
        typedef T                                                                 value_type;
        typedef Allocator                                                         allocator_type;
        typedef typename std::allocator_traits<allocator_type>::size_type         size_type;
        typedef typename std::allocator_traits<allocator_type>::difference_type   difference_type;
        typedef value_type&                                                       reference;
        typedef const value_type&                                                 const_reference;
        typedef typename std::allocator_traits<allocator_type>::pointer           pointer;
        typedef typename std::allocator_traits<allocator_type>::const_pointer     const_pointer;
        
        typedef typename base::iterator            iterator;
        typedef typename base::const_iterator      const_iterator;

        // iterator
        iterator before_begin() { return iterator(this->__head); }
        const_iterator before_begin() const { return const_iterator(this->__head); }
        const_iterator cbefore_begin() const { return const_iterator(this->__head); }

        iterator begin() { return iterator(this->__head->__next); }
        const_iterator begin() const { return const_iterator(this->__head->__next); }
        const_iterator cbegin() const { return const_iterator(this->__head->__next); }

        iterator end() { return iterator(nullptr); }
        const_iterator end() const { return const_iterator(nullptr); }
        const_iterator cend() const { return const_iterator(nullptr); }

        allocator_type get_allocator() const { return this->__alloc; }

        // constructor
        forward_list() {}     // default
        explicit forward_list(const allocator_type& a) : base(a) {}

        forward_list(size_type n){
            insert_after()
        }



        // insert_after
        iterator insert_after(const_iterator pos, size_type n, const_reference val){
            if(n > 0){
                __node_pointer p = pos.__ptr;
                for(; n > 0;)
            }
        }
    };
}
#endif
