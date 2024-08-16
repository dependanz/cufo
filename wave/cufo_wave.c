#include "cufo_wave.h"

typedef enum _riff_list_type {
    RIFF_LIST_UNK,      // Unknown type
    RIFF_LIST_IARL,		// The location where the subject of the file is archived
    RIFF_LIST_IART,		// The artist of the original subject of the file
    RIFF_LIST_ICMS,		// The name of the person or organization that commissioned the original subject of the file
    RIFF_LIST_ICMT,		// General comments about the file or its subject
    RIFF_LIST_ICOP,		// Copyright information about the file (e.g., "Copyright Some Company 2011")
    RIFF_LIST_ICRD,		// The date the subject of the file was created (creation date) (e.g., "2022-12-31")
    RIFF_LIST_ICRP,		// Whether and how an image was cropped
    RIFF_LIST_IDIM,		// The dimensions of the original subject of the file
    RIFF_LIST_IDPI,		// Dots per inch settings used to digitize the file
    RIFF_LIST_IENG,		// The name of the engineer who worked on the file
    RIFF_LIST_IGNR,		// The genre of the subject
    RIFF_LIST_IKEY,		// A list of keywords for the file or its subject
    RIFF_LIST_ILGT,		// Lightness settings used to digitize the file
    RIFF_LIST_IMED,		// Medium for the original subject of the file
    RIFF_LIST_INAM,		// Title of the subject of the file (name)
    RIFF_LIST_INFO,		// 
    RIFF_LIST_IPLT,		// The number of colors in the color palette used to digitize the file
    RIFF_LIST_IPRD,		// Name of the title the subject was originally intended for
    RIFF_LIST_ISBJ,		// Description of the contents of the file (subject)
    RIFF_LIST_ISFT,		// Name of the software package used to create the file
    RIFF_LIST_ISRC,		// The name of the person or organization that supplied the original subject of the file
    RIFF_LIST_ISRF,		// The original form of the material that was digitized (source form)
    RIFF_LIST_ITCH		// The name of the technician who digitized the subject file
} e_riff_list_type;

// [TODO] This is disgusting code....
e_riff_list_type get_riff_list_type_from_chars(char* s) {
    if(s[0] == 'I' && s[1] == 'A' && s[2] == 'R' && s[3] == 'L') return RIFF_LIST_IARL;
    if(s[0] == 'I' && s[1] == 'A' && s[2] == 'R' && s[3] == 'T') return RIFF_LIST_IART;
    if(s[0] == 'I' && s[1] == 'C' && s[2] == 'M' && s[3] == 'S') return RIFF_LIST_ICMS;
    if(s[0] == 'I' && s[1] == 'C' && s[2] == 'M' && s[3] == 'T') return RIFF_LIST_ICMT;
    if(s[0] == 'I' && s[1] == 'C' && s[2] == 'O' && s[3] == 'P') return RIFF_LIST_ICOP;
    if(s[0] == 'I' && s[1] == 'C' && s[2] == 'R' && s[3] == 'D') return RIFF_LIST_ICRD;
    if(s[0] == 'I' && s[1] == 'C' && s[2] == 'R' && s[3] == 'P') return RIFF_LIST_ICRP;
    if(s[0] == 'I' && s[1] == 'D' && s[2] == 'I' && s[3] == 'M') return RIFF_LIST_IDIM;
    if(s[0] == 'I' && s[1] == 'D' && s[2] == 'P' && s[3] == 'I') return RIFF_LIST_IDPI;
    if(s[0] == 'I' && s[1] == 'E' && s[2] == 'N' && s[3] == 'G') return RIFF_LIST_IENG;
    if(s[0] == 'I' && s[1] == 'G' && s[2] == 'N' && s[3] == 'R') return RIFF_LIST_IGNR;
    if(s[0] == 'I' && s[1] == 'K' && s[2] == 'E' && s[3] == 'Y') return RIFF_LIST_IKEY;
    if(s[0] == 'I' && s[1] == 'L' && s[2] == 'G' && s[3] == 'T') return RIFF_LIST_ILGT;
    if(s[0] == 'I' && s[1] == 'M' && s[2] == 'E' && s[3] == 'D') return RIFF_LIST_IMED;
    if(s[0] == 'I' && s[1] == 'N' && s[2] == 'A' && s[3] == 'M') return RIFF_LIST_INAM;
    if(s[0] == 'I' && s[1] == 'N' && s[2] == 'F' && s[3] == 'O') return RIFF_LIST_INFO;
    if(s[0] == 'I' && s[1] == 'P' && s[2] == 'L' && s[3] == 'T') return RIFF_LIST_IPLT;
    if(s[0] == 'I' && s[1] == 'P' && s[2] == 'R' && s[3] == 'D') return RIFF_LIST_IPRD;
    if(s[0] == 'I' && s[1] == 'S' && s[2] == 'B' && s[3] == 'J') return RIFF_LIST_ISBJ;
    if(s[0] == 'I' && s[1] == 'S' && s[2] == 'F' && s[3] == 'T') return RIFF_LIST_ISFT;
    if(s[0] == 'I' && s[1] == 'S' && s[2] == 'R' && s[3] == 'C') return RIFF_LIST_ISRC;
    if(s[0] == 'I' && s[1] == 'S' && s[2] == 'R' && s[3] == 'F') return RIFF_LIST_ISRF;
    if(s[0] == 'I' && s[1] == 'T' && s[2] == 'C' && s[3] == 'H') return RIFF_LIST_ITCH;
    return RIFF_LIST_UNK;
}
typedef struct _riff_list {
    char chunk_id[4];
    char size[4];
    e_riff_list_type type_id;
    // data
} t_riff_list;

