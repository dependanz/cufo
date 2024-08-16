#ifndef cufo_wave_h
#define cufo_wave_h

#include <stdlib.h>

#include "cufo_reader.h"
#include "cufo_array.h"
#include "cufo_value.h"

typedef enum _wave_chunk_type
{
    WAVE_UNKNOWN_FORMAT_CHUNK,
    WAVE_FILE_FORMAT_CHUNK,
    WAVE_DATA_FORMAT_CHUNK,
    WAVE_FACT_CHUNK,
    WAVE_LIST_CHUNK,
    WAVE_DATA_CHUNK
} e_wave_chunk_type;

typedef enum _wave_format_code
{
    WAVE_FORMAT_UNKNOWN,
    WAVE_FORMAT_PCM = 0x0001,
    WAVE_FORMAT_IEEE_FLOAT = 0x0003,
    WAVE_FORMAT_ALAW = 0x0006,
    WAVE_FORMAT_MULAW = 0x0007,
    WAVE_FORMAT_EXTENSIBLE = 0xFFFE // Determined by SubFormat guid
} e_wave_format_code;

typedef struct _wave_extension
{
    int extension_size;
    int num_valid_bits;
    int speaker_position_mask;
    t_guid subformat_guid;
} t_wave_extension;

typedef struct _wave_fact
{
    int num_samples_per_channel; // dwSampleLength
} t_wave_fact;

typedef struct _wave_data
{
    double **samples;
    int size;
    int padded;
    int is_loaded;
} t_wave_data;

typedef struct _wave
{
    // Wave File Format
    int file_size;

    // Data
    t_wave_data data;
    int data_block_size;
    t_cufo_array _blocked_samples;

    // Data Format
    e_wave_format_code format_code;
    int num_channels;
    int sample_rate;
    int data_rate;
    int bits_per_sample;

    // Data Format Extension
    t_wave_extension ext;

    // 

    // fact chunk
    int has_fact;
    t_wave_fact fact;
} t_wave;

void wave_free(t_wave *wave);

void wave_set_format_code(t_wave *wave, int format_code);
e_wave_chunk_type wave_read_chunk_id(t_cufo_reader *reader, FILE *fptr);
int wave_parse_file_format_chunk(t_cufo_reader *reader, FILE *fptr, t_wave *wave);
int wave_parse_data_format_chunk(t_cufo_reader *reader, FILE *fptr, t_wave *wave);
int wave_parse_fact_chunk(t_cufo_reader *reader, FILE *fptr, t_wave *wave);

/**
 *  Util: get the nth bit (left is more-significant)
 */
char get_nth_bit(int b, int n);

/**
 *  Util: print binary string (no newline)
 */
void print_bin(int b, int startbit, int endbit);

int _wave_process_block_ieee_float(t_wave *wave, char *block, double *sample);
int wave_parse_data_chunk(t_cufo_reader *reader, FILE *fptr, t_wave *wave);

int wave_parse_list_chunk(t_cufo_reader *reader, FILE *fptr, t_wave *wave);

int wave_parse_chunk(t_cufo_reader *reader, FILE *fptr, t_wave *wave);

int cufo_wave(t_wave *wave, char *filename);

#endif