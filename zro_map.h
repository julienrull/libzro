#ifndef __ZRO_MAP__
    #define __ZRO_MAP__
    #define __ZRO_ALLOC_IMPL__
    #include "zro_alloc.h"
    #ifndef __ZRO_MAP_IMPL__
    #else
        #define DEFAULT_HMAP_CAP 6000
        #define hmap(T, A) zro_new_hmap(sizeof(T), DEFAULT_HMAP_CAP, &A)
        #define hmap_set(C, K, V) C[zro_hash(K)] = V
        #define hmap_get(C, K) C[zro_hash(K)]
        unsigned long zro_hash(unsigned char *str)
        {
            unsigned long hash = 5381;
            int c;
            while (c = *str++)
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
            return hash % DEFAULT_HMAP_CAP;
        }
        void *zro_new_hmap(size_t e_size, size_t cap, Allocator *a) {
            void *ptr = a->alloc(cap, a->context);
            char *cursor = ptr; 
            for(int i = 0; i < cap; i++) {
                cursor = NULL; 
                cursor += i*e_size;
            }
            return ptr;
        }
    #endif
#endif
