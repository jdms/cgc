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


#include <malloc.h>
#include <cstdlib>
#include <vector>
#include <type_traits>


template<typename T>
class cgc {
    public:
        using type = T;

        /* ctor is only defined when type T represents an /object/ that is not a /pointer/ */ 
        template<
            typename U = T /* dummy type U must depend on T for SFINAE to kick in */, 
            typename std::enable_if< 
                std::is_convertible<U, T>::value 
                && std::is_object<U>::value 
                && !std::is_pointer<U>::value >::type* = nullptr>
        cgc() 
        {
        }

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

        T* alloc( size_t n = 1 )
        {
            if ( n == 0 )
            {
                return (T*) NULL;
            }

            T* mem_ptr = (T*) malloc( n * sizeof(T) );

            ptr_list.push_back(mem_ptr);

            return mem_ptr;
        }

        void free( T* ptr )
        {
            if ( ptr == (T*) NULL )
            {
                return;
            }

            for ( auto &p : ptr_list )
            {
                if ( p == ptr )
                {
                    p = (T*) NULL;
                }
            }

            ::free(ptr);
        }

    private:
        std::vector<T*> ptr_list{};
};

#endif
