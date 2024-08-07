#define __DYNARR_IMPL__
#include "../tests.h"
#include "../dynarr.h"
int tests_run = 0;
static char * test_dynarr_init() {
    Allocator a = std_alloc_init();
    int* arr = array(int, &a);
    ArrayHeader *header = array_header(arr);
    mu_assert("error, header.capacity != 16", header->capacity == 16);
    mu_assert("error, header.lenght != 0", header->lenght == 0);
    return 0;
}
static char * all_tests() {
    mu_run_test(test_dynarr_init);
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
