#define __ZRO_DYNARR_IMPL__
#define __ZRO_STRING_IMPL__
//#include <Windows.h>
#include <locale.h>
//#include <io.h>
//#include <fcntl.h>
#include "../zro_test.h"
#include "../zro_string.h"

int tests_run = 0;
static char * test_string_init() {
    //wchar_t message[] = L"Hello, é马克!\n";
    //const char *message = "Hello, é马克!";
    //DWORD written = 0;
    //WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), message, strlen(message), &written, NULL);
    
    setlocale(LC_ALL, "fr_FR.UTF-8");
    //SetConsoleCP(CP_UTF8);
    //SetConsoleOutputCP(CP_UTF8);
    Allocator a = std_alloc_init();
    String test = string_new(L"L'évènement de la vie", &a);
    string_println(test);
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
