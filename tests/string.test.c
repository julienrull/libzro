#define __DYNARR_IMPL__
#define __STRING_IMPL__
#include "../tests.h"
#include "../string.h"
int tests_run = 0;
static char * test_string_init() {
    setlocale(LC_CTYPE, "");
    Allocator a = std_alloc_init();
    String test = string_new(L"L'évènement de la vie", &a);
    String test2 = string_cpy(test, &a);
    String test3 = string_concat((String[]){test, test2}, 2, &a);
    string_println(test3);
    //mu_assert("error, header.capacity != 16", array_header(arr)->capacity == 16);
    return 0;
}

static char * all_tests() {
    mu_run_test(test_string_init);
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
