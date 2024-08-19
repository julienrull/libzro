#define __ZRO_DYNARR_IMPL__
#include "../zro_test.h"
#include "../zro_dynarr.h"
int tests_run = 0;
static char * test_dynarr_init() {
    Allocator a = std_alloc_init();
    int* arr = array(int, &a);
    mu_assert("error, header.capacity != 16", array_header(arr)->capacity == 16);
    mu_assert("error, header.lenght != 0", array_header(arr)->lenght == 0);
    return 0;
}

static char * test_dynarr_append() {
    Allocator a = std_alloc_init();
    int* arr = array(int, &a);
    array_append(arr, 10);
    array_append(arr, 9);
    array_append(arr, 8);
    mu_assert("error, header.capacity != 16", array_header(arr)->capacity == 16);
    mu_assert("error, header.lenght != 3", array_header(arr)->lenght == 3);
    return 0;
}

static char * test_dynarr_append_resize() {
    Allocator a = std_alloc_init();
    int* arr = array(int, &a);
    for(int i = 0; i < 20; i++) array_append(arr, i);
    mu_assert("error, header.capacity != 32", array_header(arr)->capacity == 32);
    mu_assert("error, header.lenght != 20", array_header(arr)->lenght == 20);
    return 0;
}

static char * test_dynarr_indexes() {
    Allocator a = std_alloc_init();
    int* arr = array(int, &a);
    for(int i = 0; i < 20; i++) array_append(arr, i);
    for(int i = 0; i < 20; i++) mu_assert("error, arr[i] != i", arr[i] == i);
    return 0;
}

static char * test_dynarr_unordered_remove() {
    Allocator a = std_alloc_init();
    int* arr = array(int, &a);
    for(int i = 0; i < 20; i++) array_append(arr, i);
    array_unordered_remove(arr, 10);
    mu_assert("error, header.lenght != 19", array_header(arr)->lenght == 19);
    for(int i = 0; i < array_header(arr)->lenght; i++) {
        if(i == 10){
            mu_assert("error, arr[i] != 19", arr[i] == 19);
        }else {
            mu_assert("error, arr[i] != i", arr[i] == i);
        } 
    }
    return 0;
}

static char * test_dynarr_ordered_remove() {
    Allocator a = std_alloc_init();
    int* arr = array(int, &a);
    for(int i = 0; i < 20; i++) array_append(arr, i);
    array_ordered_remove(arr, 10);
    mu_assert("error, header.lenght != 19", array_header(arr)->lenght == 19);
    for(int i = 0; i < array_header(arr)->lenght; i++) {
        if(i >= 10){
            mu_assert("error, arr[i] != i + 1", arr[i] == i + 1);
        }else {
            mu_assert("error, arr[i] != i", arr[i] == i);
        } 
    }
    return 0;
}
static char * all_tests() {
    mu_run_test(test_dynarr_init);
    mu_run_test(test_dynarr_append);
    mu_run_test(test_dynarr_append_resize);
    mu_run_test(test_dynarr_indexes);
    mu_run_test(test_dynarr_unordered_remove);
    mu_run_test(test_dynarr_ordered_remove);
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
