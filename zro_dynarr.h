#ifndef ZRODS
#define ZRODS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zro_alloc.h"

// *** CONFIG ***
#ifndef ZRODS_NO_SHORT_NAMES

#define array_header_t  zrods_array_header_t
#define array           zrods_array
#define ahead           zrods_array_header
#define alen            zrods_array_lenght
#define acap            zrods_array_capacity
#define apush           zrods_array_append
#define aundel          zrods_array_unordered_remove
#define aordel          zrods_array_ordered_remove
#define apop            zrods_array_pop_back

#endif

#define ARRAY_INITIAL_CAPACITY 16

typedef struct {
    size_t capacity;
    size_t lenght;
    // NOTE : is padding still necessary since Allocator is now aligned
    // thanks to realloc function pointer ? 
    // --> size_t padding; <--
    zroa_alloc_t *a;
} zrods_array_header_t;

#define zrods_array(T, a) _zrods_array_init(sizeof(T), ARRAY_INITIAL_CAPACITY, a)
#define zrods_array_header(a) ((zrods_array_header_t *)(a) - 1)
#define zrods_array_lenght(a) (zrods_array_header(a)->lenght)
#define zrods_array_capacity(a) (zrods_array_header(a)->capacity)
#define zrods_array_append(a, v) ( \
    (a) = _zrods_array_ensure_capacity(a, 1, sizeof(v)), \
    (a)[zrods_array_header(a)->lenght] = (v), \
    &(a)[zrods_array_header(a)->lenght++])
#define zrods_array_unordered_remove(a, i) do { \
    zrods_array_header_t *h = zrods_array_header(a); \
    if (i == h->lenght - 1) { \
        h->lenght -= 1; \
    } else if (h->lenght > 1) { \
        void *ptr = &a[i]; \
        void *last = &a[h->lenght - 1]; \
        h->lenght -= 1; \
        memcpy(ptr, last, sizeof(*a)); \
    } \
} while (0);
#define zrods_array_ordered_remove(a, i) do { \
    zrods_array_header_t *h = zrods_array_header(a); \
    if (i == h->lenght - 1) { \
        h->lenght -= 1; \
    } else if (h->lenght > 1) { \
        void *ptr = &a[i]; \
        memmove(ptr, (char*)(ptr) + sizeof(*a),  (h->lenght - (i + 1)) * sizeof(*a)); \
        h->lenght -= 1; \
    } \
} while (0);

#define zrods_array_pop_back(a) (zrods_array_header(a)->lenght -= 1)

void *_zrods_array_init(size_t item_size, size_t capacity, zroa_alloc_t *a) {
    void *ptr = 0;
    size_t size = item_size * capacity + sizeof(zrods_array_header_t);
    zrods_array_header_t *h = a->alloc(size, a->context);

    if (h) {
        h->capacity = capacity;
        h->lenght = 0;
        h->a = a;
        ptr = h + 1;
    }

    return ptr;
}

void *_zrods_array_ensure_capacity(void *a, size_t item_count, size_t item_size) {
    zrods_array_header_t *h = zrods_array_header(a);
    size_t desired_capacity = h->lenght + item_count;

    if (h->capacity < desired_capacity) {
        size_t new_capacity = h->capacity * 2;
        while (new_capacity < desired_capacity) {
            new_capacity *= 2;
        }
        size_t new_size = sizeof(zrods_array_header_t) + new_capacity * item_size;
        size_t old_size = sizeof(*h) + h->lenght * item_size;
        zrods_array_header_t *new_h = h->a->realloc(h, old_size, new_size, h->a->context);
        if (new_h) {
            new_h->capacity = new_capacity;
            h = new_h + 1;
        } else {
            h = 0;
        }
    } else { h += 1; }
    return h;
}
#endif
