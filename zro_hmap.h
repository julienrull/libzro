#ifndef __ZRO_HMAP__
    #define __ZRO_HMAP__
    #include <string.h>
    #ifndef __ZRO_HMAP_IMPL__
    #else
        #define HMAP_ID_SIZE 16
        typedef struct _hmap_val_t {
            char id[HMAP_ID_SIZE];
            void *value;
        } hmap_val_t;

        #define hmap(T, a) zro_hmap_init(sizeof(T), a)
        #define hmap_set(H, k, v) do { \
                hmap_val_t *hmh = (hmap_val_t*)( ((char*)H) - (sizeof(hmap_val_t) - sizeof(void*)));\
                size_t index = zro_hash(k, array_capacity(hmh));\
                int i = index;\
                int found = 0;\
                while (i < array_capacity(hmh) && !found) {\
                    if(strcmp(hmh[i].id, k) == 0 || !*hmh[i].id) {\
                        index = i;\
                        found = 1;\
                    }\
                    i++;\
                }\
                if(!found) {\
                    i = index;\
                    while (i >= 0 && !found) {\
                        if(strcmp(hmh[i].id, k) == 0 || !*hmh[i].id) {\
                            index = i;\
                            found = 1;\
                        }\
                        i--;\
                    }\
                }\
                if(found) { \
                    printf("???\n");\
                    hmh[index].value = v;\
                }\
        }while(0);

        #define hmap_get(H, k) ((hmap_val_t*)((char*)H - HMAP_ID_SIZE))[zro_hash(k, 16)].value
        
            //if (!strcpm(H[zro_hash(k, array_capacity(H))].id, key)){
            //    H[zro_hash(k, array_capacity(H))].value = &v;  
            //} 
        void *zro_hmap_init(size_t val_size, Allocator *a) {
            hmap_val_t *hm = array_init(sizeof(hmap_val_t), ARRAY_INITIAL_CAPACITY, a);
            for(int i = 0; i < array_capacity(hm); i++){
                memset(hm[i].id, 0, HMAP_ID_SIZE);
                hm[i].value = 0;
            }
            return ((char*)hm) + HMAP_ID_SIZE;
        }
        unsigned long zro_hash(unsigned char *str, size_t cap)
        {
            size_t hash = 5381;
            int c;
            while (c = *str++)
                hash = ((hash << 5) + hash) + c;
            return hash % cap;
        }
    #endif
#endif
