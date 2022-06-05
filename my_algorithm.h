#ifndef __MY_ALGORITHM_H_
#define __MY_ALGORITHM_H_
#include "my_iterator.h"
#include "my_type_traits.h"
#include <cstddef>
#include <cstring>
#include <iostream>

namespace jan
{

	template<typename InputIter, typename T>
	T accumulate(InputIter first, InputIter last, T init)
	{
		for (; first != last; ++first)
		{
			init += *first;
		}
		return init;
	}

	template<typename InputIter, typename T, typename BinaryOp>
	T accumulate(InputIter first, InputIter last, T init, const BinaryOp& binary_op)
	{
		for (; first != last; ++first)
		{
			init += binary_op(init, *first);
		}
		return first;
	}

	template<typename InputIter, typename OutputIter>
	OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter res)
	{
		if (first == last)
			return res;
		*res = *first;
		while (++first != last)
		{
			*(++res) = *first - *(first - 1);
		}
		return ++res;
	}

	template<typename InputIter, typename OutputIter, typename BinaryOp>
	OutputIter adjacent_difference(InputIter first, InputIter last, OutputIter res, BinaryOp binary_op)
	{
		if (first == last)
			return res;
		*res = *first;
		while (++first != last)
		{
			*(++res) = binary_op(*first, *(first - 1));
		}
		return ++res;
	}

	template<typename InputIter1, typename InputIter2, typename T>
	T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			init += (*first1 * *first2);
		}
		return init;
	}

	template<typename InputIter1, typename InputIter2, typename T, typename BinaryOp>
	T inner_product(InputIter1 first1, InputIter1 last1, InputIter2 first2, T init, BinaryOp binary_op)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			init += binary_op(*first1, *first2);
		}
		return init;
	}

	template<typename InputIter, typename OutputIter>
	OutputIter partial_sum(InputIter first, InputIter last, OutputIter res)
	{
		if (first == last)
			return res;
		using value_type = typename iterator_traits<InputIter>::value_type;
		value_type val{};
		for (; first != last; ++first)
		{
			val += *first;
			*res++ = val;
		}
		return res;
	}

	template<typename InputIter, typename OutputIter, typename BinaryOp>
	OutputIter partial_sum(InputIter first, InputIter last, OutputIter res, BinaryOp binary_op)
	{
		if (first == last)
			return res;
		using value_type = typename iterator_traits<InputIter>::value_type;
		value_type val = *first;
		for (; first != last;)
		{
			*res++ = val;
			val = binary_op(val, *++first);
		}
		return res;
	}

	template<typename ForwardIter, typename T>
	void iota(ForwardIter first, ForwardIter last, T val)
	{
		while (first != last)
		{
			*first++ = val++;
		}
	}

	template<typename ForwardIter1, typename ForwardIter2>
	void iter_swap(ForwardIter1 iter1, ForwardIter2 iter2)
	{
		using value_type = typename iterator_traits<ForwardIter1>::value_type;
		value_type tmp = *iter1;
		*iter1 = *iter2;
		*iter2 = tmp;
	}

	template<typename InputIter1, typename InputIter2>
	inline bool lexicographical_compare(InputIter1 first1, InputIter1 last1,
										InputIter2 first2, InputIter2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (*first1 < *first2)
				return true;
			else if (*first1 > *first2)
				return false;
			else
				continue;
		}
		return first1 == last1 && first2 != last2;
	}

	template<typename T>
	inline const T& min(const T& a, const T& b)
	{
		return a > b ? b : a;
	}

	template<typename T, typename Compare>
	inline const T& min(const T& a, const T& b, Compare comp)
	{
		return comp(a, b) ? a : b;
	}

	template<typename T>
	inline const T& max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}

	inline bool lexicographical_compare(const unsigned char* first1,
										const unsigned char* last1,
										const unsigned char* first2,
										const unsigned char* last2)
	{
		const size_t len1 = last1 - first1;
		const size_t len2 = last2 - first2;
		int res = memcmp(first1, first2, min(len1, len2));
		return res != 0 ? res < 0 : len1 < len2;
	}

	inline bool lexicographical_compare(const char* first1, const char* last1,
										const char* first2, const char* last2)
	{
		int res = strcmp(first1, first2);
		return res <= 0 ? true : false;
	}

	template<typename T>
	inline void swap(T& a, T& b)
	{
		T tmp = std::move(a);
		a = std::move(b);
		b = std::move(tmp);
	}

	template<typename InputIter1, typename InputIter2>
	auto mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2) -> std::pair<InputIter1, InputIter2>
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (*first1 != *last1)
				break;
		}
		return {first1,first2};
	}

	template<typename InputIter1, typename InputIter2, typename BinaryOp>
	auto mismatch(InputIter1 first1, InputIter1 last1, InputIter2 first2, BinaryOp binary_op) -> std::pair<InputIter1, InputIter2>
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (binary_op(*first1, *first2))
				break;
		}
		return {first1, first2};
	}

	/**以下是copy一族函数，应使用copy接口**/

	/**
 * @brief 用以c风格字符串的copy,这是一个最终版
 * 
 * @param first 
 * @param last 
 * @param res 
 * @return char* 
 */
	inline char* copy(const char* first, const char* last, char* res)
	{
		//char is one byte
        std::cout << (last - first) << std::endl;
        std::cin.get();
		memmove(res, first, last - first);
		return res + (last - first);
	}

	/**
 * @brief 用以c风格字符串的拷贝，但是是wchar_t版本,这是一个最终版
 * 
 * @param first 
 * @param last 
 * @param res 
 * @return wchar_t* 
 */
	inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* res)
	{
		memmove(res, first, sizeof(wchar_t) * (last - first));
		return res + (last - first);
	}

	/**
 * @brief 使用Distance > 0 来作为循环的结束，如果迭代类型是一个随机访问类型
 *        例如deque::iterator,则会调用这个函数,这是一个最终版
 * 
 * @tparam RandomIter 
 * @tparam OutputIter 
 * @tparam Distance 
 * @param first 
 * @param last 
 * @param res 
 * @return OutputIter 
 */
	template<typename RandomIter, typename OutputIter, typename Distance>
	inline OutputIter __copy_d(RandomIter first, RandomIter last, OutputIter res, Distance*)
	{
		for (Distance n = last - first; n > 0; --n, ++first, ++res)
			*res = *first;
		return res;
	}

    /**下面两个函数是堆迭代器为原生指针类型做的优化**/
	/**
 * @brief 如果对象具有无关紧要的赋值运算符，也就是trivial assianment operator会调用这个版本,这是一个最终版
 * 
 * @tparam T 
 * @param first 
 * @param last 
 * @param res 
 * @return T* 
 */
	template<typename T>
	inline T* __copy_t(const T* first, const T* last, T* res, _true_type)
	{
        // std::cout << "[len:" << (last - first) << ":]" << std::endl;
		memmove(res, first,sizeof(T) * (last - first));
		return res + (last - first);
	}

	/**
 * @brief 如果对象的赋值运算符是non-trivial的，则会调用这个函数，此函数调用__copy_d
 * 
 * @tparam T 
 * @param first 
 * @param last 
 * @param res 
 * @return T* 
 */
	template<typename T>
	inline T* __copy_t(const T* first, const T* last, T* res, _false_type)
	{
		return __copy_d(first, last, res, (ptrdiff_t*)nullptr);
	}

	/**
 * @brief 如果迭代器是RandomAccess的，则会调用这个版本，其不做任何处理的调用__copy_d 
 * 
 * @tparam RandomIter 
 * @tparam OutputIter 
 * @param first 
 * @param last 
 * @return OutputIter 
 */
	template<typename RandomIter, typename OutputIter>
	inline OutputIter __copy(RandomIter first, RandomIter last, OutputIter res, random_access_iterator_tag)
	{
		return __copy_d(first, last, res, distance_type(first));
	}

	/**
 * @brief 如果迭代器不能实现随机访问的功能，则调用此版本,此版本使用迭代器相等的形式判断结束,这是一个最终版
 * 
 * @tparam InputIter 
 * @tparam OutputIter 
 * @param first 
 * @param last 
 * @param res 
 * @return OutputIter 
 */
	template<typename InputIter, typename OutputIter>
	inline OutputIter __copy(InputIter first, InputIter last, OutputIter res, input_iterator_tag)
	{
		for (; first != last; ++first, ++res)
			*res = *first;
		return res;
	}

	/**
 * @brief 用于作为中转函数的仿函数
 * 
 * @tparam InputIter 
 * @tparam OutputIter 
 */
	template<typename InputIter, typename OutputIter>
	struct __copy_dispatch {
		OutputIter operator()(InputIter first, InputIter last, OutputIter res)
		{
			return __copy(first, last, res, iterator_category(first));
		}
	};
    /**
     * @brief 用于作为中转函数的仿函数,对于T*的偏特化版本
     * 
     * @tparam T 
     */
	template<typename T>
	struct __copy_dispatch<T*, T*> {
		T* operator()(T* first, T* last, T* res)
		{
			using t = typename type_traits<T>::has_trivial_assignment_operator;
			//这里可以调用下面的偏特化版本，但是没有必要
			return __copy_t(first, last, res, t());
		}
	};
    /**
     * @brief 用于作为中转函数的仿函数,对于const T*, T*的偏特化版
     * 
     * @tparam T 
     */
	template<typename T>
	struct __copy_dispatch<const T*, T*> {
		T* operator()(const T* first, T* last, T* res)
		{
			using t = typename type_traits<T>::has_trivial_assignment_operator;
			return __copy_t(first, last, res, t());
		}
	};

    /**
     * @brief 将[first,last) 区间内的元素复制到以res起始的容器区间,
     *        对外提供的copy接口，用户应当仅仅使用这个函数
     * 
     * @tparam InputIter 
     * @tparam OutputIter 
     * @param first 源容器的区间首迭代器
     * @param last  源容器区间的尾后迭代器
     * @param res   目标容器的迭代器起始位置
     * @return OutputIter 
     */
	template<typename InputIter, typename OutputIter>
	inline OutputIter copy(InputIter first, InputIter last, OutputIter res)
	{
		return __copy_dispatch<InputIter, OutputIter>()(first, last, res);
	}
}// namespace jan

#endif