void wave_free(t_wave* wave) {
    // [TODO] is_loaded var
    if(wave->data.is_loaded) {
        for(int i = 0; i < wave->num_channels; i++) {
            free(wave->data.samples[i]);
        }
        free(wave->data.samples);
    }
    wave->data.is_loaded = 0;
}

void wave_set_format_code(t_wave* wave, int format_code) {
    switch(format_code) {
        case WAVE_FORMAT_PCM:
            wave->format_code = WAVE_FORMAT_PCM;
            break;
        case WAVE_FORMAT_IEEE_FLOAT:
            wave->format_code = WAVE_FORMAT_IEEE_FLOAT;
            break;
        case WAVE_FORMAT_ALAW:
            wave->format_code = WAVE_FORMAT_ALAW;
            break;
        case WAVE_FORMAT_MULAW:
            wave->format_code = WAVE_FORMAT_MULAW;
            break;
        case WAVE_FORMAT_EXTENSIBLE:
            wave->format_code = WAVE_FORMAT_EXTENSIBLE;
            break;
        default:
            wave->format_code = WAVE_FORMAT_UNKNOWN;
    }
}

e_wave_chunk_type wave_read_chunk_id(t_cufo_reader* reader, FILE* fptr) {
    int i;
    char chunk_id[4];
    char riff_id[4] = "RIFF";
    char list_id[4] = "LIST";
    char fact_id[4] = "fact";
    char format_block_id[4] = "fmt ";
    char data_id[4] = "data";

    // Read Chunk ID
    for(i = 0; i < 4; i++) {
        chunk_id[i] = cufo_read_char(reader, fptr);
    }
    for(i = 0; i < 4; i++) {
        if(chunk_id[i] == riff_id[i]){
            if(i == 3) return WAVE_FILE_FORMAT_CHUNK;
            continue;
        }
        break;
    }
    for(i = 0; i < 4; i++) {
        if(chunk_id[i] == format_block_id[i]){
            if(i == 3) return WAVE_DATA_FORMAT_CHUNK;
            continue;
        }
        break;
    }
    for(i = 0; i < 4; i++) {
        if(chunk_id[i] == fact_id[i]){
            if(i == 3) return WAVE_FACT_CHUNK;
            continue;
        }
        break;
    }
    for(i = 0; i < 4; i++) {
        if(chunk_id[i] == data_id[i]){
            if(i == 3) return WAVE_DATA_CHUNK;
            continue;
        }
        break;
    }
    for(i = 0; i < 4; i++) {
        if(chunk_id[i] == list_id[i]){
            if(i == 3) return WAVE_LIST_CHUNK;
            continue;
        }
        break;
    }
    return WAVE_UNKNOWN_FORMAT_CHUNK;
}

