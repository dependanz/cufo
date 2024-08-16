#include "cufo_json.h"

int return_fclose(t_json * json, FILE* fptr, int retval){
    json_free(json);
    fclose(fptr);
    return retval;
}

int ishex(char c) {
    return isdigit(c) || (tolower(c) == 'a') || (tolower(c) == 'b') || (tolower(c) == 'c') || (tolower(c) == 'd') || (tolower(c) == 'e') || (tolower(c) == 'f');
}

int fromhex(char c) {
    if(!ishex(c)) return -1;
    if(isdigit(c)) return (int)(c - '0');
    return (int)(tolower(c) - 'a');
}

int json_read_unicode(t_cufo_reader *reader, FILE* fptr) {
    int hexval = 0;
    int unicode = 0;
    for(int i = 3; i >= 0; i--) {
        cufo_read_char(reader, fptr);
        hexval = fromhex(reader->c);
        if(hexval == -1) return 0;
        unicode += ((int)pow(16.0, (double)i)) * hexval;
    }
    reader->c = unicode;
    return 1;
}

e_json_value_type parse_value(t_cufo_reader *reader, FILE *fptr, t_json_value* value) {
    if(cufo_read_char(reader, fptr) == EOF) {
        printf("ERROR::cufo::JSON - JSON Object not properly closed. After a key, you need a value.\n");
        return -1;
    }
    if(reader->c == '"') {                               // jsonSTRING
        json_string_init(&value->sval);
        while(cufo_read_char(reader, fptr) != '"'){
            if(reader->c == '\\') {
                cufo_read_char(reader, fptr);
                switch(reader->c) {
                    case 'b':
                        reader->c = '\b';
                        break;
                    case 'f':
                        reader->c = '\f';
                        break;
                    case 'n':
                        reader->c = '\n';
                        break;
                    case 'r':
                        reader->c = '\r';
                        break;
                    case 't':
                        reader->c = '\t';
                        break;
                    case 'u':
                        // parse unicode (todo, this is broken most likely)
                        if(!json_read_unicode(reader, fptr)) {
                            printf("ERROR::cufo::JSON::STRING - Error parsing unicode value. Either eof reached or not valid unicode.\n");
                            return 1;
                        }
                    case '"':
                    case '/':
                    case '\\':
                    default:
                        break;
                }
            }
            json_string_insert(&value->sval, reader->c);
            if(reader->c == EOF) {
                printf("ERROR::cufo::JSON::VALUE::STRING - End of file reached while parsing string value.\n");
                return -1;
            }
        }
        json_string_insert(&value->sval, '\0');
        // printf("value: %s\n", value->sval.str);
        return jsonSTRING;
    } else if(isdigit(reader->c) || reader->c == '-') {   // jsonREAL | jsonINT
        t_json_string valstring;
        json_string_init(&valstring);
        if(reader->c == '-') {
            json_string_insert(&valstring, reader->c);
            cufo_read_char(reader, fptr);
        }
        if(reader->c != '0') {
            while(isdigit(reader->c)) {
                json_string_insert(&valstring, reader->c);
                cufo_read_char(reader, fptr);
            }
            cufo_putback(reader);
        } else {
            json_string_insert(&valstring, reader->c);
        }
        cufo_read_char(reader, fptr);
        if(isdigit(reader->c)) {
            printf("ERROR::cufo::JSON::VALUE::NUMBER - Error parsing a number. (If number starts with 0, cannot have trailing numbers)\n");
            json_string_free(&valstring);
            return -1;
        }
        if(reader->c != '.' && reader->c != 'e') {  // Parse jsonINT
            json_string_insert(&valstring, '\0');
            cufo_putback(reader);
            value->ival = atoi(valstring.str);
            // printf("value: %d\n", value->ival);
            json_string_free(&valstring);
            return jsonINT;
        }

        // Parse jsonREAL
        if(reader->c == '.') {
            json_string_insert(&valstring, reader->c);
            cufo_read_char(reader, fptr);
            if(!isdigit(reader->c)) {
                printf("ERROR::cufo::JSON::VALUE::REAL - Error parsing a number. (A real-value must have digits after the decimal place)\n");
                json_string_free(&valstring);
                return -1;
            }
            while(isdigit(reader->c)) {
                json_string_insert(&valstring, reader->c);
                cufo_read_char(reader, fptr);
            }
        }
        if(tolower(reader->c) != 'e') {
            json_string_insert(&valstring, '\0');
            cufo_putback(reader);
            value->rval = atof(valstring.str);
            // printf("value: %f\n", value->rval);
            json_string_free(&valstring);
            return jsonREAL;
        }

        json_string_insert(&valstring, reader->c);
        cufo_read_char(reader, fptr);
        if(reader->c == '-' || reader->c == '+') {
            json_string_insert(&valstring, reader->c);
        } else if (isdigit(reader->c)) {
            cufo_putback(reader);
        } else {
            printf("ERROR::cufo::JSON::VALUE::REAL - Error parsing a number. (If using scientific notation, after the 'e' should be a '+' a '-' or a number)\n");
            return 1;
        }
        while(isdigit(cufo_read_char(reader, fptr))) {
            json_string_insert(&valstring, reader->c);
        }
        json_string_insert(&valstring, '\0');
        cufo_putback(reader);
        value->rval = atof(valstring.str);
        // printf("value: %f\n", value->rval);
        json_string_free(&valstring);
        return jsonREAL;
    } else if(reader->c == 'f') {                        // jsonBOOL:false
        if(cufo_read_char(reader, fptr) != 'a') {
            printf("ERROR::cufo::JSON::VALUE::BOOL::FALSE - Unrecognized value\n");
            return -1;
        }
        if(cufo_read_char(reader, fptr) != 'l') {
            printf("ERROR::cufo::JSON::VALUE::BOOL::FALSE - Unrecognized value\n");
            return -1;
        }
        if(cufo_read_char(reader, fptr) != 's') {
            printf("ERROR::cufo::JSON::VALUE::BOOL::FALSE - Unrecognized value\n");
            return -1;
        }
        if(cufo_read_char(reader, fptr) != 'e') {
            printf("ERROR::cufo::JSON::VALUE::BOOL::FALSE - Unrecognized value\n");
            return -1;
        }
        if(!isspace(cufo_read_char(reader, fptr)) && reader->c != ',' && reader->c != '}') {
            printf("ERROR::cufo::JSON::VALUE::BOOL::FALSE - Unrecognized value\n");
            return -1;
        }
        cufo_putback(reader);
        value->ival = 0;
        // printf("value: false");
        return jsonBOOL;
    } else if(reader->c == 't') {                        // jsonBOOL:false
        if(cufo_read_char(reader, fptr) != 'r') {
            printf("ERROR::cufo::JSON::VALUE::BOOL::TRUE - Unrecognized value\n");
            return -1;
        }
        if(cufo_read_char(reader, fptr) != 'u') {
            printf("ERROR::cufo::JSON::VALUE::BOOL::TRUE - Unrecognized value\n");
            return -1;
        }
        if(cufo_read_char(reader, fptr) != 'e') {
            printf("ERROR::cufo::JSON::VALUE::BOOL::TRUE - Unrecognized value\n");
            return -1;
        }
        if(!isspace(cufo_read_char(reader, fptr)) && reader->c != ',' && reader->c != '}') {
            printf("ERROR::cufo::JSON::VALUE::BOOL::TRUE - Unrecognized value\n");
            return -1;
        }
        cufo_putback(reader);
        value->ival = 1;
        // printf("value: true");
        return jsonBOOL;
    } else if(reader->c == 'n') {                        // jsonNULL
        if(cufo_read_char(reader, fptr) != 'u') {
            printf("ERROR::cufo::JSON::VALUE::NULL - Unrecognized value\n");
            return -1;
        }
        if(cufo_read_char(reader, fptr) != 'l') {
            printf("ERROR::cufo::JSON::VALUE::NULL - Unrecognized value\n");
            return -1;
        }
        if(cufo_read_char(reader, fptr) != 'l') {
            printf("ERROR::cufo::JSON::VALUE::NULL - Unrecognized value\n");
            return -1;
        }
        if(!isspace(cufo_read_char(reader, fptr)) && reader->c != ',' && reader->c != '}') {
            printf("ERROR::cufo::JSON::VALUE::NULL - Unrecognized value\n");
            return -1;
        }
        cufo_putback(reader);
        value->ival = -1;
        // printf("value: null");
        return jsonNULL;
    } else if(reader->c == '[') {                        // jsonARRAY
        value->aval = (t_json_array*) malloc(sizeof(t_json_array));
        json_array_init(value->aval);
        value->aval->allocated = 1;

        while(reader->c != ']') {
            // Skip whitespace
            while(isspace(cufo_read_char(reader, fptr))) {
            }
            cufo_putback(reader);

            t_json_value arrval;
            e_json_value_type value_type;
            value_type = parse_value(reader, fptr, &arrval);
            if(value_type == -1) {
                printf("ERROR::cufo::JSON::VALUE - Could not parse a value.\n");
                return -1;
            }
            json_array_insert(value->aval, value_type, arrval);

            // Skip whitespace
            while(isspace(cufo_read_char(reader, fptr))) {
            }
            cufo_putback(reader);

            // Expect a comma if not a ']'
            if(cufo_read_char(reader, fptr) == ',') {
                continue;
            } else if(reader->c != ']') {
                if(value_type == -1) {
                    printf("ERROR::cufo::JSON::VALUE::ARRAY - json array not properly closed.\n");
                    return -1;
                }
            }
        }
        for(int j = 0; j < value->aval->size; j++) {
            // printf("array val: %s\n", value->aval->values[j].sval.str);
        }
        // printf("An array!\n");
        return jsonARRAY;
    } else if(reader->c == '{') {                        // jsonJSON
        value->jval = (t_json*) malloc(sizeof(t_json));
        json_init(value->jval);
        value->jval->allocated = 1;

        // Parse json
        value->jval->err = parse_json(reader, fptr, value->jval);
        if(value->jval->err) {
            printf("ERROR::cufo::JSON::VALUE::JSON - Error parsing json.\n");
            return -1;
        }

        return jsonJSON;
    } else {
        printf("ERROR::cufo::JSON::VALUE - Invalid json value format\n");
        return -1;
    }
}

