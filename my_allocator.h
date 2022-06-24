//
// Created by JAN on 2022/5/27.
//

#ifndef MYSTL__MY_ALLOCATOR_H_
#define MYSTL__MY_ALLOCATOR_H_
#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>
#include "my_type_traits.h"
#include "my_iterator.h"
//#define __MY_ALLOC_DEBUG
namespace jan{

//下列一些函数是配置器经常用到的方法，进行了一些优化

 /**
  * @brief before C++11 
  * 
  * @tparam T 
  * @param p 
  * @param val 
  */
	template <typename T>
	inline void construct(T * p, const T & val)
	{
		new (p) T(val);
	}

 /**
  * @brief after C++11
  * 
  * @tparam T 
  * @tparam Args 
  * @param p 
  * @param args 
  */
	template <typename T, typename... Args>
	inline void construct(T * p, Args && ... args)
	noexcept (noexcept(::new(p) T(std::forward<Args>(args)...)))
	{
		new T(std::forward<Args>(args)...);
	}

	template <typename T>
	inline void destroy(T * p)
	{
		p->~T();
	}

  /**
   * @brief not a POD type call this function
   * 
   * @tparam ForwardIterator 
   * @param first 
   * @param last 
   */
	template <typename ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, _false_type)
	{
		for(;first != last; ++first)
		{
			destroy(&*first);
		}
	}

 /**
  * @brief POD type call this function
  * 
  * @tparam ForwardIterator 
  * @param first 
  * @param last 
  */
	template <typename ForwardIterator>
	inline void _destroy_aux(ForwardIterator first, ForwardIterator last, _true_type)
	{ }


 /**
  * @brief 萃取出是否是POD类型，然后重载调用重载的aux版本
  * 
  * @tparam ForwardIterator 
  * @tparam T 
  * @param first 
  * @param last 
  */
	template <typename ForwardIterator, typename T>
	inline void _destroy(ForwardIterator first, ForwardIterator last, T *)
	{
		using has = typename type_traits<T>::has_trivial_destructor;
		_destroy_aux(first,last,has());
	}

  /**
   * @brief 用户应当使用这个
   * 
   * @tparam ForwardIterator 
   * @param first 
   * @param last 
   */
	template <typename ForwardIterator>
	inline void destroy(ForwardIterator first, ForwardIterator last)
	{
		_destroy(first,last, value_type(first));
	}

  //such as new_handler
	using malloc_handler = void (*)();
 /**
  * @brief 一级配置器，用于配置大块内存
  * 
  * @tparam ints 
  */
	template <int ints>
	class level_one_alloc_template
	{
	 private:
		static void * oom_malloc(size_t);
		static void * oom_realloc(void *, size_t);
		static malloc_handler malloc_alloc_oom_handler;
	 public:
		static void * allocate(size_t size)
		{
			void * res = malloc(size);
			if(res == nullptr)
				res = oom_malloc(size);
			return res;
		}

		static void deallocate(void * p, size_t)
		{
			free(p);
		}

		static void * reallocate(void * p, size_t, size_t new_size)
		{
			void * res = realloc(p,new_size);
			if(res == nullptr)
				res = oom_realloc(p, new_size);
			return res;
		}

		//一下函数用于仿真C++中的new_handler
		static malloc_handler set_malloc_handler(malloc_handler handler)
		{
			malloc_handler old_handler = malloc_alloc_oom_handler;
			malloc_alloc_oom_handler = handler;
			return old_handler;
		}
	};

	template <int ints>
	jan::malloc_handler
	level_one_alloc_template<ints>::malloc_alloc_oom_handler = nullptr;

	template <int ints>
	void* level_one_alloc_template<ints>::oom_malloc(size_t size)
	{
		malloc_handler my_handler;
		void * res;
		while(true)
		{
			my_handler = malloc_alloc_oom_handler;
			if(my_handler == nullptr) throw std::bad_alloc();
			my_handler();
			res = malloc(size);
			if(res != nullptr)
				return res;
		}
	}

	template <int ints>
	void* level_one_alloc_template<ints>::oom_realloc(void* p, size_t size)
	{
		void * res;
		malloc_handler my_handler;
		while(true)
		{
			my_handler = malloc_alloc_oom_handler;
			if(my_handler == nullptr) throw std::bad_alloc();
			my_handler();
			res = realloc(p,size);
			if(res != nullptr)
				return res;
		}
	}

	//设置一级配置器
	using malloc_alloc = level_one_alloc_template<0>;

  //为二级配置器准备的一些枚举值
	enum {_ALIGN = 8, _MAX_BYES = 128, _NFREELISTS = _MAX_BYES / _ALIGN}; // NOLINT(bugprone-reserved-identifier)

  /**
   * @brief 二级配置器，使用内存池技术，在请求内存块大的时候调用一级配置器
   * 
   * @tparam ints 
   */
	template <int ints>
	class level_two_alloc_template
	{
	 private:
		//向上取整8的倍数
		static size_t round_up(size_t size)
		{
			return (size + _ALIGN - 1)  & ~(_ALIGN - 1);
		}
		//内存区块对应的自由链表的位置为若不是8的倍数，向上找到最小8的倍数对应的节点
		//例如30找到32，60找到64
		static size_t FINDLIST_INDEX(size_t size)
		{
			return (size + _ALIGN - 1)/_ALIGN - 1;
		}
		union obj
		{
			obj * next;
			char client_data[1];
		};

		static obj * free_list[_NFREELISTS];
		static void * refill(size_t size);
		static void * chunk_alloc(size_t size, int & nobjs);
		static char * start_free;	 //内存池起始位置
		static char * end_free;		//内存池结束位置
		static size_t heap_size;

	 public:
		using free_list_type = obj * [_NFREELISTS];
		static void * allocate(size_t size);
		static void deallocate(void * p, size_t n);
//		static void * reallocate(void * p, size_t old_size, size_t new_size);
	};

	template <int ints>
	char *level_two_alloc_template<ints>::start_free = nullptr;

	template <int ints>
	char * level_two_alloc_template<ints>::end_free = nullptr;

	template <int ints>
	size_t level_two_alloc_template<ints>::heap_size = 0;

	template <int ints>
