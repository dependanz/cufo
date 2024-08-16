#ifndef cufo_array_h
#define cufo_array_h

#include <stdlib.h>

#include "cufo_value.h"

typedef struct _cufo_array {
    int allocated;
    int size;
    int capacity;
    e_cufo_type* value_type;
    t_cufo_value* values;
} t_cufo_array;

void cufo_array_init(t_cufo_array *array);
void cufo_array_free(t_cufo_array *array);
void cufo_array_grow_capacity(t_cufo_array *array);
void cufo_array_insert(t_cufo_array *array, e_cufo_type value_type, t_cufo_value value);

#endif