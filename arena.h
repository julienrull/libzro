#include<stdio.h>
#include<stdlib.h>

#ifndef __ARENA_IMPL__
    // * Definitions only
    typedef struct Arena *Arena; 
    Arena arena_make(size_t size);
    void* arena_alloc(Arena arena, size_t size, size_t alignment);
    void arena_free(Arena arena);
#else
    #ifndef __ALIGNMENT__
        #define __ALIGNMENT__ (2 * sizeof(void *))
    #endif
    // * Implementations
    typedef struct Arena{
        void* base;
        size_t size;
        size_t offset;
        size_t committed;
    }*Arena;
    Arena arena_make(size_t size){
        if(!size){
            return 0;
        }
        Arena arena = malloc(sizeof(struct Arena));
        arena->size = size;
        arena->offset = 0;
        arena->committed = 0;
        arena->base = malloc(size);
        return arena;
    }
    void* arena_alloc(Arena arena, size_t size, size_t alignment){
        void* curr_ptr = arena->base + arena->offset; 
        size_t len = curr_ptr - arena->base;
        size_t modulo =  (len % alignment);
        if(len + size > arena->size) {
            return 0;
        }
        if(modulo) {
           curr_ptr += alignment - modulo; 
        }
        arena->offset += size;
        arena->committed += size;
        return curr_ptr;
    }
    void arena_free(Arena arena) {
       free(arena->base); 
       free(arena); 
    }
#endif
