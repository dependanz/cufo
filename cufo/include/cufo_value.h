#ifndef cufo_value_h
#define cufo_value_h

#define GUID_LENGTH 16
typedef char t_guid[GUID_LENGTH];

typedef enum _cufo_type {
    cufoBOOL,
    cufoINT,
    cufoREAL,
    cufoSTRING,
} e_cufo_type;

typedef struct _cufo_array t_cufo_array;
typedef union _cufo_value {
    int ival;
    double rval;

    // t_cufo_string* sval;
    t_cufo_array* aval;
    // t_json* json;
} t_cufo_value;

#endif