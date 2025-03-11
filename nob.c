// nob.c
#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#define NOB_EXPERIMENTAL_DELETE_OLD
#include "nob.h"


int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    Nob_Cmd cmd = {0};

    const char *program_name = shift(argv, argc);
    const char *command_name = "";
    if (argc > 0) command_name = shift(argv, argc);

    if (strcmp(command_name, "test") == 0) 
    {
        #ifdef _WIN32 
            nob_cmd_append(&cmd, "cl", "/nologo", ".\\zro_test.c", "/Fe.\\zro_test.exe");
        #else // UNIX 
            nob_cmd_append(&cmd, "gcc", "./zro_test.c", "-o", "./zro_test");
        #endif 
        if (!cmd_run_sync_and_reset(&cmd))  return 1;
        cmd_append(&cmd, "./zro_test");
        if (!cmd_run_sync_and_reset(&cmd))  return 1;
        nob_log(INFO, "--- %s finished ---", "./zro_test");
    }

    return 0;
}
