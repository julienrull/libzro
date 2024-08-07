#ifndef __TESTS__
    #define __TESTS__
    #define mu_assert(message, test) do { if (!(test)) return message; } while (0)
    #define mu_run_test(test) do { char *message = test(); tests_run++; \
    extern int tests_run;
#endif