int parse_json(t_cufo_reader *reader, FILE *fptr, t_json *json) {
    // Skip whitespace
    while(isspace(cufo_read_char(reader, fptr))) {
    }
    cufo_putback(reader);
    
    if(cufo_read_char(reader, fptr) != '{' && !json->allocated) {
        printf("ERROR::cufo::JSON - First char must be a '{'. (check for any whitespaces at the beginning)\n");
        return 1;
    }
    if(json->allocated) {
        cufo_putback(reader);
    }

    while(1) {
        // Skip whitespace
        // printf("test2\n");
        while(isspace(cufo_read_char(reader, fptr))) {
        }
        cufo_putback(reader);

        // Parse property string
        // Expect ' " ' as the start of a property string
        if(cufo_read_char(reader, fptr) == EOF) {
            printf("ERROR::cufo::JSON - JSON Object not properly closed.\n");
            return 1;
        }
        if(reader->c != '"') {
            printf("ERROR::cufo::JSON::PROPERTY - A property value must be a string enclosed with quotation marks.\n");
            return 1;
        }
        json_property_init(json);
        while(cufo_read_char(reader, fptr) != '"'){
            if(reader->c == '\\') {
                cufo_read_char(reader, fptr);
                switch(reader->c) {
                    case 'b':
                        reader->c = '\b';
                        break;
                    case 'f':
                        reader->c = '\f';
                        break;
                    case 'n':
                        reader->c = '\n';
                        break;
                    case 'r':
                        reader->c = '\r';
                        break;
                    case 't':
                        reader->c = '\t';
                        break;
                    case 'u':
                        // parse unicode (todo, this is broken most likely)
                        if(!json_read_unicode(reader, fptr)) {
                            printf("ERROR::cufo::JSON::STRING - Error parsing unicode value. Either eof reached or not valid unicode.\n");
                            return 1;
                        }
                    case '"':
                    case '/':
                    case '\\':
                    default:
                        break;
                }
            }
            json_property_insert_char(json, reader->c);
            if(reader->c == EOF) {
                printf("ERROR::cufo::JSON::PROPERTY - End of file reached while parsing property.\n");
                return 1;
            }
        }
        json_property_insert_char(json, '\0');

        // printf("property: '%s'\n", json->properties[json->size-1].str);

        // Skip whitespace
        while(isspace(cufo_read_char(reader, fptr))) {
        }
        cufo_putback(reader);

        // After property, expect a ":"
        if(cufo_read_char(reader, fptr) == EOF) {
            printf("ERROR::cufo::JSON - After a property key, you need a ':' to separate from a value.\n");
            return 1;
        }
        if(reader->c != ':') {
            printf("ERROR::cufo::JSON - After a property key, you need a ':' to separate from a value.\n");
            return 1;
        }

        // Skip whitespace
        while(isspace(cufo_read_char(reader, fptr))) {
        }
        cufo_putback(reader);

        // Parse Value
        t_json_value value;
        e_json_value_type value_type;
        value_type = parse_value(reader, fptr, &value);
        if(value_type == -1) {
            printf("ERROR::cufo::JSON::VALUE - Could not parse a value.\n");
            return 1;
        }
        json_value_insert(json, value_type, value);

        json_bst_insert(json->bst, json->properties[json->size-1].str, json->values->value_type[json->values->size-1], json->values->values[json->values->size-1]);

        // Skip whitespace
        while(isspace(cufo_read_char(reader, fptr))) {
        }
        cufo_putback(reader);

        // If comma, start the loop over again.
        if(cufo_read_char(reader, fptr) != ',') {
            cufo_putback(reader);
            break;
        }
    }

    if(cufo_read_char(reader, fptr) != '}') {
        printf("ERROR::cufo::JSON - JSON Object not properly closed.\n");
        return 1;
    }
    return 0;
}

