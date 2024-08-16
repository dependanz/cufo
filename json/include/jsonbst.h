#ifndef cufo_json_bst_h
#define cufo_json_bst_h

#include <stdio.h>
#include <stdlib.h>

#include "jsonstring.h"
#include "jsonarray.h"
#include "jsonjson.h"
#include "jsonvalue.h"
#include "jsonhash.h"

typedef struct _json_bst {
    int size;
    int capacity;
    char** keys;
    e_json_value_type* value_type;
    t_json_value* values;
} t_json_bst;

void json_bst_new(t_json_bst* tree);
void _json_bst_grow_capacity(t_json_bst* tree);
void json_bst_insert(t_json_bst* tree, char* key, e_json_value_type value_type, t_json_value value);
e_json_value_type json_bst_get(t_json_bst* tree, char* key, t_json_value* out);
void json_bst_set(t_json_bst* tree, char* key, e_json_value_type value_type, t_json_value value);
void json_bst_free(t_json_bst* tree);

#endif