#include "jsonstring.h"

void json_string_init(t_json_string *str) {
    str->size = 0;
    str->capacity = 0;
    str->str = (char*) malloc(0);
}

void json_string_new_from_cstr(t_json_string* str, int N, char* c_str) {
    json_string_init(str);
    for(int i = 0; i < N; i++) {
        json_string_insert(str, c_str[i]);
    }
    json_string_insert(str, '\0');
}

void json_string_free(t_json_string *str) {
    free(str->str);
}

void json_string_grow_capacity(t_json_string *str) {
    str->capacity = str->capacity == 0 ? 1 : 2 * str->capacity;
    str->str = (char*) realloc(str->str, str->capacity * sizeof(char));
}

void json_string_insert(t_json_string *str, char c) {
    if(str->size == str->capacity) {
        json_string_grow_capacity(str);
    }
    str->str[str->size] = c;
    str->size++;
}