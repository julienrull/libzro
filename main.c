#define __ARENA_IMPL__
#include"arena.h"
#include <stdint.h>

int main() {
    Arena arena = arena_make(1024);
    int* val = arena_alloc(arena, sizeof(int), __ALIGNMENT__);
    size_t i = 1;
    while(val != 0) {
        val = arena_alloc(arena, sizeof(int), __ALIGNMENT__);
        i++;
        printf("%lu - %lu\n", i, arena->committed);
    }
    printf("DONE\n");
    return 0;
}
