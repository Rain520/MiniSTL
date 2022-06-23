//
// Created by JAN on 2022/5/30.
//

#ifndef MYSTL__MY_TYPE_TRAITS_H_
#define MYSTL__MY_TYPE_TRAITS_H_

namespace jan{
	struct _true_type { };
	struct _false_type{ };
#define A_BUILD_IN_TYPE(Type) 								\
    template<> struct type_traits<Type>{        			\
      typedef _true_type has_trivial_default_constructor; \
   		typedef _true_type has_trivial_copy_constructor;  	\
   		typedef _true_type has_trivial_assignment_operator;\
   		typedef _true_type has_trivial_destructor;  		\
   		typedef _true_type is_POD_type;                 	\
	};                         						   		\

	template <typename T>
	struct type_traits{
		typedef _false_type has_trivial_default_constructor;
		typedef _false_type has_trivial_copy_constructor;
		typedef _false_type has_trivial_assignment_operator;
		typedef _false_type has_trivial_destructor;
		typedef _false_type is_POD_type;
	};

	template <typename T>
	struct type_traits<T*>
	{
		typedef _true_type has_trivial_default_constructor;
		typedef _true_type has_trivial_copy_constructor;
		typedef _true_type has_trivial_assignment_operator;
		typedef _true_type has_trivial_destructor;
		typedef _true_type is_POD_type;
	};

	
	//int8 16 32 ..._t均为typedef
	A_BUILD_IN_TYPE(wchar_t);
	A_BUILD_IN_TYPE(int);
	A_BUILD_IN_TYPE(double);
	A_BUILD_IN_TYPE(long);
	A_BUILD_IN_TYPE(char);
	A_BUILD_IN_TYPE(signed char );
	A_BUILD_IN_TYPE(unsigned char );
	A_BUILD_IN_TYPE(unsigned int);
	A_BUILD_IN_TYPE(long long int);
	A_BUILD_IN_TYPE(short)
	A_BUILD_IN_TYPE(unsigned short );
	A_BUILD_IN_TYPE(unsigned long);
	A_BUILD_IN_TYPE(long double);


}

#endif//MYSTL__MY_TYPE_TRAITS_H_
