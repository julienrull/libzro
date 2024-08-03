#include<stdio.h>
#include<stdlib.h>

#ifndef __ZSTD__
    #define __ZSTD__
    // *** ALLOCATORS ***  
    #ifndef __ALIGNMENT__
        #define __DEFAULT_ALIGNMENT__ (2 * sizeof(void *))
    #endif
    #define make(T, n, a) ((T *)((a).alloc(sizeof(T) * n, (a).context)))
    #define release(s, p, a) ((a).free(s, p, (a).context))
    #define arena_alloc_init(a) (Allocator){arena_alloc, arena_free, a}
    #define is_power_of_two(x) ((x != 0) && ((x & (x - 1)) == 0))
    // *** TESTS ***  
    #define mu_assert(message, test) do { if (!(test)) return message; } while (0)
    #define mu_run_test(test) do { char *message = test(); tests_run++; \
    extern int tests_run;
    #ifndef __ZSTD_IMPL__
            // *** CONTEXT ***  
            typedef struct Context;
            // *** LOGGER ***  
            // *** ALLOCATORS ***  
            typedef struct Allocator;
            typedef struct Arena; 
            void *arena_alloc_aligned(Arena *arena, size_t size, size_t alignment);
            void* arena_alloc(Arena arena, size_t size, size_t alignment);
            void arena_free(size_t size, void *ptr, void *context);
            void arena_free_all(void *context);
            Arena arena_init(void *buffer, size_t size);
    #else
            // *** ALLOCATORS ***  
            typedef struct {
                void *(*alloc)(size_t size, void *context);
                void (*free)(size_t size, void *ptr, void *context);
                void *context;
            } Allocator;
            typedef struct {
                void* base;
                size_t size;
                size_t offset;
                size_t committed;
            } Arena;
            void *arena_alloc_aligned(Arena *arena, size_t size, size_t alignment){
                void *curr_ptr = arena->base + arena->offset; 
                size_t len = curr_ptr - arena->base;
                size_t modulo =  (len % alignment);
                if(!is_power_of_two(alignment)){
                    return 0;
                }
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
            void *arena_alloc(size_t size, void *context){
                if(!size) {
                 return 0;
                } 
                return arena_alloc_aligned((Arena *)context, size, __DEFAULT_ALIGNMENT__);
            } 
            void arena_free(size_t size, void *ptr, void *context) {
                (void)ptr; (void)size; (void)context;
            }
            void arena_free_all(void *context) {
                Arena *a = context;
                a->offset = 0;
                a->committed = 0;
            }
            Arena arena_init(void *buffer, size_t size) {
                return (Arena){.base = buffer, .size = size};
            }
    #endif
#endif
