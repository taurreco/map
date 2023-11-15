# map
A simple open address hashmap container library

# Usage & Lifetimes
This implementation of a hashmap only accepts strings as keys.  Keys will be duplicated and managed by the hashmap.  The value of this data structure is effectivley a tagged union.  Values can be <= 64 bit literals (int, long, float, etc) or pointers to more complicated data.  The hashmap constructor accepts a function pointer as an argument which acts as the destructor for the value data type, and this will be invoked upon the destruction of the hashmap or removal of the key-value pair from the hashmap.  It is undefined behavior if the library user free's the data pointed to by a value inside the hashmap.  For simple values a 0 can be passed into the function pointer argument of the hasmamp constructor indicating it will not free the data.