int cufo_json(t_json* json, char* filename) {
    int err;
    FILE* fptr;

    t_cufo_reader reader;
    reader.putback = 0;
    
    fptr = fopen(filename, "r");
    if(!fptr) {
        printf("ERROR::cufo::JSON - Error opening file\n");
        return return_fclose(json, fptr, 1);
    }

    json_init(json);
    err = parse_json(&reader, fptr, json);

    fclose(fptr);

    return err;
}

static void _cufo_json_write_h(FILE* fptr, e_json_value_type e, t_json_value value, int level) {
    switch(e) {
        case jsonNULL:
            fprintf(fptr, "null");
            break;
        case jsonBOOL:
            if(value.ival == 1) {
                fprintf(fptr, "true");
            } else {
                fprintf(fptr, "false");
            }
            break;
        case jsonINT:
            fprintf(fptr, "%d", value.ival);
            break;
        case jsonREAL:
            fprintf(fptr, "%f", value.rval);
            break;
        case jsonSTRING:
            fputc('"', fptr);
            for(int i = 0; i < value.sval.size; i++) {
                if(value.sval.str[i] == '\0') continue;
                if(value.sval.str[i] == '\n') {
                    fprintf(fptr, "\\n");
                    continue;
                }
                if(value.sval.str[i] == '\r') {
                    fprintf(fptr, "\\r");
                    continue;
                }
                if(value.sval.str[i] == '\t') {
                    fprintf(fptr, "\\t");
                    continue;
                }
                if(value.sval.str[i] == '"') {
                    fprintf(fptr, "\\\"");
                    continue;
                }
                if(value.sval.str[i] == '\\') {
                    fprintf(fptr, "\\\\");
                    continue;
                }
                if(value.sval.str[i] == '\b') {
                    fprintf(fptr, "\\b");
                    continue;
                }
                if(value.sval.str[i] == '\f') {
                    fprintf(fptr, "\\f");
                    continue;
                }
                // [TODO] Unicode stuff
                fputc(value.sval.str[i], fptr);
            }
            fputc('"', fptr);
            break;
        case jsonARRAY:
            fprintf(fptr, "[\n");
            for(int i = 0; i < value.aval->size; i++) {
                for(int j = 0; j < level+1; j++) {
                    fprintf(fptr, "    ");
                }
                _cufo_json_write_h(fptr, value.aval->value_type[i], value.aval->values[i], level+1);
                if(i < value.aval->size - 1) {
                    fprintf(fptr, ",");
                }
                fprintf(fptr, "\n");
            }
            for(int j = 0; j < level; j++) {
                fprintf(fptr, "    ");
            }
            fprintf(fptr, "]");
            break;
        case jsonJSON:
            fprintf(fptr, "{\n");
            for(int i = 0; i < value.jval->size; i++) {
                for(int j = 0; j < level+1; j++) {
                    fprintf(fptr, "    ");
                }
                fprintf(fptr, "\"%s\" : ", value.jval->properties[i].str);
                _cufo_json_write_h(fptr, value.jval->values->value_type[i], value.jval->values->values[i], level+1);
                if(i < value.jval->size - 1) {
                    fprintf(fptr, ",");
                }
                fprintf(fptr, "\n");
            }
            for(int j = 0; j < level; j++) {
                fprintf(fptr, "    ");
            }
            fprintf(fptr, "}");
            break;
        default:
            fprintf(fptr, "unknown json_value_type\n");
    }
}