//	typename level_two_alloc_template<ints>::obj *
	typename level_two_alloc_template<ints>::obj *
	level_two_alloc_template<ints>::free_list[_NFREELISTS]  {
			0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
	};

	template <int ints>
	void* level_two_alloc_template<ints>::chunk_alloc(size_t size, int& nobjs)
	{
#if defined(__MY_ALLOC_DEBUG)
		std::cout << "[call chunk_alloc]" << std::endl;
#endif
		char * res;
		size_t tot_byte = size * nobjs;
#if defined(__MY_ALLOC_DEBUG)
		std::cout << size << " " << nobjs << std::endl;
#endif
		size_t remain = end_free - start_free;
		if(remain >= tot_byte)
		{

#if defined(__MY_ALLOC_DEBUG)
			std::cout << "[remain:" << remain << "]" << std::endl;
			std::cout << "1" << std::endl;
#endif
			res = start_free;
			start_free += tot_byte;
			return res;
		}
		else if(remain >= size)
		{
#if defined(__MY_ALLOC_DEBUG)
			std::cout << "2" << std::endl;
#endif
			nobjs = remain / size;
			tot_byte = size * nobjs;
			res = start_free;
			start_free += tot_byte;
			return res;
		}
		else
		{
#if defined(__MY_ALLOC_DEBUG)
			std::cout << "3" << std::endl;
#endif
			size_t byte_to_get = 2 * tot_byte + round_up(heap_size >> 4);
			//如果还有一些剩余内存，规整到自由链表中
			if(remain > 0)
			{
				auto free_list_of_index = free_list + FINDLIST_INDEX(remain);
				((obj*)start_free)->next = *free_list_of_index;
				*free_list_of_index = (obj*)start_free;
			}

			//在heap区配置空间
			start_free = (char *)malloc(byte_to_get);

			//在自由链表中寻找可用内存
			if(res == nullptr)
			{
				size_t i;
				obj **free_list_of_index;
				obj * p;
				for(i = size; i<=_MAX_BYES ; i += _ALIGN)
				{
					free_list_of_index = free_list + FINDLIST_INDEX(i);
					p = *free_list_of_index;
					if(p != nullptr)
					{
						*free_list_of_index = p->next;
						start_free = (char *)p;
						end_free = start_free + i;
						return chunk_alloc(size,nobjs);
					}
				}
				end_free = nullptr;
				start_free = (char *)malloc_alloc ::allocate(byte_to_get);
			}

			heap_size += byte_to_get;
			end_free = start_free + byte_to_get;
			return chunk_alloc(size,nobjs);
		}
	}

	template <int ints>
	void* level_two_alloc_template<ints>::refill(size_t size)
	{
#if defined(__MY_ALLOC_DEBUG)
		std::cout << "[call refill]" << std::endl;
#endif
		int nobjs = 20;
		char * chunk = (char *)chunk_alloc(size,nobjs);
		if(nobjs == 1)
			return chunk;
		auto free_list_of_index = free_list + FINDLIST_INDEX(size);
		char * res = chunk;
		obj * current_node, *next_node;
		*free_list_of_index = next_node = (obj*)(chunk + size);
		int i;
		for(i = 1; ; ++i)
		{
			current_node = next_node;
			next_node = (obj*)((char *)next_node + size);
			if(nobjs - 1 == i)
			{
				current_node->next = nullptr;
				break;
			}
			current_node->next = next_node;
		}
		return res;
	}

	template <int ints>
	void* level_two_alloc_template<ints>::allocate(size_t size)
	{

#if defined(__MY_ALLOC_DEBUG)
		std::cout << "[call allocate]" << std::endl;
#endif
		if(size > _MAX_BYES)
			return malloc_alloc::allocate(size);
		auto free_list_of_index = free_list + FINDLIST_INDEX(size);
		obj * res = *free_list_of_index;
		if(res == nullptr)
		{
			return refill(round_up(size));
		}
		*free_list_of_index = res->next;
		return res;
	}

	template <int ints>
	void level_two_alloc_template<ints>::deallocate(void* p, size_t n)
	{

#if defined(__MY_ALLOC_DEBUG)
		std::cout << "[call deallocate]" << std::endl;
#endif
		if(n > _MAX_BYES)
		{
			malloc_alloc::deallocate(p,n);
			return;
		}
		auto free_list_of_index = free_list + FINDLIST_INDEX(n);
		obj * q = (obj*)p;
		q->next = *free_list_of_index;
		*free_list_of_index = q;
	}

	using alloc = level_two_alloc_template<0>;

	template <typename T,typename Alloc>
	struct alloc_adapter{
		typedef T		value_type;
		static T * allocate(size_t size)
		{
#if defined(__MY_ALLOC_DEBUG)
			std::cout << "[call allocate_adapter size is:" << size << "]" << std::endl;
#endif
			return size == 0 ? nullptr : (T*)Alloc::allocate(size * sizeof (T));
		}

    /**
     * @brief 返回一个sizeof T内存大小的首地址
     * 
     * @return T* 
     */
		static T * allocate()
		{
			return (T*)Alloc::allocate(sizeof (T));
		}

		static void deallocate(T * p, size_t size)
		{
			Alloc::deallocate(p,size);
		}

		static void deallocate(T * p)
		{
			Alloc::delllocate(p,sizeof (T));
		}
	};
}	//namespace jan



