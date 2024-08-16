#ifndef cufo_json_string_h
#define cufo_json_string_h

#include <stdlib.h>

typedef struct _json_string {
    int size;
    int capacity;
    char *str;
} t_json_string;

void json_string_init(t_json_string *str);
void json_string_new_from_cstr(t_json_string* str, int N, char* c_str);
void json_string_free(t_json_string *str);
void json_string_grow_capacity(t_json_string *str);
void json_string_insert(t_json_string *str, char c);

#endif