cmake_minimum_required(VERSION 3.21)
project(MyStl)

set(CMAKE_CXX_STANDARD 11)

add_executable(MyStl main.cpp my_allocator.h my_iterator.h my_type_traits.h my_heap.h
               my_algorithm.h)
