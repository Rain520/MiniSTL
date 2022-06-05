//
// Created by JAN on 2022/6/4.
//

#ifndef MYSTL__MY_PAIR_H_
#define MYSTL__MY_PAIR_H_
#include "my_algorithm.h"

namespace jan{

template <typename T1, typename T2>
struct pair
{

	using first_type = T1;
	using second_type = T2;
	T1 first;
	T2 second;

	pair() = default;
	pair(const T1 & t1, const T2 & t2) : first(t1), second(t2)
	{ }
	pair(T1 && t1, T2 && t2) noexcept : first(std::forward<T1>(t1)), second(std::forward<T2>(t2))
	{ }
	pair(const pair &) = default;
	pair(pair && )  noexcept = default;


	pair<T1,T2> & operator=(const pair<T1,T2> & p)
	{
		first = p.first;
		second = p.second;
	}
//	pair & operator=(const pair & p) = default;

	pair<T1,T2> & operator=(pair<T1,T2> && p) noexcept
	{
		first = std::move(p.first);
		second = std::move(p.second);
	}

	bool operator==(const pair<T1,T2> & p) const
	{
		return first == p.first && second == p.second;
	}

	bool operator!=(const pair<T1,T2> & p) const
	{
		return first!=p.first && second!=p.second;
	}

	void swap(pair<T1,T2> & p)
	{
		using jan::swap;
		swap(first,p.first);
		swap(second,p.second);
	}
};

template <typename T1, typename T2>
pair<T1,T2> make_pair(T1 && t1, T2 && t2)
{
	return {std::forward<T1>(t1), std::forward<T2>(t2)};
}
} //namespace jan end
#endif//MYSTL__MY_PAIR_H_
