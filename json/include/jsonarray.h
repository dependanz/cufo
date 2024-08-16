#ifndef cufo_json_array_h
#define cufo_json_array_h

#include <stdlib.h>
#include "jsonvalue.h"

typedef enum _json_value_type e_json_value_type;
typedef union _json_value t_json_value;
typedef struct _json_array {
    int allocated;
    int size;
    int capacity;
    e_json_value_type *value_type;
    t_json_value* values;
} t_json_array;

void json_array_init(t_json_array *array);
void json_array_free(t_json_array *array);
void json_array_grow_capacity(t_json_array *array);
void json_array_insert(t_json_array *array, e_json_value_type value_type, t_json_value value);

#endif