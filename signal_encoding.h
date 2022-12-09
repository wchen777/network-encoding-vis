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
#define WINDOW_SIZE 32
#define MAX_SEG_CHARS 6
#define MIN(a, b) ((a < b) ? a : b)

// static char BLOCK_4B5B_TABLE[16] = {
//     0b11110, // 0000
//     0b01001, // 0001
//     0b10100, // 0010
//     0b10101, // 0011

//     0b01010, // 0100
//     0b01011, // 0101
//     0b01110, // 0110
//     0b01111, // 0111

//     0b10010, // 1000
//     0b10011, // 1001
//     0b10110, // 1010
//     0b10111, // 1011

//     0b11010, // 1100
//     0b11011, // 1101
//     0b11100, // 1110
//     0b11101  // 1111
// };

// get the nth bit from b
#define BITSLOT(b, n) (((b) >> (n)) & 1)

void visualize_nrz(short *bit_array, int len);
void construct_bit_arr_nrz(short* bit_array, int len, bit_signal_pair_t* signal_pair_array);
void visualize_nrzi(short *bit_array, int len);
void visualize_manchester(short *bit_array, int len);
void visualize_block(short *bit_array, int len);

void fill_signal(bit_signal_pair_t* curr, bit_signal_pair_t* prev, char* bit_line, char* top_line, char* mid_line, char* bottom_line);
void print_signal(char* bit_line, char* top_line, char* mid_line, char* bottom_line);

void decode_char(char c, int curr_char, short* bit_array);
void decode_characters(char* data, short* bit_array);