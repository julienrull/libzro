/* zro.h - v0.0.1 - public domain helper library - Julien Rull 2025
 
   This is a single-header-file library that provides easy-to-use
   allocators and data structures for C also works in C++.

   --> Inspired by Sean Barrett's STB library : https://github.com/nothings/stb

   To use this library, do this in *one* C or C++ file:
      #define ZRO_IMPLEMENTATION
      #include "ZRO.h"

TABLE OF CONTENTS

 Table of Contents
 Compile-time options

COMPILE-TIME OPTIONS

  #define ZRO_NO_SHORT_NAMES

     This flag needs to be set globally.

     By default zro exposes shorter function names that are not qualified
     with the "zro_" prefix. If these names conflict with the names in your
     code, define this flag.

  #define ZRO_DEFAULT_ALIGNMENT

  #define ZRO_DEFAULT_ALLOCATOR
    
*/

#ifndef ZRO_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define ZRO_API

#ifndef ZRO_NO_SHORT_NAMES
    #define  arn_init    zro_arena_init
    #define  arn_alloc   zro_arena_alloc
    #define  arn_resize  zro_arena_resize
    #define  arn_free    zro_arena_free_all
    #define  arn_begin   zro_temp_arena_memory_begin
    #define  arn_end     zro_temp_arena_memory_end
#endif // ZRO_NO_SHORT_NAMES

#define ZRO_DEFAULT_ALIGNMENT 16
#define ZRO_DEFAULT_ALLOCATOR() zro_create_std_allocator()

#define zro_dynarr_header(A) ((ArrayHeader*)(((unsigned char*)(A)) - sizeof(ArrayHeader))) 
#define zro_dynarr_cap(A) (zro_dynarr_header((A))->cap) 
#define zro_dynarr_len(A) (zro_dynarr_header((A))->len) 

#define zro_dynarr_push(A, E) \
    if(zro_dynarr_ensure_cap((A), 1)) { \
        zro_dynarr_header((A))->len += 1; \
        (A)[zro_dynarr_len((A)) - 1] = (E); \
    }

#define zro_dynarr_unordered_del(A, I) \
    if((I) < zro_dynarr_len((A)) && (I) > 0) { \
        (A)[(I)] = (A)[zro_dynarr_len((A)) - 1]; \
        zro_dynarr_header((A))->len -= 1; \
    }

#define zro_dynarr_ordered_del(A, I) \
    if((I) < zro_dynarr_len((A)) && (I) > 0) { \
        if((I) + 1 >= zro_dynarr_len((A))){ \
            zro_dynarr_unordered_del(A, I) \
        } else { \
            memmove(&(A)[(I)], &(A)[(I) + 1], zro_dynarr_len((A)) - ((I) + 1));\
            zro_dynarr_header((A))->len -= 1; \
        } \
    }

#define zro_dynarr_pop(A) zro_dynarr_header((A))->len -= 1 

//
// :STRUCT DECLARATIONS
//
typedef struct Allocator Allocator;
typedef struct Arena Arena;
typedef struct Temp_Arena_Memory Temp_Arena_Memory;
typedef struct ArrayHeader ArrayHeader;

//
// :API DECLARATIONS
//
ZRO_API Allocator zro_create_std_allocator();
ZRO_API Allocator zro_create_arena_allocator(Arena *a);
ZRO_API void zro_arena_init(Arena *a, void *backing_buffer, size_t backing_buffer_length);
ZRO_API void *zro_arena_alloc(Arena *arena, size_t size);
ZRO_API void *zro_arena_realloc(Arena *arena, void *ptr, size_t new_size);
ZRO_API void *zro_arena_resize(Arena *a, void *old_memory, size_t old_size, size_t new_size);
ZRO_API void zro_arena_free(Arena *a);
ZRO_API Temp_Arena_Memory zro_temp_arena_memory_begin(Arena *a);
ZRO_API void zro_temp_arena_memory_end(Temp_Arena_Memory temp);
ZRO_API void *zro_dynarr_init(Allocator *allocator, size_t cap, size_t elem_size);

//
// :HELPERS DECLARATIONS
//

