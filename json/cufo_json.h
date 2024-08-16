#ifndef cufo_json_h
#define cufo_json_h

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "cufo_reader.h"

#include "jsonstring.h"
#include "jsonarray.h"
#include "jsonjson.h"
#include "jsonbst.h"

int return_fclose(t_json * json, FILE* fptr, int retval);

int ishex(char c);
int fromhex(char c);

int json_read_unicode(t_cufo_reader *reader, FILE* fptr);

e_json_value_type parse_value(t_cufo_reader *reader, FILE *fptr, t_json_value* value);
int parse_json(t_cufo_reader *reader, FILE *fptr, t_json *json);

int cufo_json(t_json* json, char* filename);
void cufo_json_write(t_json* json, char* filename);

e_json_value_type cufo_json_get(t_json* json, char* property, t_json_value* value);
int cufo_json_get_i(t_json* json, char* property);            // cufo.json.get.i(nteger)
int cufo_json_get_n(t_json* json, char* property);            // cufo.json.get.n(ull)
int cufo_json_get_b(t_json* json, char* property);            // cufo.json.get.b(oolean)
double cufo_json_get_r(t_json* json, char* property);         // cufo.json.get.r(eal)
t_json_string cufo_json_get_s(t_json* json, char* property);  // cufo.json.get.s(tring)
t_json_array* cufo_json_get_a(t_json* json, char* property);  // cufo.json.get.a(rray)
t_json* cufo_json_get_j(t_json* json, char* property);        // cufo.json.get.j(son)

void cufo_json_set(t_json* json, char* property, e_json_value_type value_type, t_json_value value);
void cufo_json_set_i(t_json* json, char* property, int i);
void cufo_json_set_n(t_json* json, char* property);
void cufo_json_set_b(t_json* json, char* property, int b);
void cufo_json_set_r(t_json* json, char* property, double r);
void cufo_json_set_s(t_json* json, char* property, t_json_string s);
void cufo_json_set_a(t_json* json, char* property, t_json_array* a);
void cufo_json_set_j(t_json* json, char* property, t_json* j);

void cufo_json_set_write_i(t_json* json, char* property, int i, char* filename);              // cufo.json.set_write.i(nteger)
void cufo_json_set_write_b(t_json* json, char* property, int b, char* filename);              // cufo.json.set_write.b(oolean)
void cufo_json_set_write_n(t_json* json, char* property, char* filename);                     // cufo.json.set_write.n(ull)
void cufo_json_set_write_r(t_json* json, char* property, double r, char* filename);           // cufo.json.set_write.r(eal)
void cufo_json_set_write_s(t_json* json, char* property, t_json_string s, char* filename);    // cufo.json.set_write.s(tring)
void cufo_json_set_write_a(t_json* json, char* property, t_json_array* a, char* filename);    // cufo.json.set_write.a(rray)
void cufo_json_set_write_j(t_json* json, char* property, t_json* j, char* filename);          // cufo.json.set_write.j(son)

#endif