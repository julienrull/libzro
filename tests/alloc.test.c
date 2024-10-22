#include "../libzro.h"

int tests_run = 0;

static char * test_arena_init() {
    unsigned char *buffer = malloc(1024);
    arena_t arena = arena_init(buffer, 1024);
    alloc_t allocator = arena_alloc(&arena);
    mu_assert("error, &arena != allocator.context", &arena == allocator.context);
    mu_assert("error, buffer != ((arena_t*)(allocator.context))->base", buffer == ((arena_t*)(allocator.context))->base);
    mu_assert("error, ((arena_t*)(allocator.context))->size != 1024", ((arena_t*)(allocator.context))->size == 1024);
    mu_assert("error, ((arena_t*)(allocator.context))->offset != 0", ((arena_t*)(allocator.context))->offset == 0);
    mu_assert("error, ((arena_t*)(allocator.committed))-> != 0", ((arena_t*)(allocator.context))->committed == 0);
    return 0;
}

static char * test_arena_alloc() {
    unsigned char *buffer = malloc(1024);
    arena_t arena = arena_init(buffer, 1024);
    alloc_t allocator = arena_alloc(&arena);
    int *val = allocator.alloc(sizeof(int), allocator.context);
    *val = 555;
    mu_assert("error, val != (int*)arena.base", val == (int*)arena.base);
    mu_assert("error, arena.offset != sizeof(int)", arena.offset == sizeof(int));
    mu_assert("error, arena.committed != sizeof(int)", arena.committed == sizeof(int));
    mu_assert("error, *val != 555", *val == 555);
    int *val2 = allocator.alloc(sizeof(int), allocator.context);
    *val2 = 444;
    mu_assert("error, val2 != (int*)(arena.base + 16)", val2 == (int*)((char*)(arena.base) + 16));
    mu_assert("error, arena.offset != 16 + sizeof(int)", arena.offset == 16 + sizeof(int));
    mu_assert("error, arena.committed != sizeof(int) * 2", arena.committed == sizeof(int) * 2);
    mu_assert("error, *val2 != 444", *val2 == 444);
    return 0;
}

static char * test_arena_free_all() {
    unsigned char *buffer = malloc(1024);
    arena_t arena = arena_init(buffer, 1024);
    alloc_t allocator = arena_alloc(&arena);
    int *val = allocator.alloc(sizeof(int), allocator.context);
    int *val2 = allocator.alloc(sizeof(int), allocator.context);
    arena_free(allocator.context);
    mu_assert("error, arena.offset != 0", arena.offset == 0);
    mu_assert("error, arena.committed != 0", arena.committed == 0);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_arena_init);
    mu_run_test(test_arena_alloc);
    mu_run_test(test_arena_free_all);
    return 0;
}

int main(int argc, char **argv) {
    char *result = all_tests();
    if (result != 0) {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);
    return result != 0;
}
