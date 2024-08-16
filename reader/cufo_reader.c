#include "cufo_reader.h"

char cufo_read_char(t_cufo_reader *reader, FILE* fptr) {
    if(!reader->putback) {
        reader->c = fgetc(fptr);
    } else {
        reader->putback = 0;
    }
    printf("%c %d\n", reader->c, (unsigned int)reader->c);
    return reader->c;
}

void cufo_putback(t_cufo_reader *reader) {
    reader->putback = 1;
}