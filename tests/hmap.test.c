#include "../libzro.h"
int tests_run = 0;
static char * test_hmap_init() {
    //const char test[11] = "Hello";
    //printf("%lu\n", sizeof(test));
    Allocator a = std_alloc_init();
    int *ages = hmap(int, &a);
    int age = 60;
    hmap_set(ages, "john", &age);
    int *john = hmap_get(ages, "john");
    printf("%d\n", *john);
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
