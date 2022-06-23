#ifndef __MY_HEAP_
#define __MY_HEAP_
#include "my_iterator.h"
namespace jan
{

	template <typename RandomIter, typename Distance, typename T>
	void __push_heap(RandomIter first, Distance holdIdx, Distance topInx, const T & val)
	{
		Distance parent = (holdIdx - 1) / 2;
		while(holdIdx > topInx && val > *(first + parent))
		{
			*(first + holdIdx) = *(first + parent);
			holdIdx = parent;
			parent = (holdIdx - 1) / 2;
		}
		*(first + holdIdx) = val;
	}
template <typename RandomIter, typename Distance, typename T>
inline void __push_heap_aux(RandomIter first, RandomIter last, Distance *, T *)
{
    __push_heap(first,
            static_cast<Distance>((last - first - 1)),
            static_cast<Distance>(0),
            T(*(last - 1)));
}


template <typename RandomIter>
inline void push_heap(RandomIter first, RandomIter last)
{
	__push_heap_aux(first,last,distance_type(first),value_type(first));
}




/**
 * @brief  调整一个根节点不遵守规则的大根堆
 * 
 * @tparam RandomIter 
 * @tparam Distance 
 * @tparam T 
 * @param first 	容器的首迭代器
 * @param holdIdx 	区间起始
 * @param len 		容器头到区间结束的长度
 * @param val 		根节点的值
 */
template <typename RandomIter, typename Distance, typename T>
void __adjust_heap(RandomIter first, Distance holdIdx, Distance len, T  val)
{
	Distance topIdx = holdIdx;
	Distance child = holdIdx * 2 + 1;
	while (child < len)
	{
		if(child < len - 1 && *(first + child + 1) > *(first + child))
			++child;
		if(val >= *(first + child))
			break;
		*(first + holdIdx) = *(first + child);
		holdIdx = child;
		child = holdIdx * 2 + 1;
	}
	*(first + holdIdx) = val;
}


/**
 * @brief 
 * 
 * @tparam RandomIter 
 * @tparam T 
 * @tparam Distance 
 * @param first 容器首迭代器
 * @param last 	容器尾迭代器
 * @param res 	将顶部元素pop后放到哪里
 * @param val 	被上溯的元素
 */
template <typename RandomIter, typename T, typename Distance>
inline void __pop_heap(RandomIter first, RandomIter last, RandomIter res,
										 T val, Distance *)
{
	*res = *first;	//将队首元素赋值响应的地方
	__adjust_heap(first,Distance(0),Distance(last - first),val);
}

template <typename RandomIter, typename T>
inline void __pop_heap_aux(RandomIter first, RandomIter last, T*)
{
	//第一个last-1表示：将顶部节点放到最尾后，这个调整堆的时候尾部迭代器要向前移动
	//第二个last-1表示，将顶部的值复制到last-1出，即原先的尾端
	__pop_heap(first,last-1,last-1,T(*(last-1)),distance_type(first));
}
template <typename RandomIter>
inline void pop_heap(RandomIter first, RandomIter last)
{
	__pop_heap_aux(first,last,value_type(first));
}

template <typename RandomIter>
inline void sort_heap(RandomIter first, RandomIter last)
{
	while(last - first > 1)
	{
		jan::pop_heap(first,last--);
	}
}

template <typename RandomIter, typename T, typename Distance>
inline void __make_heap(RandomIter first, RandomIter last, T *, Distance *)
{
	if(last - first < 2)
		return;
	Distance len = last - first;
	Distance holdIdx = (len - 2) / 2;
	while(holdIdx >= 0)
	{
		__adjust_heap(first,holdIdx,len,T(*(first + holdIdx)));
		--holdIdx;
	}
}

template <typename RandomIter>
inline void make_heap(RandomIter first, RandomIter last)
{
	__make_heap(first,last,value_type(first),distance_type(first));
}


template <typename RandomIter, typename Compare	>
inline void pop_heap(RandomIter first, RandomIter last, const Compare comp)
{

}


} // namespace jan


#endif