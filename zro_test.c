#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#define  ZRO_IMPLEMENTATION
#include "zro.h" 

typedef enum print_type_e print_type_e;

enum print_type_e {
    ASSERT_SIGNED_INTEGER,
    ASSERT_UNSIGNED_INTEGER,
    ASSERT_FLOAT,
    ASSERT_STRING
};

char assert_equal(bool condition, void *expect, void *actual, size_t elem_size, print_type_e format, char *file, char *func, int line)
{
    
    if(!condition)
    {
        printf("\033[0;31m[TEST] - [%s] %s() at line %d (FAILED)\n", file, func, line);
        switch(format)
        {
            case ASSERT_SIGNED_INTEGER:{
                intmax_t v_expect = 0;
                intmax_t v_actual = 0;
                memcpy(&v_expect, expect, elem_size);
                memcpy(&v_expect, expect, elem_size);
                printf("\texpected %lld got %lld\033[0m\n", v_expect, v_actual);
            }break;
            case ASSERT_UNSIGNED_INTEGER:{
                uintmax_t v_expect = 0;
                uintmax_t v_actual = 0;
                memcpy(&v_expect, expect, elem_size);
                memcpy(&v_expect, expect, elem_size);
                printf("\texpected %lld got %lld\033[0m\n", v_expect, v_actual);
            }break;
            case ASSERT_FLOAT:{
                double v_expect = 0;
                double v_actual = 0;
                memcpy(&v_expect, expect, elem_size);
                memcpy(&v_expect, expect, elem_size);
                printf("\texpected %f got %f\033[0m\n", v_expect, v_actual);
            }break;
            case ASSERT_STRING:{
                char *v_expect = (char*)expect;
                char *v_actual = (char*)actual;
                printf("\texpected %s got %s\033[0m\n", v_expect, v_actual);
            }break;
        } 
        return 0;
    } 
    return 1;
}

#define assert_equal_int(C, E, A) (assert_equal((C), (&E), (&A), sizeof((E)), ASSERT_SIGNED_INTEGER, __FILE__, __func__, __LINE__))
#define assert_equal_uint(C, E, A) (assert_equal((C), (&E), (&A), sizeof((E)), ASSERT_USIGNED_INTEGER, __FILE__, __func__, __LINE__))
#define assert_equal_float(C, E, A) (assert_equal((C), (&E), (&A), sizeof((E)), ASSERT_FLOAT, __FILE__, __func__, __LINE__))
#define assert_equal_string(C, E, A) (assert_equal((C), (E), (A), sizeof((E)), ASSERT_STRING, __FILE__, __func__, __LINE__))


bool test_zro_dynarr()
{
    Allocator allocator = ZRO_DEFAULT_ALLOCATOR();
    size_t expect = 24;
    int *vals = zro_dynarr_init(&allocator, expect, sizeof(int));
    if(!assert_equal_int(zro_dynarr_cap(vals) == expect, expect, zro_dynarr_cap(vals))) return false;
    expect = 0;
    if(!assert_equal_int(zro_dynarr_len(vals) == expect, expect, zro_dynarr_len(vals))) return false;
    for(int i = 0; i < zro_dynarr_cap(vals); i++)
    {
        zro_dynarr_push(vals, i);
        if(!assert_equal_int(zro_dynarr_len(vals) == (i + 1), i + 1, zro_dynarr_len(vals))) return false;
    }
    for(int i = 0; i < zro_dynarr_len(vals); i++)
    {
        if(!assert_equal_int(vals[i] == i, i, vals[i])) return false;
    }
    zro_dynarr_pop(vals);
    expect = 23;
    if(!assert_equal_int(zro_dynarr_len(vals) == expect, expect, zro_dynarr_len(vals))) return false;
    expect = 22;
    if(!assert_equal_int(vals[zro_dynarr_len(vals) - 1] == expect, expect, vals[zro_dynarr_len(vals) - 1])) return false;
    zro_dynarr_unordered_del(vals, 2);
    expect = 22;
    if(!assert_equal_int(zro_dynarr_len(vals) == expect, expect, zro_dynarr_len(vals))) return false;
    if(!assert_equal_int(vals[2] == expect, expect, vals[2])) return false;
    zro_dynarr_ordered_del(vals, 2);
    expect = 21;
    if(!assert_equal_int(zro_dynarr_len(vals) == expect, expect, zro_dynarr_len(vals))) return false;
    expect = 3;
    if(!assert_equal_int(vals[2] == expect, expect, vals[2])) return false;
    return 1;
}

bool test_zro_arena()
{
    Arena arena;
    char memory[1024];
    int *lot_of_int[1024] = {0};
    int running = 1;
    int i = 0;

    arn_init(&arena, memory, 1024);

    while(running && i < 1024)
    {
        lot_of_int[i] = (int*)arn_alloc(&arena, sizeof(int));
        if(lot_of_int[i] != NULL)
        {
            *lot_of_int[i] = i+1;
            i += 1;
        }else{
            running = 0;
        }
    }

    size_t expect_len = 64;
    if(!assert_equal_int(i == expect_len, expect_len, i)) return false;
    return 1;
}


int main()
{
    if(!test_zro_arena()) return EXIT_FAILURE;
    printf("\033[0;32m[TEST] - test_zro_arena (OK)\033[0m\n");

    if(!test_zro_dynarr()) return EXIT_FAILURE;
    printf("\033[0;32m[TEST] - test_zro_dynarr (OK)\033[0m\n");
    return 0;
}
