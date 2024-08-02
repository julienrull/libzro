#define __ARENA_IMPL__
#include"arena.h"


int main() {
    Arena arena = arena_make(1024);
    int* val = arena_alloc(arena, sizeof(int));
    *val = 666;
    int* val2 = arena_alloc(arena, sizeof(int));
    *val2 = 555;
    printf("val: %d, val2: %d\n", *val, *val2);
    arena_free(arena);
    return 0;
}