void cufo_json_write(t_json* json, char* filename) {
    int err;
    FILE* fptr;

    fptr = fopen(filename, "w+");
    if(!fptr) {
        printf("ERROR::cufo::JSON::WRITER - Error opening file\n");
        return;
    }

    t_json_value value;
    value.jval = json;
    _cufo_json_write_h(fptr, jsonJSON, value, 0);

    fclose(fptr);
}

e_json_value_type cufo_json_get(t_json* json, char* property, t_json_value* value) {
    return json_bst_get(json->bst, property, value);
}

int cufo_json_get_i(t_json* json, char* property) {
    t_json_value value;
    e_json_value_type value_type = cufo_json_get(json, property, &value);
    if(value_type != jsonINT) {
        printf("ERROR::cufo::JSON::GET::INT - Value is not an integer.");
        return 0;
    }
    return value.ival;
}

int cufo_json_get_n(t_json* json, char* property) {
    t_json_value value;
    e_json_value_type value_type = cufo_json_get(json, property, &value);
    if(value_type != jsonNULL) {
        printf("ERROR::cufo::JSON::GET::NULL - Value is not a NULL.");
        return 0;
    }
    return value.ival;
}

int cufo_json_get_b(t_json* json, char* property) {
    t_json_value value;
    e_json_value_type value_type = cufo_json_get(json, property, &value);
    if(value_type != jsonBOOL) {
        printf("ERROR::cufo::JSON::GET::BOOL - Value is not a boolean.");
        return 0;
    }
    return value.ival;
}

