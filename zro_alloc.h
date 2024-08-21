#ifndef __ZRO_ALLOC__
    #define __ZRO_ALLOC__
    #ifndef __ZRO_ALLOC_IMPL__
    #else
        #define make(T, n, a) ((T *)((a).alloc(sizeof(T) * n, (a).context)))
        #define release(s, p, a) ((a).free(s, p, (a).context))
        #define std_alloc_init() (Allocator){_std_alloc, _std_free, _std_realloc, NULL}
        // *** ALLOCATORS ***  
        typedef struct {
            void *(*alloc)(size_t size, void *context);
            void (*free)(size_t size, void *ptr, void *context);
            void *(*realloc)(void *ptr, size_t old_size, size_t new_size, void *context);
            void *context;
        } Allocator;
        void *_std_alloc(size_t bytes, void *context) {
            (void)context;
            return malloc(bytes);
        }
        void _std_free(size_t size, void *ptr, void *context) {
            (void)ptr; (void)context;
            free(ptr);
        }
        void *_std_realloc(void *ptr, size_t old_size, size_t new_size, void *context){
            //(void)old_size;
            return realloc(ptr, new_size);
        } 
    #endif
#endif
