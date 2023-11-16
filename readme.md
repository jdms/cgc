## cgc/ggc: simple C memory manager for C++ projects

This is both a document and a test, as it shows how to use the `cgc` and `ggc`
classes for common memory management tasks, and the binary compiled from this
code can be used as input to valgrind to check whether there are any issues.

To manage memory for C code (inside C++) that depends on a single (or a few)
types for which you want to allocate pre-initialized C memory, please use class
`cgc<T>`.

If you don't want the memory to be pre-initialized or you want to manage
memory allocated for multiple types with a single collector, please use class
`ggc`. 


### First example: int type.

To manage C memory, one first has to instantiate an object of the cgc<T>
class.  Type T must be such that sizeof(T) returns a meaningful number,
i.e., neither a reference nor a pointer.  More specifically, cgc<T> ctor
uses SFINAE to allow instantiation only if T is a (1) trivial, (2)
standard layout and (3) non pointer type. 

```.cpp

    cgc<int> gc_integers;

    // Call cgc<T>::alloc( size_t n = 1 ) to allocate an array of type T with
    // 'n' elements, each element of which being also _automatically
    // initialized_ to T{}. 

    int *iptr0 = gc_integers.alloc(); // it defaults to n = 1
    // gc_integers.alloc() is automatically performing: 
    /* for ( size_t i = 0; i < 1; ++i ) */
    /*     iptr0[i] = 0; */

    *iptr0 = 1;
```


### Second example: int[] type.

```.cpp
    int *iptr1 = gc_integers.alloc(1024*1024);
    // gc_integers.alloc() is automatically performing: 
    /* for ( size_t i = 0; i < 1024*1024; ++i ) */
    /*     iptr1[i] = 0; */

    // not going through all of it, you've got the idea...
    for ( int i = 0; i < 10; ++i ) 
        iptr1[i] = i+1;
```


### Third example: inside a new scope.

Pointer `iptr2` is created inside this scope and destroyed right
away.  This is not a problem, as `gc_integers` holds a copy that
points to the same memory, which will be freed when `gc_integers`
itself is destroyed.  
   
```.cpp
    {
        int *iptr2 = gc_integers.alloc(10);
        // gc_integers.alloc() is automatically performing: 
        /* for ( size_t i = 0; i < 10; ++i ) */
        /*     iptr3[i] = 0; */
        
        *iptr2 = 2;
    }
```


### Forth example: use `cgc<T>::free(T**)` to destroy large objects early.

Call `cgc<T>::free( T** ptr )` to manually free memory pointed by 'ptr' at
any time.  If successful it will return a (T*) NULL value.  Notice that
this signature, differently from the free( void* ) that is available in
<cstdlib>, expects a pointer to a pointer.  

This is handy to destroy large objects earlier, if memory availability
is a concern.  Notice that a `cgc` instance will refuse to free memory it
has not allocated itself.  

```.cpp
    printf("Trying to manually free iptr1: ");
    gc_integers.free(iptr1);

    bool success = (iptr1 == (int *) NULL);
    if ( success )
        printf("success.\n");
    else
        printf("failure.\n");
```


### Fifth example: managing C strings.

```.cpp
    cgc<char> gc_chars;

    char *word = gc_chars.alloc(256); // no more than 255 letters in this word
    // gc_chars.alloc() is automatically performing: 
    /* for ( size_t i = 0; i < 256; ++i ) */
    /*     word[i] = '\0'; */

    word[0] = 'O'; word[1] = 'k'; word[2] = '\0';

    printf( "My word is %s.\n", word );
```


### Sixth example: managing C structs.

```.cpp
    struct Barista {
        double age;
        char *name;
    };

    cgc<Barista> gc_baristas;
    Barista *bptr = gc_baristas.alloc();

    bptr->age = 32;
    bptr->name = gc_chars.alloc(256); 

    char name[4] = { 'F', 'o', 'o', '\0' }; 
    for ( int i = 0; i < 4; ++i )
    {
        bptr->name[i] = name[i];
    }

    printf( "Barista %s is %d years old.\n", bptr->name, (int)bptr->age);
```

When objects instantiated from `cgc<T>` (such as `gc_integers`, `gc_chars`
and `gc_baristas` in this example) are destroyed, they safely free all
memory that they have allocated during their lifetime.
