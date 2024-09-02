#ifndef ZROA
#define ZROA

#include <stdlib.h>
#include <strings.h>

// *** CONFIG ***

#ifndef ZROA_NO_SHORT_NAMES

#define alloc_t         zroa_alloc_t
#define make            zroa_make
#define del             zroa_free

#define std_alloc       zroa_std_alloc_init

#define arena_alloc     zroa_arena_alloc_init
#define arena_free      zroa_arena_free_all
#define arena_t         zroa_arena_t
#define arena_init      zroa_arena_init


#endif


#ifndef ZRO_ARENA_ALIGNMENT

#define ZRO_ARENA_ALIGNMENT (2 * sizeof(void *))

#endif

// *** UTILITIES ***
//
#define is_power_of_two(x) ((x != 0) && ((x & (x - 1)) == 0))

// *** ALLOCATORE ABSTRACTION ***

typedef struct {
    void *(*alloc)(size_t size, void *context);
    void (*free)(size_t size, void *ptr, void *context);
    void *(*realloc)(void *ptr, size_t old_size, size_t new_size, void *context);
    void *context;
} zroa_alloc_t;

#define zroa_make(T, n, a) ((T *)((a).alloc(sizeof(T) * n, (a).context)))
#define zroa_free(s, p, a) ((a).free(s, p, (a).context))

// *** STD ALLOCATOR ***
void *_zroa_std_alloc(size_t bytes, void *context) {
    (void)context;
    return malloc(bytes);
}
void _zroa_std_free(size_t size, void *ptr, void *context) {
    (void)ptr; (void)context;
    free(ptr);
}
void *_zroa_std_realloc(void *ptr, size_t old_size, size_t new_size, void *context){
    //(void)old_size;
    return realloc(ptr, new_size);
} 

#define zroa_std_alloc_init() (zroa_alloc_t){_zroa_std_alloc, _zroa_std_free, _zroa_std_realloc, NULL}

// *** ARENA ALLOCATOR ***


typedef struct {
    void *base;
    size_t size;
    size_t offset;
    size_t committed;
} zroa_arena_t;



uintptr_t _zroa_arena_align_forward(uintptr_t ptr, size_t alignment) {
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

void *_zroa_arena_alloc_aligned(zroa_arena_t *a, size_t size, size_t alignment) {
    uintptr_t curr_ptr = (uintptr_t)a->base + (uintptr_t)a->offset;
    uintptr_t offset = _zroa_arena_align_forward(curr_ptr, alignment);
    offset -= (uintptr_t)a->base;
		
    if (offset + size > a->size) {
        return 0;
    }

    a->committed += size;
    void *ptr = (char*)a->base + offset;
    a->offset = offset + size;

    return ptr;
}

void *_zroa_arena_alloc(size_t size, void *context) {
    if (!size) {
        return 0;
    }
    return _zroa_arena_alloc_aligned((zroa_arena_t *)context, size, ZRO_ARENA_ALIGNMENT);
}

void* _zroa_arena_realloc(void *ptr, size_t old_size, size_t new_size, void *context){
    void *new_mem = _zroa_arena_alloc(new_size, context);
    if (new_mem) 
        memcpy(new_mem, ptr, old_size);
        //if (h->a->free) {
        //    h->a->free(old_size, h, h->a->context);
        //}
    return new_mem;
}
// Does nothing.
void _zroa_arena_free(size_t size, void *ptr, void *context) {
    (void)ptr; (void)size; (void)context;
}

void zroa_arena_free_all(void *context) {
    zroa_arena_t *a = context;
    a->offset = 0;
    a->committed = 0;
}

zroa_arena_t zroa_arena_init(void *buffer, size_t size) {
    return (zroa_arena_t){.base = buffer, .size = size};
}


#define zroa_arena_alloc_init(a) (zroa_alloc_t){_zroa_arena_alloc, _zroa_arena_free, _zroa_arena_realloc, a}

#endif
