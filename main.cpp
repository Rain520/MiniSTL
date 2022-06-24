#include <cstddef>
#include <iostream>
#include <deque>
#include <array>
#include "my_vector.h"
#include "my_allocator.h"
#include "my_iterator.h"
// #include "my_heap.h"
#include <new>
#include <vector>
#include <algorithm>
#include <ctime>
// #include <windows.h>
#include <numeric>
#include <iterator>
#include <functional>
#include "my_algorithm.h"
//#include "my_pair.h"
using namespace std;
#define FOO
class Foo
{
 public:
	static size_t con_count;
	static size_t des_count;
#ifdef __FOO
	Foo()
	{
		++con_count;
		cout << "[Foo::construction]: "<< con_count << endl;
	}
#ifdef __FOO_MOVE
	Foo(Foo && foo) noexcept
	{
		cout << "[Foo::move]" << endl;
	}
#endif
	~Foo()
	{
		++des_count;
		cout << "[Foo::destruction]: " << des_count << endl;
	}
#endif
};
size_t Foo::con_count = 0;
size_t Foo::des_count = 0;
//static Foo foo;

template<typename T>
void print(const T & val)
{
	cout << val << " ";
}
// void test_pop_heap()
// {
// 	vector<int> vec{1,5,9,3,5,7,8,4,2,6};
// 	make_heap(vec.begin(), vec.end());
// 	for_each(vec.begin(), vec.end(), print<int>);
// 	cout << endl;
// 	pop_heap(vec.begin(), vec.end());
// 	for_each(vec.begin(), vec.end(), print<int>);
// }

// void test_my_push_heap()
// {
// 	vector<int> vec{1,5,9,3,5,7,8,4,2,6};
// 	make_heap(vec.begin(), vec.end());
// 	for_each(vec.begin(), vec.end(), print<int>);
// 	cout << endl;
// 	vec.push_back(55);
// //	std::push_heap(vec.begin(), vec.end());
// //	for_each(vec.begin(), vec.end(), print<int>);
// 	jan::push_heap(vec.begin(), vec.end());
// 	for_each(vec.begin(), vec.end(), print<int>);
// }

// void test_my_pop_heap()
// {
// 	vector<int> vec{8,7,6,5,4,3,2,1,0,2};
// 	std::make_heap(vec.begin(),vec.end());
// 	for_each(vec.begin(),vec.end(),print<int>);
// 	cout << endl;
// 	jan::pop_heap(vec.begin(),vec.end());
// 	for_each(vec.begin(),vec.end(),print<int>);
// 	vector<int> vec1(1000);
// 	srand((unsigned)time(0));
// 	for(int & it : vec1)
// 	{
// 		it = rand() % 1000 + 1;
// 	}
// 	std::make_heap(vec1.begin(),vec1.end());
// 	jan::sort_heap(vec1.begin(),vec1.end());
// 	cout << endl;
// 	for_each(vec1.begin(),vec1.end(),print<int>);
// 	cout << endl;
// 	cout << std::is_sorted(vec1.begin(),vec1.end());

// }

// void test_my_make_heap()
// {
// 	const int n = 100;
// 	vector<int> vec1(n);
// 	srand((unsigned)time(0));
// 	for(auto it = vec1.begin(); it != vec1.end(); ++it)
// 	{
// 		*it = rand() % 1000 + 1;
// 	}
// 	auto vec_copy = vec1;
// 	std::make_heap(vec_copy.begin(),vec_copy.end());
// 	jan::make_heap(vec1.begin(),vec1.end());
// 	cout << (vec1 == vec_copy);
// 	jan::sort_heap(vec1.begin(),vec1.end());
// 	std::sort_heap(vec_copy.begin(),vec_copy.end());
// 	cout << std::is_sorted(vec1.begin(),vec1.end());
// 	cout << std::is_sorted(vec_copy.begin(),vec_copy.end());
// }