int wave_parse_file_format_chunk(t_cufo_reader* reader, FILE* fptr, t_wave* wave) {
    int i;
    wave->file_size = 0;
    for(i = 0; i < 4; i++) {
        cufo_read_char(reader, fptr);
        wave->file_size += ((int) reader->c) << (8 * i);
    }
    wave->file_size += 8;

    char wave_id[4] = "WAVE";
    for(i = 0; i < 4; i++) {
        cufo_read_char(reader, fptr);
        if(reader->c != wave_id[i]){
            printf("ERROR::cufo::WAV::FILE_FORMAT_ID - Expected Wave ID for file format id\n");
            return 1;
        }
    }

    return 0;
}

int wave_parse_data_format_chunk(t_cufo_reader* reader, FILE* fptr, t_wave* wave) {
    int i;
    int format_chunk_size = 0;
    int format_code = 0;
    wave->num_channels = 0;
    wave->sample_rate = 0;
    wave->data_rate = 0;
    wave->data_block_size = 0;
    wave->bits_per_sample = 0;

    wave->ext.extension_size = 0;
    wave->ext.speaker_position_mask = 0;
    for(i = 0; i < 16; i++) {
        wave->ext.subformat_guid[i] = 0;
    }

    // Read chunk size
    for(i = 0; i < 4; i++) {
        cufo_read_char(reader, fptr);
        format_chunk_size += ((int) reader->c) << (8 * i);
    }
    wave->file_size += 8;

    // Format Code
    for(i = 0; i < 2; i++) {
        cufo_read_char(reader, fptr);
        format_code += ((int) reader->c) << (8 * i);
    }
    wave_set_format_code(wave, format_code);
    format_chunk_size -= 2;
    
    // Number of Interleaved Channels
    for(i = 0; i < 2; i++) {
        cufo_read_char(reader, fptr);
        wave->num_channels += ((int) reader->c) << (8 * i);
    }
    format_chunk_size -= 2;

    // Sample Rate
    for(i = 0; i < 4; i++) {
        cufo_read_char(reader, fptr);
        wave->sample_rate += ((int) reader->c) << (8 * i);
    }
    format_chunk_size -= 4;

    // Data Rate (Number of average bytes per second)
    for(i = 0; i < 4; i++) {
        cufo_read_char(reader, fptr);
        wave->data_rate += ((int) reader->c) << (8 * i);
    }
    format_chunk_size -= 4;

    // Data Block Size in bytes (block align)
    for(i = 0; i < 2; i++) {
        cufo_read_char(reader, fptr);
        wave->data_block_size += ((int) reader->c) << (8 * i);
    }
    format_chunk_size -= 2;

    // Bits per sample
    for(i = 0; i < 2; i++) {
        cufo_read_char(reader, fptr);
        wave->bits_per_sample += ((int) reader->c) << (8 * i);
    }
    format_chunk_size -= 2;
    
    // Extension Size (if it is an extension)
    if(format_chunk_size > 0) {
        for(i = 0; i < 2; i++) {
            cufo_read_char(reader, fptr);
            wave->ext.extension_size += ((int) reader->c) << (8 * i);
        }
        format_chunk_size -= 2;
    }

    // Number of valid bits
    if(format_chunk_size > 0) {
        for(i = 0; i < 2; i++) {
            cufo_read_char(reader, fptr);
            wave->ext.num_valid_bits += ((int) reader->c) << (8 * i);
        }
        format_chunk_size -= 2;
    }

    // Speaker position mask
    if(format_chunk_size > 0) {
        for(i = 0; i < 4; i++) {
            cufo_read_char(reader, fptr);
            wave->ext.speaker_position_mask += ((int) reader->c) << (8 * i);
        }
        format_chunk_size -= 4;
    }

    // SubFormat GUID
    if(format_chunk_size > 0) {
        for(i = 0; i < 4; i++) {
            cufo_read_char(reader, fptr);
            wave->ext.speaker_position_mask += ((int) reader->c) << (8 * i);
        }
        format_chunk_size -= 16;
    }

    // Assert format_chunk_size is 0
    if(format_chunk_size != 0) {
        printf("ERROR::cufo::WAV::DATA_FORMAT_CHUNK - Error while parsing data format chunk. (format_chunk_size must be 16, 18, or 40)\n");
        return 1;
    }

    return 0;
}

