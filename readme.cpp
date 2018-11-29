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
// used as input to valgrind to check whether there are any issues with `cgc`'s
// memory management. 

#include <cstdio>
#include "cgc.hpp"


int main()
{
    // To manage C memory, one first has to instantiate an object of the cgc<T>
    // class.  Type T must be such that sizeof(T) returns a meaningful number,
    // i.e., neither a reference nor a pointer.


    // First example with int.
    cgc<int> gc_integers;

    // cal cgc<T>::alloc( size_t n = 1 ) to allocate an array of type T with 'n' elements
    int *iptr0 = gc_integers.alloc(); // it defaults to n = 1
    *iptr0 = 0;

    int *iptr1 = gc_integers.alloc(10);
    for ( int i = 0; i < 10; ++i )
        iptr1[i] = i+1;

    {
        // Pointer 'iptr2' is created inside this scope and destroyed right
        // away.  This is not a problem, as 'gc_integers' holds a copy that
        // points to the same memory, which will be freed when 'gc_integers'
        // itself is destroyed.  
        int *iptr2 = gc_integers.alloc(1024);
        *iptr2 = 2;
    }

    // cal cgc<T>::free( T* ptr ) to manually free the memory pointed by 'ptr'
    gc_integers.free(iptr1);


    // And now to manage C type strings
    cgc<char> gc_chars;

    char *word = gc_chars.alloc(256); // no more than 255 letters in this word
    word[0] = 'O'; word[1] = 'k'; word[2] = '\0';

    printf( "\nMy word is %s\n", word );


    // Of course it works with structs as well as primitive types
    struct Barista {
        double age;
        char *name;
    };

    // It doesn't matter that 'Barista.name' is a pointer as it lies inside 'Barista'.
    cgc<Barista> gc_baristas;

    Barista *bptr = gc_baristas.alloc();

    bptr->age = 32;
    bptr->name = gc_chars.alloc(256); // but you must remember to initialize it 

    char name[4] = { 'F', 'o', 'o', '\0' }; 
    for ( int i = 0; i < 4; ++i )
    {
        bptr->name[i] = name[i];
    }

    printf( "\nBarista %s is %d years old.\n", bptr->name, (int)bptr->age);


    // When objects instantiated from cgc<T> (such as 'gc_integers', 'gc_chars'
    // and 'gc_baristas' in this example) are destroyed, they safely free all
    // memory they have allocated during their lifetime.  
    return 0; 
}
