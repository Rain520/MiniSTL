#ifndef __MY_LIST_
#define __MY_LIST_

#include "my_allocator.h"
#include "my_iterator.h"
#include <cstddef>
#include <utility>
namespace jan {

  /**
  * @brief 双向链表的节点
  *
  * @tparam T
  */
  template <typename T>
  struct _list_node {
    using self = _list_node<T>;
    T _data;
    self *_pre;
    self *_next;
    template <typename ... Args>
    explicit _list_node(Args && ... args, self * pre, self * next) : _data(std::forward<Args>(args)...),
      _pre(pre), _next(next)
    { }

    explicit _list_node(const T &val = T(), self *pre = nullptr,
                        self *next = nullptr)
        : _data(val), _pre(pre), _next(next) {}
  };

  /**
   * @brief 双向链表的迭代器, 此迭代器种类为双向迭代器,支持operator->
   * 
   * @tparam T 
  */
  template <typename T> 
  struct _list_iterator {
    using value_type = T;
    using pointer = T *;
    using receference = T &;
    using size_type = size_t;
    using iterator_category = bidirectional_iterator_tag;
    using self = _list_iterator<T>;
    using node_type = _list_node<T>;
    using difference_type = ptrdiff_t;
    T &operator*() { return (*_node)._data; }
    T *operator->() { return &*_node; }
    bool operator==(const self &rhs) { return _node == rhs._node; }
    bool operator!=(const self &rhs) { return _node != rhs._node; }
    self &operator++() {
      _node = _node->_next;
      return *this;
    }
    self operator++(int) {
      auto ret = *this;
      ++*this;
      return ret;
    }
    self &operator--() {
      _node = _node->_pre;
      return *this;
    }
    self operator--(int) {
      auto ret = *this;
      --*this;
      return ret;
    }
    node_type *_node;
    explicit _list_iterator(node_type *node = nullptr) : _node(node) {}
    _list_iterator(const self &iteraotr) : _list_iterator(iteraotr._node) {}
  };

  /**
   * @brief 双向链表容器, O(1)的插入和删除，基于双向环状链表组织
   * 
   * @tparam T 
   * @tparam Alloc 
   */
  template <typename T, typename Alloc = jan::alloc> 
  class list {
  private:
    using node_type = _list_node<T>;
  public:

    list() 
    {
      _base_node = creat_node(T{},_base_node,_base_node);
    }

    using iterator = _list_iterator<T>;
    using size_type = size_t;
    iterator begin() const { return iterator(_base_node->_next); }
    iterator end() const { return iterator(_base_node); }
    bool empty() const { return _base_node->_next == _base_node; }
    size_type size() const { return distance(begin(), end()); }
    T & front() { return *begin(); }
    T & back() { return *(--end()); }

    iterator insert(iterator pos, const T & val);
    void push_front(const T & val) { insert(begin(), val); }
    void push_back(const T & val) { auto tmp = end(); insert(--tmp, val); }

  protected:
    //此节点为基点，也是尾后节点
    using data_alloc = alloc_adapter<node_type, Alloc>;
    //传回一个节点大小内存空间
    node_type * get_node() { return data_alloc::allocate(); }
    void put_node(node_type * p) { data_alloc::deallocate(p); }
    template <typename ... Args>
    node_type * creat_node(Args && ... args,node_type * pre, node_type * next) {
      auto ret = get_node();
      construct(ret,std::forward<Args>(args)...,pre,next);
      return ret;
    }
    void destroy_node(node_type * p)
    {
      destroy_node(&(p->_data));
      put_node(p);
    }
    
    node_type * _base_node;
    
  };
   
  template <typename T, typename Alloc>
  typename list<T,Alloc>::iterator
  list<T,Alloc>::insert(iterator pos, const T &val)
  {
    auto node = creat_node(val, pos._node->_pre, pos._node);
    pos._node->pre->next = node;
    pos._node->pre = node;
    return node;
  }

} // namespace jan


#endif