void *zro_allocator_std_alloc(Allocator *self, size_t size);
void zro_allocator_std_free(Allocator *self, void* ptr);
void *zro_allocator_std_realloc(Allocator *self, void* ptr, size_t new_size);
void *zro_allocator_arena_alloc(Allocator *self, size_t size);
void zro_allocator_arena_free(Allocator *self, void* ptr);
void *zro_allocator_arena_realloc(Allocator *self, void* ptr, size_t new_size);
bool zro_is_power_of_two(uintptr_t x);
uintptr_t zro_align_forward(uintptr_t ptr, size_t align);
void *zro_arena_alloc_align(Arena *a, size_t size, size_t align);
void *zro_arena_resize_align(Arena *a, void *old_memory, size_t old_size, size_t new_size, size_t align);
bool zro_dynarr_ensure_cap(void *a, size_t elem_size);

//ZRO_API void *zro_dynarr_push(void *buf, void *elem, size_t elem_size);

#ifdef ZRO_IMPLEMENTATION

//
// :STRUCT DEFINITIONS
//

struct Allocator {
   void *context; 
   void* (*alloc)(Allocator *self, size_t size);
   void  (*free)(Allocator *self, void* ptr);
   void* (*realloc)(Allocator *self, void* ptr, size_t new_size);
};

struct Arena {
	unsigned char *buf;
	size_t         buf_len;
	size_t         prev_offset; // This will be useful for later on
	size_t         curr_offset;
};

struct Temp_Arena_Memory {
	Arena *arena;
	size_t prev_offset;
	size_t curr_offset;
};

struct ArrayHeader {
    Allocator *allocator;
    size_t cap;
    size_t len;
    unsigned char buf[0];
};

//
// :API IMPLEMENTATION
//
ZRO_API Allocator zro_create_arena_allocator(Arena *a)
{
    return (Allocator){
        .context    = a,
        .alloc      = zro_allocator_arena_alloc,
        .free       = zro_allocator_arena_free,
        .realloc    = zro_allocator_arena_realloc
    };
}
ZRO_API Allocator zro_create_std_allocator()
{
    return (Allocator){
        .context    = NULL,
        .alloc      = zro_allocator_std_alloc,
        .free       = zro_allocator_std_free,
        .realloc    = zro_allocator_std_realloc
    };
}

ZRO_API void zro_arena_init(Arena *a, void *backing_buffer, size_t backing_buffer_length) {
	a->buf = (unsigned char *)backing_buffer;
	a->buf_len = backing_buffer_length;
	a->curr_offset = 0;
	a->prev_offset = 0;
}

ZRO_API void *zro_arena_alloc(Arena *a, size_t size)
{
    return zro_arena_alloc_align(a, size, ZRO_DEFAULT_ALIGNMENT);
}

ZRO_API void *zro_arena_realloc(Arena *arena, void* ptr, size_t new_size)
{
    (void)arena; (void)ptr;(void)new_size;
    return NULL;
}

ZRO_API void *zro_arena_resize(Arena *a, void *old_memory, size_t old_size, size_t new_size) {
	return zro_arena_resize_align(a, old_memory, old_size, new_size, ZRO_DEFAULT_ALIGNMENT);
}

ZRO_API void zro_arena_free(Arena *a) {
	a->curr_offset = 0;
	a->prev_offset = 0;
}

ZRO_API Temp_Arena_Memory zro_temp_arena_memory_begin(Arena *a)
{
	Temp_Arena_Memory temp;
	temp.arena = a;
	temp.prev_offset = a->prev_offset;
	temp.curr_offset = a->curr_offset;
	return temp;
}

ZRO_API void zro_temp_arena_memory_end(Temp_Arena_Memory temp) 
{
	temp.arena->prev_offset = temp.prev_offset;
	temp.arena->curr_offset = temp.curr_offset;
}

ZRO_API void *zro_dynarr_init(Allocator *allocator, size_t cap, size_t elem_size)
{
    size_t total_size  = sizeof(ArrayHeader) + cap * elem_size;
    ArrayHeader *header = allocator->alloc(allocator, total_size); 
    header->len = 0;
    header->cap = cap;
    return header->buf;
}

