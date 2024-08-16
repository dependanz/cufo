#include "jsonhash.h"

/**
 * SHA256 (implementation from julius)
 */
char json_kc[64][4] = {
        {0b01000010, 0b10001010, 0b00101111, 0b10011000},
        {0b01110001, 0b00110111, 0b01000100, 0b10010001},
        {0b10110101, 0b11000000, 0b11111011, 0b11001111},
        {0b11101001, 0b10110101, 0b11011011, 0b10100101},
        {0b00111001, 0b01010110, 0b11000010, 0b01011011},
        {0b01011001, 0b11110001, 0b00010001, 0b11110001},
        {0b10010010, 0b00111111, 0b10000010, 0b10100100},
        {0b10101011, 0b00011100, 0b01011110, 0b11010101},
        {0b11011000, 0b00000111, 0b10101010, 0b10011000},
        {0b00010010, 0b10000011, 0b01011011, 0b00000001},
        {0b00100100, 0b00110001, 0b10000101, 0b10111110},
        {0b01010101, 0b00001100, 0b01111101, 0b11000011},
        {0b01110010, 0b10111110, 0b01011101, 0b01110100},
        {0b10000000, 0b11011110, 0b10110001, 0b11111110},
        {0b10011011, 0b11011100, 0b00000110, 0b10100111},
        {0b11000001, 0b10011011, 0b11110001, 0b01110100},
        {0b11100100, 0b10011011, 0b01101001, 0b11000001},
        {0b11101111, 0b10111110, 0b01000111, 0b10000110},
        {0b00001111, 0b11000001, 0b10011101, 0b11000110},
        {0b00100100, 0b00001100, 0b10100001, 0b11001100},
        {0b00101101, 0b11101001, 0b00101100, 0b01101111},
        {0b01001010, 0b01110100, 0b10000100, 0b10101010},
        {0b01011100, 0b10110000, 0b10101001, 0b11011100},
        {0b01110110, 0b11111001, 0b10001000, 0b11011010},
        {0b10011000, 0b00111110, 0b01010001, 0b01010010},
        {0b10101000, 0b00110001, 0b11000110, 0b01101101},
        {0b10110000, 0b00000011, 0b00100111, 0b11001000},
        {0b10111111, 0b01011001, 0b01111111, 0b11000111},
        {0b11000110, 0b11100000, 0b00001011, 0b11110011},
        {0b11010101, 0b10100111, 0b10010001, 0b01000111},
        {0b00000110, 0b11001010, 0b01100011, 0b01010001},
        {0b00010100, 0b00101001, 0b00101001, 0b01100111},
        {0b00100111, 0b10110111, 0b00001010, 0b10000101},
        {0b00101110, 0b00011011, 0b00100001, 0b00111000},
        {0b01001101, 0b00101100, 0b01101101, 0b11111100},
        {0b01010011, 0b00111000, 0b00001101, 0b00010011},
        {0b01100101, 0b00001010, 0b01110011, 0b01010100},
        {0b01110110, 0b01101010, 0b00001010, 0b10111011},
        {0b10000001, 0b11000010, 0b11001001, 0b00101110},
        {0b10010010, 0b01110010, 0b00101100, 0b10000101},
        {0b10100010, 0b10111111, 0b11101000, 0b10100001},
        {0b10101000, 0b00011010, 0b01100110, 0b01001011},
        {0b11000010, 0b01001011, 0b10001011, 0b01110000},
        {0b11000111, 0b01101100, 0b01010001, 0b10100011},
        {0b11010001, 0b10010010, 0b11101000, 0b00011001},
        {0b11010110, 0b10011001, 0b00000110, 0b00100100},
        {0b11110100, 0b00001110, 0b00110101, 0b10000101},
        {0b00010000, 0b01101010, 0b10100000, 0b01110000},
        {0b00011001, 0b10100100, 0b11000001, 0b00010110},
        {0b00011110, 0b00110111, 0b01101100, 0b00001000},
        {0b00100111, 0b01001000, 0b01110111, 0b01001100},
        {0b00110100, 0b10110000, 0b10111100, 0b10110101},
        {0b00111001, 0b00011100, 0b00001100, 0b10110011},
        {0b01001110, 0b11011000, 0b10101010, 0b01001010},
        {0b01011011, 0b10011100, 0b11001010, 0b01001111},
        {0b01101000, 0b00101110, 0b01101111, 0b11110011},
        {0b01110100, 0b10001111, 0b10000010, 0b11101110},
        {0b01111000, 0b10100101, 0b01100011, 0b01101111},
        {0b10000100, 0b11001000, 0b01111000, 0b00010100},
        {0b10001100, 0b11000111, 0b00000010, 0b00001000},
        {0b10010000, 0b10111110, 0b11111111, 0b11111010},
        {0b10100100, 0b01010000, 0b01101100, 0b11101011},
        {0b10111110, 0b11111001, 0b10100011, 0b11110111},
        {0b11000110, 0b01110001, 0b01111000, 0b11110010}};

