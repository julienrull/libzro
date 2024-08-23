#ifndef __ZRO_ARGS__
    #define __ZRO_ARGS__
    #include "zro_alloc.h"
    #include "zro_dynarr.h"
    #include "zro_hmap.h"
    #ifndef __ZRO_ARGS_IMPL__
    #else 
        typedef struct _zro_cmd_t {
            const char* name;
            const char* description;
            zro_arg_t *args;
            zro_cmd_t *cmds;
        } zro_cmd_t;

        typedef enum _zro_arg_type_t {
            SHORT,
            LONG,
            POSITIONAL
        } zro_arg_type_t;

        typedef struct zro_arg_t {
            const char* positional;
            const char* option_short;
            const char* option_long;
            int required;
        } zro_cmd_t;

        

        zro_cmd_t zro_new_cmd(
                const char* name, 
                const char* description, 
        ) {
            return zro_cmd_t{
                .name = name,
                .description = description,
                .args = NULL,
                .cmds = NULL,

            }   
        }

        void zro_cmd_add_arg(zro_cmd_t cmd, zro_arg_t arg, Allocator *a) {
           if(!cmd.args) {
                cmd.args = array(*zro_arg_t, a);
           } 
           array_append(cmd.args, arg); 
        }

        void zro_cmd_add_sub_cmd(zro_cmd_t cmd, zro_cmd_t sub_cmd) {}

        zro_arg_type_t zro_get_arg_type(char *arg){
            return                                      \
                arg[0] == '-' && arg[1] == '-' ? LONG   \ 
                : arg[0] == '-' ? SHORT                 \
                : POSITIONAL;
        }

        char** zro_cmd_parse(zro_cmd_t cmd, int argc, char **args, Allocator *a) {
           char *arg = args[0];
           if(!cmd.name){ // If root cmd (name == NULL)
            // Extract cmd name from the path
            int len = strlen(arg);
            int i = len - 1;
            while(arg[i] != '/' && i > 0)
                i--;
            cmd.name = arg[i + 1]; 
           }
           // check if arg match cmd name
           if(strcmp(cmd.name, arg) == 0){
            // parse args
            zro_arg_t *current_arg = NULL; 
            for(int i = 1; i < argc; i++) 
            {
                char *arg = argv[i];
                zro_arg_type_t arg_type = zro_get_arg_type(arg);
                switch(arg_type) {
                    case POSITIONAL: {
                        // maybe value or sub_cmd
                    }break; 
                    case SHORT: {
                        // maybe stack
                    }break;
                    case LONG: {
                        // more trivial case
                    }break;
                }
            }
           }
        }
        
        
        void zro_cmd_parse_arg(char *arg, zro_arg_t *cmd_args) {
            
        }

        int zro_is_arg_exist(char *arg, zro_arg_t *cmd_args) {
           return 0; 
        }

        zro_arg_t *zro_get_required_args(zro_arg_t *cmd_args, Allocator *a) {
            zro_arg_t *req_args = array(zro_arg_t, a);
            for (int i = 0; i < array_len(cmd_args); i++) {
                zro_arg_t arg = cmd_args[i];
                if(arg.required) {
                    array_append(req_args, arg);
                }
            }      
            return req_args;
        }
        int zro_remove_arg(char *arg, zro_arg_t *cmd_args) {

        }

        static int __zro_is_flag(char *input) {
             return input != NULL ? (input[0] == '-' ? 1 : 0) : 0;
        }
    #endif
#endif
