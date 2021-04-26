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


#ifndef GARBAGE_GARBAGE_COLLECTOR_HPP
#define GARBAGE_GARBAGE_COLLECTOR_HPP


#ifdef _WIN32
    #include <malloc.h>
#endif
#include <cstdlib>
#include <vector>


namespace jdms {

///
/// garbage garbage-collector (ggc): wraps C malloc() and free() for RAII memory management
///
class ggc {
    public:

        /// default ctor
        ggc() = default;

        /// dtor frees all of the remaining allocated memory
        virtual ~ggc()
        {
            ggc::clear();
        }

        /// deleted copy ctor
        ggc(const ggc&) = delete;

        /// deleted copy assigment
        ggc& operator=(const ggc&) = delete;

        /// default move ctor
        ggc(ggc&&) = default;

        /// default move assigment
        ggc& operator=(ggc&&) = default;

        /// malloc allocs and returns a pointer to a memory block of size `n`
        void* malloc( std::size_t n )
        {
            if ( n == 0 )
            {
                return (void*) NULL;
            }

            void* mem_ptr = (void*) ::malloc( n );

            if ( mem_ptr != (void*) NULL )
            {
                ptr_list.push_back(mem_ptr);
            }

            return mem_ptr;
        }

        /// free deallocates memory previously allocated by same instance of class
        bool free( void* ptr )
        {
            if ( ptr == (void*) NULL )
            {
                return true;
            }

            for ( auto &p : ptr_list )
            {
                if ( p == ptr )
                {
                    ::free(ptr);
                    p = (void*) NULL;
                    return true;
                }
            }

            return false;
        }

        /// clear frees all of the remaining allocated memory
        void clear()
        {
            for ( auto &p : ptr_list )
            {
                if ( p != (void*) NULL )
                {
                    ::free(p);
                }
            }

            ptr_list.clear();
        }

    private:
        std::vector<void*> ptr_list{};
};

} // namespace jdms

#endif
