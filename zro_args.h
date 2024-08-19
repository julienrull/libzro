#ifndef __ZRO_ARGS__
    #define __ZRO_ARGS__
    #include <string.h>
    #ifndef __ZRO_ARGS_IMPL__
    #else 
        typedef struct zro_cmd_t {
            const char* name;
            const char* description;
            const char* epilog;
        } zro_cmd_t;

        typedef struct zro_arg_t {
            const char* positional;
            const char* option_short;
            const char* option_long;
        } zro_cmd_t;
        
        zro_cmd_t zro_new_cmd(
                const char* name, 
                const char* description, 
                const char* epilog
        ) {
            zro_cmd_t cmd = zro_cmd_t{
                .name = name,
                .description = description,
                .epilog = epilog
            }   
        }




        static int __zro_is_flag(char *input) {
             return input != NULL ? (input[0] == '-' ? 1 : 0) : 0;
        }
    #endif
#endif