void _json_word_list_right_shift(int shiftby, int nbytes, char *word, char *out)
{
    if (shiftby <= 0)
        return;                                  // for now
    
    int _shiftby;
    char *word_copy = (char*) malloc(nbytes * sizeof(char));
    for(int i = 0; i < nbytes; i++) {
        word_copy[i] = word[i];
    }
    
    while(shiftby > 0) {
        _shiftby = (shiftby >= 8) ? 8 : shiftby; // If number to shift by is larger than 8, do increments of 8.

        for (int i = nbytes - 1; i >= 0; i--)
        {
            out[i] = ((word_copy[i] & (unsigned char)(0xFF << _shiftby)) >> _shiftby);
            if (i != 0)
                out[i] += ((word_copy[i - 1] & (unsigned char)(0xFF >> (8 - _shiftby))) << (8 - _shiftby));
        }
        shiftby -= _shiftby;

        // Set word_copy to out if shiftby is still > 0
        if(shiftby > 0) {
            for(int i = 0; i < nbytes; i++) {
                word_copy[i] = out[i];
            }
        }
    }

    free(word_copy);
}

void _json_word_list_left_shift(int shiftby, int nbytes, char *word, char *out)
{
    if (shiftby <= 0)
        return;                                  // for now

    int _shiftby;
    char *word_copy = (char*) malloc(nbytes * sizeof(char));
    for(int i = 0; i < nbytes; i++) {
        word_copy[i] = word[i];
    }

    while(shiftby > 0) {
        _shiftby = (shiftby >= 8) ? 8 : shiftby; // If number to shift by is larger than 8, do increments of 8.

        for (int i = 0; i < nbytes; i++)
        {
            out[i] = ((word_copy[i] & (unsigned char)(0xFF >> _shiftby)) << _shiftby);
            if (i != nbytes - 1)
                out[i] += ((word_copy[i + 1] & (unsigned char)(0xFF << (8 - _shiftby))) >> (8 - _shiftby));
        }
        shiftby -= _shiftby;

        // Set word_copy to out if shiftby is still > 0
        if(shiftby > 0) {
            for(int i = 0; i < nbytes; i++) {
                word_copy[i] = out[i];
            }
        }
    }

    free(word_copy);
}

void json_word_list_shift(int shiftby, int nbytes, char *word, char *out)
{
    if (shiftby == 0)
        return;
    if (shiftby < 0)
        _json_word_list_left_shift(-1 * shiftby, nbytes, word, out);
    else
        _json_word_list_right_shift(shiftby, nbytes, word, out);
}

void json_word_list_right_rotate(int rotateby, int nbytes, char *word, char *out)
{
    int i;
    int point_found = 0;
    char *a = (char *)malloc(nbytes * sizeof(char));
    char *b = (char *)malloc(nbytes * sizeof(char));
    
    for (i = 0; i < nbytes; i++)
    {
        if (!point_found)
        {
            if ((8 * nbytes - rotateby) <= (8 * (i + 1)))
            {
                point_found = 1;
                a[i] = word[i] & (unsigned char)(0xFF << ((8 * (i+1)) - (8 * nbytes - rotateby)));
                b[i] = word[i] & (unsigned char)(0xFF >> (8 - ((8 * (i+1)) - (8 * nbytes - rotateby))));
            }
            else
            {
                a[i] = word[i];
                b[i] = 0x00;
            }
        }
        else
        {
            a[i] = 0x00;
            b[i] = word[i];
        }
    }

    _json_word_list_right_shift(rotateby, nbytes, a, a);
    _json_word_list_left_shift(8 * nbytes - rotateby, nbytes, b, b);

    for (i = 0; i < nbytes; i++)
    {
        out[i] = a[i] + b[i];
    }

    free(a);
    free(b);
}

