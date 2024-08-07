
#ifndef __ARENA__
    #define __ARENA__
    #include<stdio.h>
    #include<stdlib.h>
    #define __ALLOCATOR_IMPL__
    #include "allocator.h"
    // *** ALLOCATORS ***  
    #ifndef __ALIGNMENT__
        #define __DEFAULT_ALIGNMENT__ (2 * sizeof(void *))
    #endif
    #define arena_alloc_init(a) (Allocator){arena_alloc, arena_free, a}
    #define is_power_of_two(x) ((x != 0) && ((x & (x - 1)) == 0))
    #ifndef __ARENA_IMPL__
            // *** ALLOCATORS ***  
            typedef struct Arena; 
            uintptr_t align_forward(uintptr_t ptr, size_t alignment)
            void *arena_alloc_aligned(Arena *arena, size_t size, size_t alignment);
            void* arena_alloc(Arena arena, size_t size, size_t alignment);
            void arena_free(size_t size, void *ptr, void *context);
            void arena_free_all(void *context);
            Arena arena_init(void *buffer, size_t size);
    #else
            #define DEFAULT_ALIGNMENT (2 * sizeof(void *))

            typedef struct {
                void *base;
                size_t size;
                size_t offset;
                size_t committed;
            } Arena;
            
            #define arena_alloc_init(a) (Allocator){arena_alloc, arena_free, a}
            
            #define is_power_of_two(x) ((x != 0) && ((x & (x - 1)) == 0))
            
            uintptr_t align_forward(uintptr_t ptr, size_t alignment) {
                uintptr_t p, a, modulo;
                if (!is_power_of_two(alignment)) {
                    return 0;
                }
            
                p = ptr;
                a = (uintptr_t)alignment;
                modulo = p & (a - 1);
            
                if (modulo) {
                    p += a - modulo;
                }
            
                return p;
            }
            
            void *arena_alloc_aligned(Arena *a, size_t size, size_t alignment) {
                uintptr_t curr_ptr = (uintptr_t)a->base + (uintptr_t)a->offset;
                uintptr_t offset = align_forward(curr_ptr, alignment);
                offset -= (uintptr_t)a->base;
            		
                if (offset + size > a->size) {
                    return 0;
                }
            
                a->committed += size;
                void *ptr = (uint8_t *)a->base + offset;
                a->offset = offset + size;
            
                return ptr;
            }
            
            void *arena_alloc(size_t size, void *context) {
                if (!size) {
                    return 0;
                }
                return arena_alloc_aligned((Arena *)context, size, DEFAULT_ALIGNMENT);
            }
            
            // Does nothing.
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
