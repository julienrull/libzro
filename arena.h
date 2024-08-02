#include<stdio.h>
#include<stdlib.h>

#ifndef __ARENA_IMPL__
    // * Definitions only
    typedef struct Arena *Arena; 
    Arena arena_make(size_t size);
    void* arena_alloc(Arena arena, size_t size);
    void arena_free(Arena arena);
#else
    // * Implementations
    typedef struct Arena{
        void* base;
        size_t size;
        size_t offset;
        size_t committed;
    }*Arena;
    Arena arena_make(size_t size){
        Arena arena = malloc(sizeof(struct Arena));
        arena->size = size;
        arena->offset = 0;
        arena->committed = 0;
        arena->base = malloc(size);
        return arena;
    }
    void* arena_alloc(Arena arena, size_t size){
        void* mem = arena->base + arena->offset; 
        arena->offset += size;
        arena->committed += size;
        return mem;
    }
    void arena_free(Arena arena) {
       free(arena->base); 
       free(arena); 
    }
#endif
