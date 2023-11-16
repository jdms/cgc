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


// This file is both a document and a test, as it shows how to use the `cgc`
// class for common memory management tasks, and its compiled binary can be
// used as input to valgrind to check whether there are any issues with either
// `cgc`'s or `ggc`'s memory management. 

#include <cstdio>


// If you really need to use this code, and you have a single (or a few) types
// for which you want to allocate pre-initialized C memory, please use class
// `cgc<T>` that is self-contained.
//
// If you don't want the memory to be pre-initialized or you want to manage
// memory allocated for multiple types with a single collector, please use class
// `ggc`. 
//
// Class `gcgc<T>` is meant to test the garbage garbage-collector `ggc`, both
// `cgc<T>` and `gcgc<T>` expose the same API.  Do not use class 'gcgc<T>'.

#if defined(USE_GGC)
    // Class `gcgc.hpp` is just meant to test class `ggc`, do not use it.
    #include "test/gcgc.hpp"
    template<typename T>
    using cgc = jdms::gcgc<T>;

#else
    #include "cgc.hpp"
    using namespace jdms;
#endif

int main() {

    // To manage C memory, one first has to instantiate an object of the cgc<T>
    // class.  Type T must be such that sizeof(T) returns a meaningful number,
    // i.e., neither a reference nor a pointer.  More specifically, cgc<T> ctor
    // uses SFINAE to allow instantiation only if T is a (1) trivial, (2)
    // standard layout and (3) non pointer type. 


    //// First example: int type. //////////////////////////////////////////////

    cgc<int> gc_integers;

    // Call cgc<T>::alloc( size_t n = 1 ) to allocate an array of type T with
    // 'n' elements, each element of which being also _automatically
    // initialized_ to T{}. 

    int *iptr0 = gc_integers.alloc(); // it defaults to n = 1
    // gc_integers.alloc() is automatically performing: 
    /* for ( size_t i = 0; i < 1; ++i ) */
    /*     iptr0[i] = 0; */

    *iptr0 = 1;


    //// Second example: int[] type. ///////////////////////////////////////////

    int *iptr1 = gc_integers.alloc(1024*1024);
    // gc_integers.alloc() is automatically performing: 
    /* for ( size_t i = 0; i < 1024*1024; ++i ) */
    /*     iptr1[i] = 0; */

    // not going through all of it, you've got the idea...
    for ( int i = 0; i < 10; ++i ) 
        iptr1[i] = i+1;


    //// Third example: inside a new scope. ////////////////////////////////////
    {
        // Pointer 'iptr2' is created inside this scope and destroyed right
        // away.  This is not a problem, as 'gc_integers' holds a copy that
        // points to the same memory, which will be freed when 'gc_integers'
        // itself is destroyed.  
   
        int *iptr2 = gc_integers.alloc(10);
        // gc_integers.alloc() is automatically performing: 
        /* for ( size_t i = 0; i < 10; ++i ) */
        /*     iptr3[i] = 0; */
        
        *iptr2 = 2;
    }


    //// Forth example: use cgc<T>::free(T**) to destroy large objects early. //

    // Call cgc<T>::free( T** ptr ) to manually free memory pointed by 'ptr' at
    // any time.  If successful it will return a (T*) NULL value.  Notice that
    // this signature, differently from the free( void* ) that is available in
    // <cstdlib>, expects a pointer to a pointer.  
    //
    // This is handy to destroy large objects earlier, if memory availability
    // is a concern.  Notice that a cgc instance will refuse to free memory it
    // has not allocated itself.  

    printf("Trying to manually free iptr1: ");
    gc_integers.free(iptr1);

    bool success = (iptr1 == (int *) NULL);
    if ( success )
        printf("success.\n");
    else
        printf("failure.\n");


    //// Fifth example: managing C strings. ////////////////////////////////////

    cgc<char> gc_chars;

    char *word = gc_chars.alloc(256); // no more than 255 letters in this word
    // gc_chars.alloc() automatically initializes memory

    word[0] = 'O'; word[1] = 'k'; word[2] = '\0';

    printf( "My word is %s.\n", word );


    //// Sixth example: managing C structs. ////////////////////////////////////

    struct Barista {
        double age;
        char *name;
    };
    cgc<Barista> gc_baristas;

    Barista *bptr = gc_baristas.alloc();
    // gc_baristas.alloc() automatically initializes memory

    bptr->age = 32;
    bptr->name = gc_chars.alloc(256); 
    // gc_chars.alloc() automatically initializes memory

    char name[4] = { 'F', 'o', 'o', '\0' }; 
    for ( int i = 0; i < 4; ++i )
    {
        bptr->name[i] = name[i];
    }

    printf( "Barista %s is %d years old.\n", bptr->name, (int)bptr->age);


    // When objects instantiated from cgc<T> (such as 'gc_integers', 'gc_chars'
    // and 'gc_baristas' in this example) are destroyed, they safely free all
    // memory they have allocated during their lifetime.  
    return 0; 
}