double cufo_json_get_r(t_json* json, char* property) {
    t_json_value value;
    e_json_value_type value_type = cufo_json_get(json, property, &value);
    if(value_type != jsonREAL) {
        printf("ERROR::cufo::JSON::GET::REAL - Value is not a real number.");
        return 0;
    }
    return value.rval;
}

t_json_string cufo_json_get_s(t_json* json, char* property) {
    t_json_value value;
    e_json_value_type value_type = cufo_json_get(json, property, &value);
    if(value_type != jsonSTRING) {
        printf("ERROR::cufo::JSON::GET::STRING - Value is not a string.");
        return value.sval;
    }
    return value.sval;
}

t_json_array* cufo_json_get_a(t_json* json, char* property) {
    t_json_value value;
    e_json_value_type value_type = cufo_json_get(json, property, &value);
    if(value_type != jsonARRAY) {
        printf("ERROR::cufo::JSON::GET::ARRAY - Value is not an array.");
        return 0;
    }
    return value.aval;
}

t_json* cufo_json_get_j(t_json* json, char* property) {
    t_json_value value;
    e_json_value_type value_type = cufo_json_get(json, property, &value);
    if(value_type != jsonJSON) {
        printf("ERROR::cufo::JSON::GET::ARRAY - Value is not a json object.");
        return 0;
    }
    return value.jval;
}

