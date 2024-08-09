#ifndef __STRING__
    #define __STRING__ 
    #include "type.h"
    #include "dynarr.h"
    #define STRING_FIRST 1
    #define STRING_LAST  -1
    #define STRING_ALL   0
    #ifndef __STRING_IMPL__
       typedef struct String;   
       String string_new(Rune *str, Allocator *a);
       //String string_cpy(String dest, String src);
       String string_concat(String *strs);
       String string_replace(const char *from, const char *to, int flag);
       size_t string_index_of(const char *str, int flag);
       void string_printf(String str);
    #else
       typedef struct {
           Rune     *str; 
       } String;   
       String string_new(Rune *str, Allocator *a) {
        String res = {0};  
        res.str = array(Rune, a);
        Rune *index = str;
        while (*index != L'\0') {
            array_append(res.str, *index); 
            index++;
        }
        return res;
       }
       void string_println(String str){
            int i = 0; 
            while (i != array_lenght(str.str)){
                wprintf(L"%lc", str.str[i]);
                i++;
            }
            wprintf(L"\n", str.str[i]);
       }
    #endif
#endif
