#include "jsonjson.h"

void json_init(t_json *json) {
    json->allocated = 0;
    json->size = 0;
    json->capacity = 0;
    json->properties = (t_json_string*) malloc(0);

    json->values = (t_json_array*) malloc(sizeof(t_json_array));
    json_array_init(json->values);

    json->bst = (t_json_bst*) malloc(sizeof(t_json_bst));
    json_bst_new(json->bst);

    json->err = 0;
}

void json_free(t_json *json) {
    int i;
    for(i = 0; i < json->size; i++) {
        json_string_free(json->properties + i);
    }
    free(json->properties);
    json_array_free(json->values);
    if(json->allocated) {
        free(json);
    }
    json_bst_free(json->bst);
}

void json_grow_capacity(t_json *json) {
    json->capacity = json->capacity == 0 ? 1 : 2 * json->capacity;
    json->properties = (t_json_string*) realloc(json->properties, json->capacity * sizeof(t_json_string));
    // When inserting a value, json_array_insert will invoke json_array_grow_capacity when needed.
    // json_array_grow_capacity(&json->values);
}

void json_property_init(t_json *json) {
    if(json->size == json->capacity) {
        json_grow_capacity(json);
    }
    json_string_init(json->properties + json->size);
    json->values->value_type[json->size] = jsonNULL;
    json->values->values[json->size].ival = -1;
    json->size++;
}

// Insert char for the most recent property
void json_property_insert_char(t_json *json, char c) {
    json_string_insert(json->properties + json->size - 1, c);
}

void json_value_insert(t_json* json, e_json_value_type value_type, t_json_value value) {
    json_array_insert(json->values, value_type, value);
}