void cufo_json_set(t_json* json, char* property, e_json_value_type value_type, t_json_value value) {
    json_bst_set(json->bst, property, value_type, value);
    
    // [TODO] have the BST point to the json values and not just have copies of them.
    t_json_value value_before;
    e_json_value_type value_type_before;
    value_type_before = cufo_json_get(json, property, &value_before);
    if(value_type_before == -1) {
        json_property_init(json);
        for(int i = 0; i < strlen(property); i++) {
            json_property_insert_char(json, property[i]);
        }
        json_value_insert(json, value_type, value);
        return;
    }

    for(int i = 0; i < json->size; i++) {
        if(strcmp(json->properties[i].str, property) == 0) {
            json->values->value_type[i] = value_type;
            json->values->values[i] = value;
            break;
        }
    }
}

void cufo_json_set_i(t_json* json, char* property, int i) {
    t_json_value value;
    value.ival = i;
    cufo_json_set(json, property, jsonINT, value);
}

void cufo_json_set_n(t_json* json, char* property) {
    t_json_value value;
    value.ival = -1;
    cufo_json_set(json, property, jsonNULL, value);
}

void cufo_json_set_b(t_json* json, char* property, int b) {
    t_json_value value;
    value.ival = b;
    cufo_json_set(json, property, jsonBOOL, value);
}

void cufo_json_set_r(t_json* json, char* property, double r) {
    t_json_value value;
    value.rval = r;
    cufo_json_set(json, property, jsonREAL, value);
}

void cufo_json_set_s(t_json* json, char* property, t_json_string s) {
    t_json_value value;
    value.sval = s;
    cufo_json_set(json, property, jsonSTRING, value);
}

void cufo_json_set_a(t_json* json, char* property, t_json_array* a) {
    t_json_value value;
    value.aval = a;
    cufo_json_set(json, property, jsonARRAY, value);
}

void cufo_json_set_j(t_json* json, char* property, t_json* j) {
    t_json_value value;
    value.jval = j;
    cufo_json_set(json, property, jsonJSON, value);
}

void cufo_json_set_write_i(t_json* json, char* property, int i, char* filename) {
    cufo_json_set_i(json, property, i);
    cufo_json_write(json, filename);
}

void cufo_json_set_write_n(t_json* json, char* property, char* filename) {
    cufo_json_set_n(json,property);
    cufo_json_write(json, filename);
}

void cufo_json_set_write_b(t_json* json, char* property, int b, char* filename) {
    cufo_json_set_b(json, property, b);
    cufo_json_write(json, filename);
}

void cufo_json_set_write_r(t_json* json, char* property, double r, char* filename) {
    cufo_json_set_r(json, property, r);
    cufo_json_write(json, filename);
}

void cufo_json_set_write_s(t_json* json, char* property, t_json_string s, char* filename) {
    cufo_json_set_s(json, property, s);
    cufo_json_write(json, filename);
}

void cufo_json_set_write_a(t_json* json, char* property, t_json_array* a, char* filename) {
    cufo_json_set_a(json, property, a);
    cufo_json_write(json, filename);
}

void cufo_json_set_write_j(t_json* json, char* property, t_json* j, char* filename) {
    cufo_json_set_j(json, property, j);
    cufo_json_write(json, filename);
}