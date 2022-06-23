#ifndef __MY_VECTOR_H_
#define __MY_VECTOR_H_
#include "my_allocator.h"
#include "my_algorithm.h"
#include "memory.h"
#include <cstddef>
#include <stdexcept>

namespace jan{
  /**
  * @brief vector模板类，默认分配器为jan::alloc(二级配置器)
  * 
  * @tparam T 
  * @tparam Alloc 
  */
  template <typename T, typename Alloc = jan::alloc>
  class vector
  {
  public:  
    using value_type = T;
    using pointer = T *;
    using iterator = pointer;
    using const_iterator = const pointer;
    using reference = T &;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    iterator begin() const { return start; }
    const_iterator cbegin() const { return static_cast<const_iterator>(start); }
    const_iterator cend() const { return static_cast<const_iterator>(finish); }
    iterator end() const { return finish; }
    size_type size() const { return static_cast<size_type>(finish-begin()); }
    size_type capacity() const { return static_cast<size_type>(the_end-begin()); }
    bool empty() const { return begin() == end(); }
    reference operator[](size_type index) { return *(start + index); }
    reference front() const { return *begin(); }
    reference back() const { return  *(end()-1); }
    vector() : start(nullptr), finish(nullptr), the_end(nullptr)
    { }
    vector(size_type n, const T & val) {
      fill_initialized(n, val);
    }
    ~vector();
    void push_back(const T & val);
    void pop_back();
    iterator erase(iterator pos);
    iterator erase(iterator first, iterator last);
    void resize(size_type n, const T & val = T{});
    void clear();
    // void insert(iterator pos, size_type n, const T & val);
    iterator insert(iterator pos, const T & val);
  protected:
    void insert_aux(iterator pos, const T & val);
    iterator start, finish, the_end;
    void deallocate();
    using data_allocator = jan::alloc_adapter<T, Alloc>;
    iterator alloc_and_fill(size_type n, const T & val);
    void fill_initialized(size_type n, const T & val);
  };

  template <typename T, typename Alloc>
  typename vector<T,Alloc>::iterator
  vector<T,Alloc>::insert(iterator pos, const T &val)
  {
    insert_aux(pos, val);
    return pos - 1;
  }
  
  /**
   * @brief 清除容器
   * 
   * @tparam T 
   * @tparam Alloc 
   */
  template <typename T, typename Alloc>
  void vector<T,Alloc>::clear()
  {
    erase(begin(),end());
  }
    
  /**
   * @brief 清除某位置上的元素, 不保证使用其析构函数，如果T的operator=有特殊的语义，那是用户应当考虑的事
   * 
   * @tparam T 
   * @tparam Alloc 
   * @param pos 
   * @return vector<T,Alloc>::iterator 
   */
  template <typename T, typename Alloc>
  typename vector<T,Alloc>::iterator
  vector<T,Alloc>::erase(iterator pos)
  {
    if(pos < begin() || pos >= end())
      throw std::out_of_range("pos out of range");
    if(pos != end() - 1)
      copy(pos + 1, end(), pos);
    --finish;
    destroy(finish);
    return pos;
  }

  /**
   * @brief 清除区间[first,last)的元素，不保证使用其析构函数，如果T的operator=有特殊的语义，那是用户应当考虑的事
   * 
   * @tparam T 
   * @tparam Alloc 
   * @param first 
   * @param last 
   * @return vector<T,Alloc>::iterator 
   */
  template <typename T, typename Alloc>
  typename vector<T,Alloc>::iterator
  vector<T,Alloc>::erase(iterator first, iterator last)
  {
    if(last != end())
    {
      copy(last,end(),first);
      destroy(last,end());
    }
    else  //last = end() it's [pos,end())]
    {
      destroy(first,last);
    }
    finish -= (last - first);
    return first;
  }

