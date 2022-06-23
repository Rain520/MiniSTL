#ifndef __MEMORY_H_
#define  __MEMORY_H_
#include "my_iterator.h"
#include "my_type_traits.h"
#include "my_algorithm.h"
#include "my_allocator.h"
//u_copy, u_fill, u_fill_n
namespace jan{
  /**
   * @brief 如果待填充元素的类型为 POD, 就直接调用fill_n(use operpatr=)
   * 
   * @tparam ForwardIter 
   * @tparam Size 
   * @tparam T 
   * @param first 
   * @param n 
   * @param val 
   * @return ForwardIter 
   */
  template <typename ForwardIter, typename Size, typename T>
  inline ForwardIter __uninitialized_fill_n_aux(ForwardIter first, Size n, const T & val, _true_type)
  {
    return fill_n(first, n, val);
  }
  
  /**
   * @brief 如果待填充的元素不是 POD 类型，就用构造函数
   * 
   * @tparam ForwardIter 
   * @tparam Size 
   * @tparam T 
   * @param first 
   * @param n 
   * @param val 
   * @return ForwardIter 
   */
  template <typename ForwardIter, typename Size, typename T>
  inline ForwardIter __uninitialized_fill_n_aux(ForwardIter first, Size n, const T & val, _false_type)
  {
    while (n--)
    {
      construct(&*first,val);
      ++first;
    }
    return first;
  }

  /**
   * @brief 用于匹配重载的aux版本
   * 
   * @tparam ForwardIter 
   * @tparam Size 
   * @tparam T 
   * @tparam T1 
   * @param first 
   * @param n 
   * @param val 
   * @return ForwardIter 
   */
  template <typename ForwardIter, typename Size, typename T, typename T1>
  inline ForwardIter __uninitialized_fill_n(ForwardIter first, Size n, const T & val, T1 *)
  {
    using is_POD = typename type_traits<T1>::is_POD_type;
    return __uninitialized_fill_n_aux(first,n,val,is_POD());
  }

  /**
   * @brief 初始化n个值为val的元素
   * 
   * @tparam ForwardIter 
   * @tparam Size 
   * @tparam T 
   * @param first 
   * @param n 
   * @param val 
   * @return ForwardIter 
   */
  template <typename ForwardIter, typename Size, typename T>
  inline ForwardIter uninitialized_fill_n(ForwardIter first, Size n, const T & val)
  {
    return __uninitialized_fill_n(first, n, val, value_type(first));
  }

  /************uninitialized_copy***********/
  template <typename ForwardIter, typename OutputIter>
  inline OutputIter __uninitialized_copy_aux(ForwardIter first, ForwardIter last,
                                            OutputIter res, _false_type)
  {
    for(;first != last; ++first, ++res)
      construct(&*res,*first);
  }

  template <typename ForwardIter, typename OutputIter>
  inline OutputIter __uninitialized_copy_aux(ForwardIter first, ForwardIter last,
                                             OutputIter res, _true_type)
  {
    return copy(first, last, res);
  }

  template <typename ForwardIter, typename OutputIter, typename T>
  inline OutputIter __uninitialized_copy(ForwardIter first, ForwardIter last, OutputIter res, T *)
  {
    using is_POD = typename type_traits<T>::is_POD_type;
    return __uninitialized_copy_aux(first,last,res,is_POD());
  }

  template <typename ForwardIter, typename OutputIter>
  inline OutputIter uninitialized_copy(ForwardIter first, ForwardIter last, OutputIter res)
  {
    __uninitialized_copy(first, last, res, value_type(first));
  }

  
  /**************uninitialized_fill***************/
  template <typename ForwardIter, typename T>
  inline ForwardIter __uninitialized_fill_aux(ForwardIter first, ForwardIter last, const T & val, _true_type)
  {
    return fill(first,last,val);
  }
  template <typename ForwardIter, typename T>
  inline ForwardIter __uninitialized_fill_aux(ForwardIter first, ForwardIter last, const T & val, _false_type)
  {
    for(; first != last; ++first)
      construct(&*first,val);
    return first;
  }
  template <typename ForwardIter, typename T, typename T1>
  inline ForwardIter __uninitialized_fill(ForwardIter first, ForwardIter last, const T & val, T1 *)
  {
    using is_POD = typename type_traits<T1>::is_POD_type;
    return __uninitialized_fill_aux(first,last,val,is_POD());
  }

  template <typename ForwordIter, typename T>
  inline ForwordIter uninitialized_fill(ForwordIter first, ForwordIter last, const T & val)
  {
    return __uninitialized_fill(first,last,val,value_type(first));
  }


}



#endif