int wave_parse_fact_chunk(t_cufo_reader* reader, FILE* fptr, t_wave* wave) {
    int i;
    int fact_chunk_size = 0;
    wave->fact.num_samples_per_channel = 0;
    wave->has_fact = 1;

    // Read chunk size
    for(i = 0; i < 4; i++) {
        cufo_read_char(reader, fptr);
        fact_chunk_size += ((int) reader->c) << (8 * i);
    }

    // Assert fact_chunk_size >= 4
    if(fact_chunk_size < 4) {
        printf("ERROR::cufo::WAV::FACT_CHUNK_SIZE - Fact chunk must at least be 4 bytes.\n");
        return 1;
    }

    // Read Number of Samples (per channel)
    for(i = 0; i < fact_chunk_size; i++) {
        cufo_read_char(reader, fptr);
        wave->fact.num_samples_per_channel += ((int) reader->c) << (8 * i);
    }
    return 0;
}

/**
 *  Util: get the nth bit (left is more-significant)
 */
char get_nth_bit(int b, int n) {
    return (b >> n) & 0x1;
}

/**
 *  Util: print binary string (no newline)
 */
void print_bin(int b, int startbit, int endbit) {
    for(int i = startbit; i >= endbit; i--) {
        printf("%d", get_nth_bit(b, i));
    }
}

int _wave_process_block_ieee_float(t_wave* wave, char* block, double* sample) {
    int err;
    int i, j;

    *sample = 0.0;
    switch(wave->data_block_size) {
        case 4:         // Single Precision
            union {
                char c[4];
                float f;
            } s;
            for(i = 0; i < 4; i++) {
                s.c[i] = block[i];
            }
            *sample = (double) s.f;
            break;
        case 8:         // Double Precision
            union {
                char c[8];
                double d;
            } sd;
            for(i = 0; i < 8; i++) {
                s.c[i] = block[i];
            }
            *sample = sd.d;
            break;
        default:
            printf("ERROR::cufo::WAVE::PROCESS_DATA::IEEE - Unsupported block size for IEEE 754 float\n");
            return 1;
    }

    return 0;
}