// void test_my_make_heap_time()
// {
// 	int count = 10;
// 	const int n = 1000000;
// 	srand((unsigned)time(0));
// 	while(count--)
// 	{
// 		cout << "---------" << endl;
// 		vector<int> vec1(n);
// 		for_each(vec1.begin(),vec1.end(),[](int & val){
// 			val = rand() % 1000 + 1;
// 		});
// 		auto vec2 = vec1;
// 		auto start_time = GetTickCount();
// 		std::make_heap(vec1.begin(),vec1.end());
// 		cout << "use std::make_heap: ";
// 		cout << (GetTickCount()-start_time) << endl;

// 		start_time = GetTickCount();
// 		jan::make_heap(vec2.begin(),vec2.end());
// 		cout << "use jan::make_heap: ";
// 		cout << (GetTickCount()-start_time) << endl;

// 	}
// }

// void test_my_sort_heap()
// {
// 	int count = 10;
// 	const int n = 1000000;
// 	srand((unsigned)time(0));
// 	while(count--)
// 	{
// 		cout << "---------" << endl;
// 		vector<int> vec1(n);
// 		for_each(vec1.begin(),vec1.end(),[](int & val){
// 			val = rand() % 1000 + 1;
// 		});
// 		auto vec2 = vec1;
// 		std::make_heap(vec1.begin(),vec1.end());
// 		auto start_time = GetTickCount();
// 		std::sort_heap(vec1.begin(),vec1.end());
// 		cout << "use std::sort_heap: ";
// 		cout << (GetTickCount()-start_time) << endl;

// 		jan::make_heap(vec2.begin(),vec2.end());
// 		start_time = GetTickCount();
// 		jan::sort_heap(vec2.begin(),vec2.end());
// 		cout << "use jan::sort_heap: ";
// 		cout << (GetTickCount()-start_time) << endl;
// 	}

// }

void test_partial_sum()
{
	vector<int> vec{1,2,3,4,5};
	ostream_iterator<int> osit(cout, " ");
	partial_sum(vec.begin(),vec.end(),osit);
	cout << endl;
	partial_sum(vec.begin(),vec.end(),osit,minus<int>());

}

void test_my_adjacent_difference()
{
	vector<int> vec{1,2,3,4,5};
	ostream_iterator<int> osit(cout, " ");
	jan::adjacent_difference(vec.begin(),vec.end(),osit);
	cout << endl;
	jan::adjacent_difference(vec.begin(),vec.end(),osit,plus<int>());
	cout << endl;
	jan::partial_sum(vec.begin(),vec.end(),osit);
	cout << endl;
	jan::partial_sum(vec.begin(),vec.end(),osit,minus<int>());
}

void test_iter_swap()
{
	vector<int> vec{1,2};
	jan::iter_swap(vec.begin(),vec.end()-1);
	cout << vec.at(0) << " " << vec.at(1);
}

void test_lex_compare()
{
	const char * s1 = "aabb";
	const char * s2 = "hell";
	string ss1 = "aabb";
	string ss2 = "hell";
	cout << strcmp(s1,s2) << endl;
	cout << std::lexicographical_compare(ss1.begin(),ss1.end(),
						ss2.begin(),ss2.end()) << endl;

}

//void test_my_pair()
//{
//	jan::pair<int,double> p1 (10,3.02);
//	jan::pair<int,double> p2 (22,30);
//
//	cout << p1.first << " " << p1.second << endl;
//	cout << p2.first << " " << p2.second << endl;
//
//	p1.swap(p2);
//
//	cout << p1.first << " " << p1.second << endl;
//	cout << p2.first << " " << p2.second << endl;
//
//	cout << (p1 == p2) <<  " " << (p1 != p2) << endl;
////	std::pair<int,int>(1,1).swap()
//}