void json_word_list_xor(int nbytes, char *a, char *b, char *out)
{
    for (int i = 0; i < nbytes; i++)
    {
        out[i] = a[i] ^ b[i];
    }
}

void json_word_list_and(int nbytes, char *a, char *b, char *out)
{
    for (int i = 0; i < nbytes; i++)
    {
        out[i] = a[i] & b[i];
    }
}

void json_word_list_not(int nbytes, char *a, char *out)
{
    for (int i = 0; i < nbytes; i++)
    {
        out[i] = ~a[i];
    }
}

void json_word_list_sum(int nbytes, char *a, char *b, char *out) {
    int i;
    unsigned int check;
    char carry = 0;
    char *c = (char*)malloc(nbytes * sizeof(char));
    for (i = nbytes - 1; i >= 0; i--) {
        c[i] = a[i] + b[i];
        c[i] += (carry ? 1 : 0);
        check = ((unsigned int)(unsigned char)a[i] + (unsigned int)(unsigned char)b[i] + (unsigned int)(unsigned char)carry);
        carry = (check >= 0x100 ? 1 : 0);

        // printf("s{%d, c=%d, a=%d, b=%d, test=%d, test2=%d, carry=%d} ", i, c[i], a[i], b[i], check, (unsigned int) 0xFF, carry);
    }
    for(i = 0; i < nbytes; i++) {
        out[i] = c[i];
    }
    free(c);
}

