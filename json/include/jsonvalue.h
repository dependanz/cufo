#ifndef cufo_json_value_h
#define cufo_json_value_h

#include "jsonstring.h"
#include "jsonarray.h"
#include "jsonjson.h"

typedef enum _json_value_type {
    jsonNULL,
    jsonBOOL,
    jsonINT,
    jsonREAL,
    jsonSTRING,
    jsonARRAY,
    jsonJSON
} e_json_value_type;

char* json_value_type_str(e_json_value_type e);

typedef struct _json_array t_json_array;
typedef struct _json t_json;
typedef union _json_value {
    int ival;
    float rval;
    t_json_string sval;
    t_json_array* aval;
    t_json* jval;
} t_json_value;

void json_print_value(e_json_value_type e, t_json_value value);
void json_print(t_json* json);

#endif