#include "jsonvalue.h"

char* json_value_type_str(e_json_value_type e) {
    switch(e) {
        case jsonNULL:
            return "null";
        case jsonBOOL:
            return "bool";
        case jsonINT:
            return "int";
        case jsonREAL:
            return "real";
        case jsonSTRING:
            return "string";
        case jsonARRAY:
            return "array";
        case jsonJSON:
            return "json";
        default:
            return "<unknown_type>";
    }
}

void _json_print_value_h(e_json_value_type e, t_json_value value, int level) {
    switch(e) {
        case jsonNULL:
            printf("null");
            break;
        case jsonBOOL:
            if(value.ival == 1) {
                printf("true");
            } else {
                printf("false");
            }
            break;
        case jsonINT:
            printf("%d", value.ival);
            break;
        case jsonREAL:
            printf("%f", value.rval);
            break;
        case jsonSTRING:
            printf("\"%s\"", value.sval.str);
            break;
        case jsonARRAY:
            printf("[\n");
            for(int i = 0; i < value.aval->size; i++) {
                for(int j = 0; j < level+1; j++) {
                    printf("    ");
                }
                _json_print_value_h(value.aval->value_type[i], value.aval->values[i], level+1);
                if(i < value.aval->size - 1) {
                    printf(",");
                }
                printf("\n");
            }
            for(int j = 0; j < level; j++) {
                printf("    ");
            }
            printf("]");
            break;
        case jsonJSON:
            printf("{\n");
            for(int i = 0; i < value.jval->size; i++) {
                for(int j = 0; j < level+1; j++) {
                    printf("    ");
                }
                printf("\"%s\" : ", value.jval->properties[i].str);
                _json_print_value_h(value.jval->values->value_type[i], value.jval->values->values[i], level+1);
                printf("\n");
            }
            for(int j = 0; j < level; j++) {
                printf("    ");
            }
            printf("}");
            break;
        default:
            printf("unknown json_value_type\n");
    }
}

void json_print_value(e_json_value_type e, t_json_value value) {
    _json_print_value_h(e, value, 0);
}

void json_print(t_json* json) {
    t_json_value value;
    value.jval = json;
    json_print_value(jsonJSON, value);
    printf("\n");
}