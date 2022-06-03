//
// Created by JAN on 2022/5/27.
//
#include <iostream>
#include "my_iterator.h"
#include <vector>
#include "my_allocator.h"
#include <algorithm>

void test_new_handler()
{
	auto handler = []{
		std::cerr << "out of memory" << std::endl;
		//没有异常退出状态
	};
	std::set_new_handler(handler);
	// int * data = new int[10000000000];
}

void test_my_alloc()
{
	using namespace std;
	using vec_alloc = jan::alloc_adapter<int,jan::alloc>;
//	using vec_alloc = jan::alloc_adapter<int,jan::malloc_alloc>;
//	using vec_alloc = jan::allocator<int>;
	std::vector<int,vec_alloc> vec(10);
	for(int i=0;i<100000000;++i)
		vec.push_back(i);
	for(const int & val : vec)
	{
		cout << val << " ";
	}
}

template <typename T>
void print(const T & val)
{
	cout << val << " ";
}
void test_make_heap()
{
	using std::vector;
	vector<int> vec{1,5,6,9,7,4,7,4,56,55};
	make_heap(vec.begin(),vec.end());
	for_each(vec.begin(),vec.end(),print<int>);
	using namespace std;
	cout << endl;
	pop_heap(vec.begin(),vec.end());
	for_each(vec.begin(),vec.end(),print<int>);
}