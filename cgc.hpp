/* MIT License */

/* Copyright (c) 2018 Julio Daniel Machado Silva */

/* Permission is hereby granted, free of charge, to any person obtaining a copy */
/* of this software and associated documentation files (the "Software"), to deal */
/* in the Software without restriction, including without limitation the rights */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell */
/* copies of the Software, and to permit persons to whom the Software is */
/* furnished to do so, subject to the following conditions: */

/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software. */

/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE */
/* SOFTWARE. */


#ifndef __CGC_HPP__
#define __CGC_HPP__


#ifdef _WIN32
    #include <malloc.h>
#endif
#include <cstdlib>
#include <vector>
#include <type_traits>


///
/// cgc<T> is a simple garbage collector to do C type memory management in C++ code.
///

template<typename T>
class cgc {
    public:
        using type = T;

        /// ctor is only defined when type T has a POD (C plain old data) like memory layout and is not a pointer 
        template<
            typename U = T, /* dummy type U must depend on T for SFINAE to kick in */ 
            typename std::enable_if< 
                std::is_convertible<U, T>::value 
                && std::is_trivial<U>::value 
                && std::is_standard_layout<U>::value 
                /* && std::is_object<U>::value */ 
                && !std::is_pointer<U>::value >::type* = nullptr>
        cgc() 
        {
        }

        /// dtor frees all of the remaining allocated memory
        ~cgc()
        {
            for ( auto &p : ptr_list )
            {
                if ( p != (T*) NULL )
                {
                    ::free(p);
                }
            }
        }

        /// alloc returns pointer to n*sizeof(T) (already initialized) memory blocks
        T* alloc( size_t n = 1 )
        {
            if ( n == 0 )
            {
                return (T*) NULL;
            }

            T* mem_ptr = (T*) malloc( n * sizeof(T) );

            if ( mem_ptr != (T*) NULL )
            {
                ptr_list.push_back(mem_ptr);

                for ( size_t i = 0; i < n; ++i )
                {
                    mem_ptr[i] = T{};
                }
            }

            return mem_ptr;
        }

        /// free deallocates memory previously allocated by same instance of class and clears ptr
        void free( T** ptr )
        {
            if ( *ptr == (T*) NULL )
            {
                return;
            }

            for ( auto &p : ptr_list )
            {
                if ( p == *ptr )
                {
                    ::free(*ptr);
                    *ptr = (T*) NULL;
                    p = (T*) NULL;
                }
            }
        }

    private:
        std::vector<T*> ptr_list{};
};

#endif
