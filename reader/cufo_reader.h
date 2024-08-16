#ifndef cufo_reader_h
#define cufo_reader_h

#include <stdio.h>

typedef struct _cufo_reader {
    char c;
    int putback;
} t_cufo_reader;

char cufo_read_char(t_cufo_reader *reader, FILE* fptr);
void cufo_putback(t_cufo_reader *reader);

#endif