namespace jan{

/**
 * @brief 最简单的配置器，仅仅使用::operator new来分配内存, 定位new来初始化对象
 *        
 * @tparam T 
 */
template <typename T>
class allocator
{
 public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;
	using size_type = size_t;
	using difference_tyep = ptrdiff_t;

	template<typename U>
	struct rebind {
		using other = allocator<U>;
	};
	pointer allocate(size_type n, T * = nullptr)
	{
		if (n > max_size())
			throw std::bad_alloc();
		std::cout << "[allocate " << n << " size]" << std::endl;
		return static_cast<T*>(::operator new(n * sizeof(T)));
	}
	void deallocate(pointer p, size_type)
	{
		std::cout << "[deallocate]" << std::endl;
		::operator delete (p);
	}

  /**
   * @brief 用于原地构造使用
   * 
   * @tparam U 
   * @tparam Args 
   * @param p 
   * @param args 
   */
	template<typename U, typename... Args>
	void construct(U * p, Args&&... args)
	noexcept(noexcept(::new(p) U(std::forward<Args>(args)...)))
	{
		new(p) U(std::forward<Args>(args)...);
	}
	void destroy(pointer p)
	{
		p->~T();
	}
	pointer address(reference x) const
	{
		return &x;
	}
	const_pointer address(const_reference x) const
	{
		return &x;
	}

	size_type max_size() const
	{
		return UINT_MAX / sizeof(T);
	}
};

}
#endif//MYSTL__MY_ALLOCATOR_H_
