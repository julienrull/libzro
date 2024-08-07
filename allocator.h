#ifndef __ALLOCATOR__
    #ifndef __ALLOCATOR_IMPL__
        #define make(T, n, a) ((T *)((a).alloc(sizeof(T) * n, (a).context)))
        #define release(s, p, a) ((a).free(s, p, (a).context))
        #define std_alloc_init(a) (Allocator){arena_alloc, arena_free, 0}
        typedef struct Allocator;
        void *my_alloc(size_t bytes, void *context); 
        void *my_free(size_t bytes, void *ptr, void *context); 
    #else
        // *** ALLOCATORS ***  
        typedef struct {
            void *(*alloc)(size_t size, void *context);
            void (*free)(size_t size, void *ptr, void *context);
            void *context;
        } Allocator;
        void *my_alloc(size_t bytes, void *context) {
            (void)context;
            return malloc(bytes);
        }
        void *my_free(size_t bytes, void *ptr, void *context) {
            (void)ptr; (void)context;
            free(ptr);
        }
    #endif
#endif
