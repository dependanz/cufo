#ifndef cufo_json_json_h
#define cufo_json_json_h

typedef enum _json_value_type e_json_value_type;
typedef union _json_value t_json_value;

#include "jsonbst.h"
#include "jsonvalue.h"

typedef struct _json_array t_json_array;
typedef struct _json_bst t_json_bst;
typedef struct _json {
    int allocated;
    int size;
    int capacity;
    
    t_json_string* properties;
    t_json_array* values;
    t_json_bst* bst;

    int err;    // Error flag
} t_json;

void json_init(t_json *json);
void json_free(t_json *json);
void json_grow_capacity(t_json *json);
void json_property_init(t_json *json);

// Insert char for the most recent property
void json_property_insert_char(t_json *json, char c);
void json_value_insert(t_json* json, e_json_value_type value_type, t_json_value value);

#endif