#define __ZRO_MAP_IMPL__
#include <stdio.h>
#include "../zro_map.h"
#include "../zro_test.h"

int tests_run = 0;
static char * test_hmap_init() {
    int **ages = hmap(int, std_alloc_init());
    int john = 60;
    hmap_set(ages, "john", &john);
    printf("%d\n", *hmap_get(ages, "john"));
    //mu_assert("error, header.capacity != 16", array_header(arr)->capacity == 16);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_hmap_init);
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
