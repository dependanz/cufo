#ifndef cufo_h
#define cufo_h

#include "cufo_value.h"
#include "cufo_array.h"
#include "cufo_reader.h"
#include "cufo_json.h"
#include "cufo_wave.h"

typedef struct _cufo_json_ff {
    int (*parse)(t_json* json, char* filename);
    void (*write)(t_json* json, char* filename);
    void (*free)(t_json* json);

    e_json_value_type (*get)(t_json* json, char* property, t_json_value* out);
    int (*get_i)(t_json* json, char* property);             // cufo.json.get_i(nteger)
    int (*get_n)(t_json* json, char* property);             // cufo.json.get_n(ull)
    int (*get_b)(t_json* json, char* property);             // cufo.json.get_b(oolean)
    double (*get_r)(t_json* json, char* property);          // cufo.json.get_r(eal)
    t_json_string (*get_s)(t_json* json, char* property);   // cufo.json.get_s(tring)
    t_json_array* (*get_a)(t_json* json, char* property);   // cufo.json.get_a(rray)
    t_json* (*get_j)(t_json* json, char* property);         // cufo.json.get_j(son)

    void (*set)(t_json* json, char* property, e_json_value_type value_type, t_json_value value);
    void (*set_i)(t_json* json, char* property, int i);
    void (*set_n)(t_json* json, char* property);
    void (*set_b)(t_json* json, char* property, int b);
    void (*set_r)(t_json* json, char* property, double r);
    void (*set_s)(t_json* json, char* property, t_json_string s);
    void (*set_a)(t_json* json, char* property, t_json_array* a);
    void (*set_j)(t_json* json, char* property, t_json* j);

    void (*set_write_i)(t_json* json, char* property, int i, char* filename);              // cufo.json.set_write.i(nteger)
    void (*set_write_b)(t_json* json, char* property, int b, char* filename);              // cufo.json.set_write.b(oolean)
    void (*set_write_n)(t_json* json, char* property, char* filename);                     // cufo.json.set_write.n(ull)
    void (*set_write_r)(t_json* json, char* property, double r, char* filename);           // cufo.json.set_write.r(eal)
    void (*set_write_s)(t_json* json, char* property, t_json_string s, char* filename);    // cufo.json.set_write.s(tring)
    void (*set_write_a)(t_json* json, char* property, t_json_array* a, char* filename);    // cufo.json.set_write.a(rray)
    void (*set_write_j)(t_json* json, char* property, t_json* j, char* filename);          // cufo.json.set_write.j(son)
} t_cufo_json_ff;

typedef struct _cufo_wave_ff {
    int (*parse)(t_wave *wave, char *filename);
    void (*free)(t_wave* wave);
} t_cufo_wave_ff;

typedef struct _cufo {
    t_cufo_json_ff json;
    
    // Audio
    t_cufo_wave_ff wave;
} t_cufo;

void cufo_init(t_cufo *cufo);
void cufo_free(t_cufo *cufo); // Might be unnecessary

#endif