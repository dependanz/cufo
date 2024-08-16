#ifndef cufo_json_hash_h
#define cufo_json_hash_h

#include "jsonstring.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * SHA256: https://sha256algorithm.com/
 */
void _json_word_list_right_shift(int shiftby, int nbytes, char *word, char *out);
void _json_word_list_left_shift(int shiftby, int nbytes, char *word, char *out);
void json_word_list_shift(int shiftby, int nbytes, char *word, char *out);
void json_word_list_right_rotate(int rotateby, int nbytes, char *word, char *out);
void json_word_list_xor(int nbytes, char *a, char *b, char *out);
void json_word_list_and(int nbytes, char *a, char *b, char *out);
void json_word_list_not(int nbytes, char *a, char *out);
void json_word_list_sum(int nbytes, char *a, char *b, char *out);
void _json_string_sha256(t_json_string* message, char* out);

char* json_sha256(char* message);

#endif