void test_my_copy()
{
	array<int,20> int_arr{};
	int i = 1;
	for(auto & val : int_arr)
	{
		val = i++;
	}
	for_each(int_arr.begin(),int_arr.end(),print<int>);
	cout << endl;
	jan::copy(int_arr.begin() + 10, int_arr.begin() + 15,int_arr.begin());
	// std::copy(int_arr.begin() + 10, int_arr.begin() + 15,int_arr.begin());
	for_each(int_arr.begin(),int_arr.end(),print<int>);
	cout << endl;

	deque<int> dq(int_arr.begin(),int_arr.end());
	jan::copy(dq.begin() + 10, dq.begin() + 15,dq.begin());
	cout << endl;
	for_each(dq.begin(),dq.end(),print<int>);
	cout << endl;
	char str1[] = {"0123456789"};
	char str2[] = {"0123456789"};
	std::copy(str1+3,str1 + 5, str2);
	cout << str2 << endl;

}

void test_uninitia()
{
  int a[10];
  jan::fill_n(jan::fill_n(begin(a), 6, 100), 4, 20);
  for_each(begin(a), end(a), print<int>);
  cout << endl;
}

void test_vector()
{
  jan::vector<int> vec(10,2);
  vec.emplace_back(10);
  for_each(vec.begin(), vec.end(), print<int>);
  cout << endl;
  vec.emplace_back(20);
  for_each(vec.begin(), vec.end(), print<int>);
  // auto vec_move(std::move(vec));
  // for_each(vec.begin(), vec.end(), print<int>);
  // cout << endl;
  // for_each(vec_move.begin(), vec_move.end(), print<int>);
  // cin.get();
  // auto vec_copy(vec);
  // for_each(vec.begin(), vec.end(), print<int>);
  // cout << endl;
  // for_each(vec_copy.begin(), vec_copy.end(), print<int>);
  // cout << endl;
  // jan::vector<int> vv ({1,3,6,5,8,9,45,4,4,4});
  // for_each(vv.begin(), vv.end(), print<int>);

//	for_each(vec.cbegin(), vec.cend(), print<int>);
//	vec.clear();
//	for_each(vec.cbegin(), vec.cend(), print<int>);
//   vec.push_back(10);
//   vec.push_back(20);
//   cout << "size=" << vec.size() << endl;
//   cout << vec.capacity() <<endl;
//   for_each(vec.cbegin(), vec.cend(), print<int>);
//   cout << endl;
//   vec.erase(vec.begin()+5);
//   for_each(vec.cbegin(), vec.cend(), print<int>);
//   cout << endl;
//   vec.resize(0);
//   cout << "call vec.resize(0):";
//   for_each(vec.cbegin(), vec.cend(), print<int>);
//	cout << "call vec.resize(15,6):";
	// vec.resize(15,6);
	// for_each(vec.cbegin(), vec.cend(), print<int>);
//	cout << endl;
//   cout << endl;
//   vec.push_back(1);
//   vec.push_back(2);
//   vec.push_back(3);
//   for_each(vec.cbegin(), vec.cend(), print<int>);
//   vec.pop_back();
//   cout << endl;
//   cout << vec.front() << endl;
//   cout << vec.back() << endl;
  // vec.insert(vec.end()-1,10,100);
  // for_each(vec.cbegin(), vec.cend(), print<int>);
  // std::vector<int> std_vec(10,1);
  // jan::vector<int> my_vec(10,1);
  // for(int i = 0;i < 40; ++i)
  // {
  //   int idx = rand() % std_vec.size();
  //   // cout << "idx=" << idx << endl;
  //   my_vec.insert(my_vec.begin()+idx,idx,idx);
  //   std_vec.insert(std_vec.begin()+idx,idx,idx);
  // }
  // bool eq = true;
  // for(int i=0;i<std_vec.size();++i)
  // {
  //   if(my_vec[i]!=std_vec[i])
  //   {
  //     cout << "not equal" << endl;
  //     eq = false;
  //     break;
  //   }
  // }
  // if(eq)
  //   cout << "ok" << endl;

}

int main()
{
	//	test_pop_heap();
	// test_my_push_heap();
	// test_my_pop_heap();
	// test_my_make_heap();
	// test_my_make_heap_time();
	// test_my_sort_heap();
	// test_partial_sum();
	// test_my_adjacent_difference();
	// test_iter_swap();
	// test_my_pair();
	// test_my_copy();
  // test_uninitia();
  test_vector();
	cin.get();
	return 0;
}