void _json_string_sha256(t_json_string* message, char* out) {
    int i, j, k;

    // K Constants (first 32 bits of the fractional parts of the cube roots of the first 64 primes)
    
    long long M_l = 8 * message->size;                  // Number of Bits of message.
    char message_length[8] = {0, 0, 0, 0, 0, 0, 0, 0};  // Message Length portion of Message Block
    for (i = 7; i >= 0; i--)
    {
        for (j = 0; j < 8; j++) {
            message_length[i] += ((M_l >> (j + 8 * (7-i))) & 0x01) << j;
        }
    }
    long num_chunks = ((int)ceil((double)(M_l + 64 + 8) / 512.0));
    long message_block_length = 512 * num_chunks;
    long remaining_bits = message_block_length - (M_l + message->size + 1);

    char *message_block = (char *)malloc(message_block_length * sizeof(char));
    for (i = 0; i < message->size; i++)
    {
        // Convert to UTF-8 (TODO)
        message_block[i] = message->str[i];
    }
    message_block[message->size] = 0x80;
    for (i = message->size + 1; i < (int)(message_block_length / 8) - 8; i++)
    {
        message_block[i] = 0;
    }
    j = 0;
    for (; i < (int)(message_block_length / 8); i++)
    {
        message_block[i] = message_length[j];
        j += 1;
    }

    // Split message block into 512-bit chunks
    char **chunks = (char **)malloc(num_chunks * sizeof(char *));
    for (i = 0; i < num_chunks; i++)
    {
        chunks[i] = (char *)malloc(64 * sizeof(char));
        for (j = 0; j < 64; j++)
        {
            chunks[i][j] = message_block[64 * i + j];
        }
    }

    // Init message_schedule_array
    char **message_schedule_array = (char **)malloc(64 * sizeof(char *));
    for (i = 0; i < 64; i++)
    {
        message_schedule_array[i] = (char *)malloc(4 * sizeof(char));
    }
    
    // Init values for message schedule array
    char sigma_0[4] = {0, 0, 0, 0};
    char sigma_1[4] = {0, 0, 0, 0};
    char w_m15_rr7[4] = {0, 0, 0, 0};  // Word i-15 rightrotate 7
    char w_m15_rr18[4] = {0, 0, 0, 0}; // Word i-15 rightrotate 18
    char w_m15_rs3[4] = {0, 0, 0, 0};  // Word i-15 rightshift 3
    char w_m2_rr17[4] = {0, 0, 0, 0};  // Word i-2 rightrotate 17
    char w_m2_rr19[4] = {0, 0, 0, 0};  // Word i-2 rightrotate 19
    char w_m2_rs10[4] = {0, 0, 0, 0};  // Word i-2 rightshift 10

    // Init Hash Values
    char h[8][4] = {
        {0b01101010, 0b00001001, 0b11100110, 0b01100111},
        {0b10111011, 0b01100111, 0b10101110, 0b10000101},
        {0b00111100, 0b01101110, 0b11110011, 0b01110010},
        {0b10100101, 0b01001111, 0b11110101, 0b00111010},
        {0b01010001, 0b00001110, 0b01010010, 0b01111111},
        {0b10011011, 0b00000101, 0b01101000, 0b10001100},
        {0b00011111, 0b10000011, 0b11011001, 0b10101011},
        {0b01011011, 0b11100000, 0b11001101, 0b00011001}};

    // Init Working Variables
    char av[4] = {0, 0, 0, 0};
    char bv[4] = {0, 0, 0, 0};
    char cv[4] = {0, 0, 0, 0};
    char dv[4] = {0, 0, 0, 0};
    char ev[4] = {0, 0, 0, 0};
    char fv[4] = {0, 0, 0, 0};
    char gv[4] = {0, 0, 0, 0};
    char hv[4] = {0, 0, 0, 0};
    char choice[4] = {0, 0, 0, 0};
    char choice_a[4] = {0, 0, 0, 0};
    char choice_b[4] = {0, 0, 0, 0};
    char choice_c[4] = {0, 0, 0, 0};
    char majority[4] = {0, 0, 0, 0};
    char majority_a[4] = {0, 0, 0, 0};
    char majority_b[4] = {0, 0, 0, 0};
    char majority_c[4] = {0, 0, 0, 0};
    char temp1[4] = {0, 0, 0, 0};
    char temp2[4] = {0, 0, 0, 0};
    char S0[4] = {0, 0, 0, 0};
    char S0a[4] = {0, 0, 0, 0};
    char S0b[4] = {0, 0, 0, 0};
    char S0c[4] = {0, 0, 0, 0};
    char S1[4] = {0, 0, 0, 0};
    char S1a[4] = {0, 0, 0, 0};
    char S1b[4] = {0, 0, 0, 0};
    char S1c[4] = {0, 0, 0, 0};
    
    /**
     * SHA-256 Algorithm.
     */
    int chunk_idx;
    for (chunk_idx = 0; chunk_idx < num_chunks; chunk_idx++)
    {
        // printf("|%d| CHUNK MESSAGE SCHEDULE ARRAY\n", chunk_idx);
        // Copy a chunk into message schedule array;
        for (i = 0; i < 64; i++)
        {
            for (j = 0; j < 4; j++)
            {
                if (i >= 16)
                {
                    message_schedule_array[i][j] = 0;
                }
                else
                {
                    message_schedule_array[i][j] = chunks[chunk_idx][4 * i + j];
                }
            }
        }
        
        // Calculate Message Schedule Array
        for (i = 16; i < 64; i++)
        {
            // Right Rotate 7 (sigma_0)
            json_word_list_right_rotate(7, 4, message_schedule_array[i - 15], w_m15_rr7);

            // Right Rotate 18 (sigma_0)
            json_word_list_right_rotate(18, 4, message_schedule_array[i - 15], w_m15_rr18);

            // Right Shift 3 (sigma_0)
            _json_word_list_right_shift(3, 4, message_schedule_array[i - 15], w_m15_rs3);

            // Sigma 0
            for (j = 0; j < 4; j++)
            {
                sigma_0[j] = w_m15_rr7[j] ^ w_m15_rr18[j];
                sigma_0[j] = sigma_0[j] ^ w_m15_rs3[j];
            }

            // Right Rotate 17 (sigma_1)
            json_word_list_right_rotate(17, 4, message_schedule_array[i - 2], w_m2_rr17);

            // Right Rotate 19  (sigma_1)
            json_word_list_right_rotate(19, 4, message_schedule_array[i - 2], w_m2_rr19);

            // Right Shift 10  (sigma_1)
            _json_word_list_right_shift(10, 4, message_schedule_array[i - 2], w_m2_rs10);

            // Sigma 1
            for (j = 0; j < 4; j++)
            {
                sigma_1[j] = w_m2_rr17[j] ^ w_m2_rr19[j];
                sigma_1[j] = sigma_1[j] ^ w_m2_rs10[j];
            }

            // Calculate next word
            json_word_list_sum(4, message_schedule_array[i - 16], sigma_0, message_schedule_array[i]);
            json_word_list_sum(4, message_schedule_array[i], message_schedule_array[i - 7], message_schedule_array[i]);
            json_word_list_sum(4, message_schedule_array[i], sigma_1, message_schedule_array[i]);
        }
        
        // Init working variables to hash values
        for(j = 0; j < 4; j++) {
            av[j] = h[0][j];
            bv[j] = h[1][j];
            cv[j] = h[2][j];
            dv[j] = h[3][j];
            ev[j] = h[4][j];
            fv[j] = h[5][j];
            gv[j] = h[6][j];
            hv[j] = h[7][j];
        }

        for (i = 0; i < 64; i++)
        {
            // Calculate S0 Variable
            json_word_list_right_rotate(2, 4, av, S0a);
            json_word_list_right_rotate(13, 4, av, S0b);
            json_word_list_right_rotate(22, 4, av, S0c);
            json_word_list_xor(4, S0a, S0b, S0b);
            json_word_list_xor(4, S0b, S0c, S0);

            // Calculate S1 Variable
            json_word_list_right_rotate(6, 4, ev, S1a);
            json_word_list_right_rotate(11, 4, ev, S1b);
            json_word_list_right_rotate(25, 4, ev, S1c);
            json_word_list_xor(4, S1a, S1b, S1b);
            json_word_list_xor(4, S1b, S1c, S1);

            // Calculate Choice Variable
            json_word_list_and(4, ev, fv, choice_a);
            json_word_list_not(4, ev, choice_b);
            json_word_list_and(4, choice_b, gv, choice_c);
            json_word_list_xor(4, choice_a, choice_c, choice);

            // Calculate Majority Variable
            json_word_list_and(4, av, bv, majority_a);
            json_word_list_and(4, av, cv, majority_b);
            json_word_list_and(4, bv, cv, majority_c);
            json_word_list_xor(4, majority_a, majority_b, majority_b);
            json_word_list_xor(4, majority_b, majority_c, majority);

            // Calcualte Temp2
            json_word_list_sum(4, S0, majority, temp2);

            // Calculate Temp1
            json_word_list_sum(4, hv, S1, temp1);
            json_word_list_sum(4, temp1, choice, temp1);
            json_word_list_sum(4, temp1, json_kc[i], temp1);
            json_word_list_sum(4, temp1, message_schedule_array[i], temp1);

            // Update Working Variables
            for (j = 0; j < 4; j++)
            {
                hv[j] = gv[j];
                gv[j] = fv[j];
                fv[j] = ev[j];
            }
            json_word_list_sum(4, dv, temp1, ev);
            for (j = 0; j < 4; j++)
            {
                dv[j] = cv[j];
                cv[j] = bv[j];
                bv[j] = av[j];
            }
            json_word_list_sum(4, temp1, temp2, av);
        }

        // Add working variables to current hash value
        json_word_list_sum(4, h[0], av, h[0]);
        json_word_list_sum(4, h[1], bv, h[1]);
        json_word_list_sum(4, h[2], cv, h[2]);
        json_word_list_sum(4, h[3], dv, h[3]);
        json_word_list_sum(4, h[4], ev, h[4]);
        json_word_list_sum(4, h[5], fv, h[5]);
        json_word_list_sum(4, h[6], gv, h[6]);
        json_word_list_sum(4, h[7], hv, h[7]);
    }
    
    // Final SHA-256 value
    for(i = 0; i < 8; i++) {
        for(j = 0; j < 4; j++) {
            out[4 * i + j] = h[i][j];
        }
    }

    // Garbage Collection
    for (i = 0; i < 64; i++)
    {
        free(message_schedule_array[i]);
    }
    for (i = 0; i < num_chunks; i++)
    {
        free(chunks[i]);
    }
    free(message_schedule_array);
    free(chunks);
    free(message_block);
}

char* json_sha256(char* message) {
    t_json_string str_message;
    json_string_new_from_cstr(&str_message, strlen(message), message);

    char *out = (char*) malloc(32 * sizeof(char));
    _json_string_sha256(&str_message, out);

    json_string_free(&str_message);
    return out;
}