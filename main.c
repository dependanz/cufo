#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include "cufo.h"

int main(int argc, char* argv) {
    int err = 0; 
    
    t_cufo cufo;
    cufo_init(&cufo);

    t_wave wave;
    cufo.wave.parse(&wave, "./test_files/test2.wav");

    printf("Sample Rate: %d\n", wave.sample_rate);

    // for(int i = 0; i < wave.data.size; i++) {
    //     printf("%f\n", wave.data.samples[0][i]);
    // }

    printf("%d\n", wave.data.size);

    cufo.wave.free(&wave);

    return err;
}