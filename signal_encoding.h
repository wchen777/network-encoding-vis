#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef enum signal_value {
    LO,
    HI
} signal_value_t;

typedef struct bit_signal_pair {
    signal_value_t transition[2];
    int bit;
} bit_signal_pair_t;

#define BIT_ARR_SIZE 1024 * CHAR_BIT
#define BIT_ARR_SIZE_BLOCK 1280 * CHAR_BIT

#define WINDOW_SIZE 12
#define MAX_SEG_CHARS 6
#define MIN(a, b) ((a < b) ? a : b)

// get the nth bit from b
#define BITSLOT(b, n) (((b) >> (n)) & 1)

void visualize_nrz(short *bit_array, int len, char* data_raw, float pace);
void construct_bit_arr_nrz(short* bit_array, int len, bit_signal_pair_t* signal_pair_array);

void visualize_nrzi(short *bit_array, int len, char* data_raw, float pace);
void construct_bit_arr_nrzi(short* bit_array, int len, bit_signal_pair_t* signal_pair_array);

void visualize_manchester(short *bit_array, int len, char* data_raw, float pace);
void construct_bit_arr_manchester(short* bit_array, int len, bit_signal_pair_t* signal_pair_array);

void visualize_block(short *bit_array, int len, char* data_raw, float pace);

void fill_signal(bit_signal_pair_t* curr, bit_signal_pair_t* prev, char* bit_line, char* top_line, char* mid_line, char* bottom_line);
void print_signal(char* bit_line, char* top_line, char* mid_line, char* bottom_line, char* encoding_type, char* data_raw);

void decode_char(char c, int curr_char, short* bit_array);
void decode_characters(char* data, short* bit_array);
void decode_block(short* bit_array, short* modified_bit_array, int len);

void print_visualization(bit_signal_pair_t* signal_pair_array, int len, char* data_raw, float pace, char* enc_type);