int wave_parse_data_chunk(t_cufo_reader* reader, FILE* fptr, t_wave* wave) {
    int i,j,k,err;
    int data_chunk_size = 0;
    wave->data.padded = 0;
    wave->data.is_loaded = 0;

    // Read chunk size
    for(i = 0; i < 4; i++) {
        cufo_read_char(reader, fptr);
        data_chunk_size += ((int) reader->c) << (8 * i);
    }

    // Read data
    t_cufo_value value;
    cufo_array_init(&wave->_blocked_samples);
    for(i = 0; i < data_chunk_size; i++) {
        value.ival = cufo_read_char(reader, fptr);
        cufo_array_insert(&wave->_blocked_samples, cufoREAL, value);
    }

    // Read pad if data chunk size is odd
    if(data_chunk_size % 2 == 1) {
        wave->data.padded = 1;
        value.ival = cufo_read_char(reader, fptr);
        cufo_array_insert(&wave->_blocked_samples, cufoREAL, value);
    }

    // Process the data according to the format
    double sample = 0.0;
    char *block = (char*) malloc(wave->data_block_size * sizeof(char));
    wave->data.samples = (double**) malloc(wave->num_channels * sizeof(double*));
    for(i = 0; i < wave->num_channels; i++) {
        wave->data.samples[i] = (double*) malloc((wave->_blocked_samples.size / wave->num_channels) * sizeof(double));
    }
    for(i = 0; i < data_chunk_size; i += wave->data_block_size) {
        for(j = 0; j < wave->num_channels; j++) {
            for(k = 0; k < wave->data_block_size / wave->num_channels; k++) {
                block[k] = wave->_blocked_samples.values[i+j+k].ival;
            }
            switch(wave->format_code) {
                case WAVE_FORMAT_PCM:
                    break;
                case WAVE_FORMAT_IEEE_FLOAT:
                    err = _wave_process_block_ieee_float(wave, block, wave->data.samples[j] + (i / wave->data_block_size));
                    if(err) {
                        printf("ERROR::cufo::WAVE::PARSE_IEEE_FLOAT - Error processing ieee float.\n");

                        cufo_array_free(&wave->_blocked_samples);
                        free(block);
                        return err;
                    }
                    break;
            }
        }
    }
    wave->data.size = (wave->_blocked_samples.size / wave->data_block_size);
    cufo_array_free(&wave->_blocked_samples);
    free(block);

    wave->data.is_loaded = 1;
    return 0;
}

int wave_parse_list_chunk(t_cufo_reader *reader, FILE *fptr, t_wave *wave) {
    int i;
    char chunk_size[4];
    char riff_list_type_ID[4];
    
    // Read size
    for(i = 0; i < 4; i++) {
        cufo_read_char(reader, fptr);
        chunk_size[i] = reader->c;
    }
    // List type ID
    for(i = 0; i < 4; i++) {
        cufo_read_char(reader, fptr);
        riff_list_type_ID[i] = reader->c;
    }

    return 1;
}

int wave_parse_chunk(t_cufo_reader* reader, FILE* fptr, t_wave* wave) {
    // Read Chunk ID
    e_wave_chunk_type chunk_type;
    chunk_type = wave_read_chunk_id(reader, fptr);
    if(chunk_type == WAVE_UNKNOWN_FORMAT_CHUNK) {
        printf("ERROR::cufo::WAV::CHUNK_ID - Either unknown chunk type or error reading chunk id\n");
        return 1;
    }

    switch(chunk_type) {
        case WAVE_FILE_FORMAT_CHUNK:
            return wave_parse_file_format_chunk(reader, fptr, wave);
        case WAVE_DATA_FORMAT_CHUNK:
            return wave_parse_data_format_chunk(reader, fptr, wave);
        case WAVE_FACT_CHUNK:
            return wave_parse_fact_chunk(reader, fptr, wave);
        case WAVE_LIST_CHUNK:
            return wave_parse_list_chunk(reader, fptr, wave);
        case WAVE_DATA_CHUNK:
            return 1;
            // return wave_parse_data_chunk(reader, fptr, wave);
        default:
            printf("ERROR::cufo::WAV::CHUNK_ID - Currently unsupported wave chunk type\n");
            return 1;
    }
}

int cufo_wave(t_wave* wave, char* filename) {
    int err;
    FILE* fptr;
    
    fptr = fopen(filename, "rb");

    if(!fptr) {
        printf("ERROR::cufo::WAV - Error opening file.\n");
        return 1;
    }

    t_cufo_reader reader;
    reader.c = 0;
    reader.putback = 0;

    while(reader.c != EOF) {
        // Parse chunk
        err = wave_parse_chunk(&reader, fptr, wave);
        if(err) {
            fclose(fptr);
            wave_free(wave);
            return err;
        }
        cufo_read_char(&reader, fptr);
        if(reader.c == EOF) break;
        cufo_putback(&reader);
    }
    
    fclose(fptr);
    return err;
}