  /**
   * @brief 重新调整容器大小为n，并且如果n大于原来大小，新的元素被构造为val
   *        如果不指定val，则为T{}形式的初值
   * @tparam T 
   * @tparam Alloc 
   * @param n 
   * @param val 
   */
  template <typename T, typename Alloc>
  void vector<T,Alloc>::resize(size_type n, const T & val)
  {
    if (n < 0)
      throw std::out_of_range("n is less zero");
    if (n == size())
      return;
    if(n < size())
      erase(begin()+n,end());
//    else
//      insert(end(),n - size(),val);
  }


  /**
   * @brief  在迭代器指向处插入一个元素, such as inster_after
   *      用于辅助插入，如果容器还有空间, 就正常插入，否则重新调整并插入
   * 
   * @tparam T 
   * @tparam Alloc 
   * @param pos 
   * @param val 
   */
  template <typename T, typename Alloc>
  void vector<T,Alloc>::insert_aux(iterator pos, const T & val)
  {
      //如果还有空间
      if(end() < the_end)
      {
        copy(pos,end(),pos+1);
        *pos = val;
        ++finish;
      } // 如果没有空间了
      else
      {
        const size_type old_size = size();
        const size_type new_size = old_size == 0 ? 1 : 2 * old_size;
        iterator new_start = data_allocator::allocate(new_size);
        iterator new_finish;
        try {
          new_finish = uninitialized_copy(begin(), pos, new_start);
          construct(new_finish,val);
          ++new_finish; //now new finish is pos
          new_finish = uninitialized_copy(pos, finish, new_finish);
        } catch (...) { 
          destroy(new_start,new_finish);
          data_allocator::deallocate(new_start,new_size);
          throw;
        }
        destroy(begin(),end());
        deallocate();
        start = new_start;
        finish = new_finish;
        the_end = new_start + new_size;
      }
  }
  


  /**
   * @brief 向容器尾部添加一个元素
   * 
   * @tparam T 
   * @tparam Alloc 
   * @param val 
   */
  template <typename T, typename Alloc>
  void vector<T,Alloc>::push_back(const T & val)
  {
    if(end() < the_end){
      construct(finish++,val);
    }
    else
      insert_aux(end(),val);
  }
  
  /**
   * @brief 将容器最后一个元素删除，执行析构函数
   * 
   * @tparam T 
   * @tparam Alloc 
   */
  template <typename T, typename Alloc>
  void vector<T,Alloc>::pop_back()
  {
    --finish;
    destroy(finish);
  }

  /**
   * @brief 回收全部内存空间
   * 
   * @tparam T 
   * @tparam Alloc 
   */
  template <typename T, typename Alloc>
  void vector<T,Alloc>::deallocate()
  {
    data_allocator::deallocate(begin(),size());
  }

  /**
   * @brief Destroy the vector<T,Alloc>::vector object
   * 
   * @tparam T 
   * @tparam Alloc 
   */
  template <typename T, typename Alloc>
  vector<T,Alloc>::~vector()
  {
    destroy(start,finish);
    deallocate();
  }

  
  /**
   * @brief 开辟内存并填充元素
   * 
   * @tparam T 
   * @tparam Alloc 
   * @param n 
   * @param val 
   * @return vector<T,Alloc>::iterator 
   */
  template <typename T, typename Alloc>
  typename vector<T,Alloc>::iterator
  vector<T,Alloc>::alloc_and_fill(size_type n, const T & val)
  {
    iterator res = data_allocator::allocate(n);
    jan::uninitialized_fill_n(res,n,val);
    // std::uninitialized_fill_n(begin(),n,val);
    return res;
  }
  
  
  /**
   * @brief 开辟并填充元素, 并调整指针
   * 
   * @tparam T 
   * @tparam Alloc 
   * @param n 
   * @param val 
   */
  template <typename T, typename Alloc>
  void vector<T,Alloc>::fill_initialized(size_type n, const T & val)
  {
    start = alloc_and_fill(n, val);
    finish = begin() + n;
    the_end = finish;
  }

}


#endif