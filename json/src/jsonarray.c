#include "jsonarray.h"

void json_array_init(t_json_array *array) {
    array->allocated = 0;
    array->size = 0;
    array->capacity = 0;
    array->value_type = (e_json_value_type*) malloc(0);
    array->values = (t_json_value*) malloc(0);
}

void json_array_free(t_json_array *array) {
    int i;
    for(i = 0; i < array->size; i++) {
        // printf("Freeing a %s.\n", json_value_type_str(array->value_type[i]));
        switch(array->value_type[i]) {
            case jsonSTRING:
                json_string_free(&(array->values[i].sval));
                break;
            case jsonARRAY:
                json_array_free(array->values[i].aval);
                break;
            case jsonJSON:
                json_free(array->values[i].jval);
        }
    }
    free(array->value_type);
    free(array->values);
    if(array->allocated) {
        free(array);
    }
}

void json_array_grow_capacity(t_json_array *array) {
    array->capacity = array->capacity == 0 ? 1 : 2 * array->capacity;
    array->value_type = (e_json_value_type*) realloc(array->value_type, array->capacity * sizeof(e_json_value_type));
    array->values = (t_json_value*) realloc(array->values, array->capacity * sizeof(t_json_value));
}

void json_array_insert(t_json_array *array, e_json_value_type value_type, t_json_value value) {
    if(array->size == array->capacity) {
        json_array_grow_capacity(array);
    }
    array->value_type[array->size] = value_type;
    array->values[array->size] = value;
    array->size++;
}