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
       void string_printf(String str, Allocator *a);
       String string_cpy(String src, Allocator *a);
       String string_concat(String *strs, Allocator *a);
       //String string_replace(const char *from, const char *to, int flag);
       //size_t string_index_of(const char *str, int flag);
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
       String string_cpy(String src, Allocator *a){
            String res = string_new(L"", a);  
            int i = 0; 
            while (i < array_lenght(src.str)){
                array_append(res.str, src.str[i]); 
                i++;
            }
            return res;
       }
       String string_concat(String *strs, size_t arr_size, Allocator *a){
            String res = string_new(L"", a);
            for(int i = 0; i < arr_size; i++) {
                String str = strs[i];
                for(int j = 0; j < array_lenght(str.str); j++) {
                    array_append(res.str, str.str[j]);
                }  
            }  
            return res;
       }
       void string_println(String str){
            int i = 0; 
            while (i < array_lenght(str.str)){
                wprintf(L"%lc", str.str[i]);
                i++;
            }
            wprintf(L"\n", str.str[i]);
       }
    #endif
#endif
