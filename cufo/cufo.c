#include "cufo.h"

void cufo_json_init_ff(t_cufo* cufo) {
    // cufo.json
    cufo->json.parse = cufo_json;
    cufo->json.write = cufo_json_write;
    cufo->json.free = json_free;

    // cufo.json.get
    cufo->json.get = cufo_json_get;
    cufo->json.get_i = cufo_json_get_i;
    cufo->json.get_n = cufo_json_get_n;
    cufo->json.get_b = cufo_json_get_b;
    cufo->json.get_r = cufo_json_get_r;
    cufo->json.get_s = cufo_json_get_s;
    cufo->json.get_a = cufo_json_get_a;
    cufo->json.get_j = cufo_json_get_j;

    // cufo.json.set
    cufo->json.set = cufo_json_set;
    cufo->json.set_i = cufo_json_set_i;
    cufo->json.set_n = cufo_json_set_n;
    cufo->json.set_b = cufo_json_set_b;
    cufo->json.set_r = cufo_json_set_r;
    cufo->json.set_s = cufo_json_set_s;
    cufo->json.set_a = cufo_json_set_a;
    cufo->json.set_j = cufo_json_set_j;

    // cufo.json.set_write
    cufo->json.set_write_i = cufo_json_set_write_i;
    cufo->json.set_write_n = cufo_json_set_write_n;
    cufo->json.set_write_b = cufo_json_set_write_b;
    cufo->json.set_write_r = cufo_json_set_write_r;
    cufo->json.set_write_s = cufo_json_set_write_s;
    cufo->json.set_write_a = cufo_json_set_write_a;
    cufo->json.set_write_j = cufo_json_set_write_j;
}

void cufo_wave_init_ff(t_cufo* cufo) {
    // cufo.wave
    cufo->wave.parse = cufo_wave;
    cufo->wave.free = wave_free;
}

void cufo_init(t_cufo *cufo) {
    cufo_json_init_ff(cufo);
    cufo_wave_init_ff(cufo);
}

void cufo_free(t_cufo *cufo) {
    
}