/*
ZRO_API void *zro_dynarr_push(void *buf, void *elem, size_t elem_size)
{
    ArrayHeader* header = zro_dynarr_header(buf);
    size_t cap = zro_dynarr_cap(buf);
    size_t len = zro_dynarr_len(buf);
    unsigned char* b_buf = buf;
    if(len + elem_size < cap)
    {
        header->len += 1;
        return memcpy(buf + header->len, elem, elem_size);
    }
    return NULL;
}
*/

//
// :HELPERS IMPLEMENTATION
//

void *zro_allocator_std_alloc(Allocator *self, size_t size)
{
   (void)self;
   return malloc(size);
}
void zro_allocator_std_free(Allocator *self, void* ptr)
{
   (void)self;
   free(ptr);
}
void *zro_allocator_std_realloc(Allocator *self, void* ptr, size_t new_size)
{
   (void)self;
   return realloc(ptr, new_size);
}


void *zro_allocator_arena_alloc(Allocator *self, size_t size)
{
    return zro_arena_alloc((Arena*)self->context, size);
}
void zro_allocator_arena_free(Allocator *self, void* ptr)
{
    (void)ptr;
    zro_arena_free((Arena*)self->context);
}

void *zro_allocator_arena_realloc(Allocator *self, void* ptr, size_t new_size)
{
    return zro_arena_realloc((Arena*)self->context, ptr, new_size);
}

bool zro_is_power_of_two(uintptr_t x) {
	return (x & (x-1)) == 0;
}

uintptr_t zro_align_forward(uintptr_t ptr, size_t align) {
	uintptr_t p, a, modulo;

	assert(zro_is_power_of_two(align));

	p = ptr;
	a = (uintptr_t)align;
	// Same as (p % a) but faster as 'a' is a power of two
	modulo = p & (a-1);

	if (modulo != 0) {
		// If 'p' address is not aligned, push the address to the
		// next value which is aligned
		p += a - modulo;
	}
	return p;
}

void *zro_arena_alloc_align(Arena *a, size_t size, size_t align) {
	// Align 'curr_offset' forward to the specified alignment
	uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
	uintptr_t offset = zro_align_forward(curr_ptr, align);
	offset -= (uintptr_t)a->buf; // Change to relative offset

	// Check to see if the backing memory has space left
	if (offset+size <= a->buf_len) {
		void *ptr = &a->buf[offset];
		a->prev_offset = offset;
		a->curr_offset = offset+size;

		// Zero new memory by default
		memset(ptr, 0, size);
		return ptr;
	}
	// Return NULL if the arena is out of memory (or handle differently)
	return NULL;
}


void *zro_arena_resize_align(Arena *a, void *old_memory, size_t old_size, size_t new_size, size_t align) {
	unsigned char *old_mem = (unsigned char *)old_memory;

	assert(zro_is_power_of_two(align));

	if (old_mem == NULL || old_size == 0) {
		return zro_arena_alloc_align(a, new_size, align);
	} else if (a->buf <= old_mem && old_mem < a->buf+a->buf_len) {
		if (a->buf+a->prev_offset == old_mem) {
			a->curr_offset = a->prev_offset + new_size;
			if (new_size > old_size) {
				// Zero the new memory by default
				memset(&a->buf[a->curr_offset], 0, new_size-old_size);
			}
			return old_memory;
		} else {
			void *new_memory = zro_arena_alloc_align(a, new_size, align);
			size_t copy_size = old_size < new_size ? old_size : new_size;
			// Copy across old memory to the new memory
			memmove(new_memory, old_memory, copy_size);
			return new_memory;
		}

	} else {
		assert(0 && "Memory is out of bounds of the buffer in this arena");
		return NULL;
	}

}


bool zro_dynarr_ensure_cap(void *a, size_t elem_size)
{
    bool has_cap = true;
    if(zro_dynarr_len(a) + elem_size > zro_dynarr_cap(a)) 
    {
        ArrayHeader *header = zro_dynarr_header(a); 
        size_t new_size = header->cap * 2 + elem_size;
        Allocator *allocator = header->allocator;
        ArrayHeader *new_header = allocator->realloc(allocator, header, new_size); 
        if(!new_header)
            has_cap = false;

    }
    return has_cap;
}

#endif // ZRO_IMPLEMENTATION
#endif // ZRO_H_
