#ifndef __ZRO_DYNARR__
    #define __ZRO_DYNARR__
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #define __ZRO_ALLOC_IMPL__
    #include "zro_alloc.h"
    #define ARRAY_INITIAL_CAPACITY 16
    #ifndef __ZRO_DYNARR_IMPL__
        #define array(T, a) array_init(sizeof(T), ARRAY_INITIAL_CAPACITY, a)
        typedef struct ArrayHeader;
        #define array_header(a) ((ArrayHeader *)(a) - 1)
        #define array_lenght(a) (array_header(a)->lenght)
        #define array_capacity(a) (array_header(a)->capacity)
        #define array_append(a, v) ( \
            (a) = array_ensure_capacity(a, 1, sizeof(v)), \
            (a)[array_header(a)->lenght] = (v), \
            &(a)[array_header(a)->lenght++])
        #define array_unordered_remove(a, i) do { \
            ArrayHeader *h = array_header(a); \
            if (i == h->lenght - 1) { \
                h->lenght -= 1; \
            } else if (h->lenght > 1) { \
                void *ptr = &a[i]; \
                void *last = &a[h->lenght - 1]; \
                h->lenght -= 1; \
                memcpy(ptr, last, sizeof(*a)); \
            } \
        } while (0);
        #define array_ordered_remove(a, i) do { \
            ArrayHeader *h = array_header(a); \
            if (i == h->lenght - 1) { \
                h->lenght -= 1; \
            } else if (h->lenght > 1) { \
                void *ptr = &a[i]; \
                memcpy(ptr, ptr + 1,  (h->lenght - i + 1) * sizeof(*a)); \
                h->lenght -= 1; \
            } \
        } while (0);
        #define array_pop_back(a) (array_header(a)->lenght -= 1)
        void *array_init(size_t item_size, size_t capacity, Allocator *a);
        void *array_ensure_capacity(void *a, size_t item_count, size_t item_size);
    #else
        #define array(T, a) array_init(sizeof(T), ARRAY_INITIAL_CAPACITY, a)
        typedef struct {
            size_t capacity;
            size_t lenght;
            size_t padding;
            Allocator *a;
        } ArrayHeader;
        #define array_header(a) ((ArrayHeader *)(a) - 1)
        #define array_lenght(a) (array_header(a)->lenght)
        #define array_capacity(a) (array_header(a)->capacity)
        #define array_append(a, v) ( \
            (a) = array_ensure_capacity(a, 1, sizeof(v)), \
            (a)[array_header(a)->lenght] = (v), \
            &(a)[array_header(a)->lenght++])
        #define array_unordered_remove(a, i) do { \
            ArrayHeader *h = array_header(a); \
            if (i == h->lenght - 1) { \
                h->lenght -= 1; \
            } else if (h->lenght > 1) { \
                void *ptr = &a[i]; \
                void *last = &a[h->lenght - 1]; \
                h->lenght -= 1; \
                memcpy(ptr, last, sizeof(*a)); \
            } \
        } while (0);
        #define array_ordered_remove(a, i) do { \
            ArrayHeader *h = array_header(a); \
            if (i == h->lenght - 1) { \
                h->lenght -= 1; \
            } else if (h->lenght > 1) { \
                void *ptr = &a[i]; \
                memmove(ptr, ptr + sizeof(*a),  (h->lenght - (i + 1)) * sizeof(*a)); \
                h->lenght -= 1; \
            } \
        } while (0);
        
        #define array_pop_back(a) (array_header(a)->lenght -= 1)
        
        void *array_init(size_t item_size, size_t capacity, Allocator *a) {
            void *ptr = 0;
            size_t size = item_size * capacity + sizeof(ArrayHeader);
            ArrayHeader *h = a->alloc(size, a->context);
        
            if (h) {
                h->capacity = capacity;
                h->lenght = 0;
                h->a = a;
                ptr = h + 1;
            }
        
            return ptr;
        }
        
        void *array_ensure_capacity(void *a, size_t item_count, size_t item_size) {
            ArrayHeader *h = array_header(a);
            size_t desired_capacity = h->lenght + item_count;
        
            if (h->capacity < desired_capacity) {
                size_t new_capacity = h->capacity * 2;
                while (new_capacity < desired_capacity) {
                    new_capacity *= 2;
                }
        
                size_t new_size = sizeof(ArrayHeader) + new_capacity * item_size;
                ArrayHeader *new_h = h->a->alloc(new_size, h->a->context);
        
                if (new_h) {
                    size_t old_size = sizeof(*h) + h->lenght * item_size;
                    memcpy(new_h, h, old_size);
        
                    if (h->a->free) {
                        h->a->free(old_size, h, h->a->context);
                    }
        
                    new_h->capacity = new_capacity;
                    h = new_h + 1;
                } else {
                    h = 0;
                }
            } else { h += 1; }
            return h;
        }
    #endif
#endif
