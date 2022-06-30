#ifndef __MY_DEQUE_H_
#define __MY_DEQUE_H_

#include "my_iterator.h"
#include <cstddef>
namespace jan {

template <typename T, typename Ref, typename Ptr, size_t BufSize>
struct __deuqe_iterator
{
  using iterator_category = random_access_iterator_tag;
  using iterator = __deuqe_iterator<T, T&, T*, BufSize>;
  using const_iterator = __deuqe_iterator<T, const T &, const T *, BufSize>;
  using map_pointer = T**;
  using value_type = T;
  using pointer = Ptr;
  using receference = Ref;
  using difference_type = ptrdiff_t;
  using size_type = size_t;

  //当前指向的数据
  pointer cur;
  //指向缓冲区起始位置
  pointer first;
  //指向缓冲区的尾后位置，such as end()
  pointer last;

  //指向中控,中控是一个array with pointer so, type of node is map_pointer(T**)
  map_pointer node;

  /**
   * @brief 目的为了使迭代器跳过一个缓冲区, new_node 为要跳到哪个缓冲区
   * 
   * @param new_node 
   */
  void set_node(map_pointer new_node)
  {
    node = new_node;
    first = *node;
    last = first + buffer_size();
  }

  receference operator*() { return *cur; }
  pointer operator->() { return &(operator*()); }

  /***对于deuqe来说一些基本的操作***/
  iterator & operator++()
  {
    ++cur;
    if(cur == last)
    {
      set_node(++node);
      cur = first;
    }
    return *this;
  }

  iterator operator++(int)
  {
    auto ret = *this;
    ++*this;
    return ret;
  }

  iterator & operator--()
  {
    if(cur == first)
    {
      set_node(--node);
      cur = last - 1;
    }
    return *this;
  }

  iterator operator--(int)
  {
    auto ret = *this;
    ++*this;
    return ret;
  }

  /**
   * @brief 返回两个迭代器之间的距离
   * 
   * @param rhs 
   * @return difference_type 
   */
  difference_type operator-(const iterator & rhs) const
  {
    return static_cast<difference_type>(buffer_size()) * (node - rhs.node - 1)
     + (cur - first) + (last - cur);
  }

  /**
   * @brief 迭代器的随机访问
   * 
   * @param n 
   * @return iterator& 
   */
  iterator & operator+=(difference_type n)
  {
  }
  static size_t buffer_size();
};

  
/**
 * @brief 计算缓冲区大小，单位为能容纳多少个元素，如果用户提供了缓冲区大小，使用用户的缓冲区大小，
 *        否则，返回512 / sizeof(T) 向上取整,或者是1，如果sizeof(T) < 512
 *       
 * 
 * @tparam T 
 * @tparam Ref 
 * @tparam Ptr 
 * @tparam BufSize 
 * @return size_t 
 */
template <typename T, typename Ref, typename Ptr, size_t BufSize>
size_t __deuqe_iterator<T, Ref, Ptr, BufSize>::buffer_size()
{
  if(BufSize != 0)
    return BufSize;
  else
  {
    auto value_size = sizeof(T);
    if(value_size < 512)
      return static_cast<size_type>(512 / value_size) + 1;
    else
      return static_cast<size_type>(1);
  }
}



}
#endif 