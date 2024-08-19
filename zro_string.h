#ifndef __ZRO_STRING__
    #define __ZRO_STRING__ 
    #include "zro_type.h"
    #include "zro_dynarr.h"
    #define ZRO_STRING_FIRST 1
    #define ZRO_STRING_LAST  -1
    #define ZRO_STRING_ALL   0
    #ifndef __ZRO_STRING_IMPL__
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
            DWORD written = 0;
            while (i < array_lenght(str.str)){
                //wprintf(L"%lc", str.str[i]);
                WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), &str.str[i], 1, &written, NULL);
                i++;
            }
            //wprintf(L"\n", str.str[i]);
            WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), L"\n", 1, &written, NULL);
       }
    #endif
#endif
