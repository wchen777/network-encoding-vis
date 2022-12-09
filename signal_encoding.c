#include "signal_encoding.h"

void decode_char(char c, int curr_char, short* bit_array) {
    int index = curr_char; 
    for (int i = CHAR_BIT - 1; i>=0; --i) {
        bit_array[index] = BITSLOT(c, i); 
        index++; 
    }
}

void decode_characters(char* data, short* bit_array) {
    for (int i = 0; *(data + i) != '\0'; i++) {
        decode_char(*(data + i), i*CHAR_BIT, bit_array); 
    }
}

/*
    Used for all the different encodings. 
    Fills the line buffers with a given signal "segment"
*/
void fill_signal(bit_signal_pair_t* curr, bit_signal_pair_t* prev, char* bit_line,
                 char* top_line, char* mid_line, char* bottom_line) {
    // if the prev signal doesn't match our first
    int needs_filler = prev && prev->transition[1] != curr->transition[0]; 

    // each segment's string
    char mid[MAX_SEG_CHARS];
    char top[MAX_SEG_CHARS];
    char bottom[MAX_SEG_CHARS];

    memset(mid, 0, MAX_SEG_CHARS);
    memset(top, 0, MAX_SEG_CHARS); 
    memset(bottom, 0, MAX_SEG_CHARS); 

    // add the bit to the line
    strcat(bit_line, curr->bit ? "  1" : "  0");

    top[0] = needs_filler ? ' ' : '\0'; 
    mid[0] = needs_filler ? '|' : '\0';
    bottom[0] = needs_filler ? '|' : '\0';

    // increase the size of the segment if we need to
    if (needs_filler)
        strcat(bit_line, " ");

    // "     ____"
    // "    |  "
    // "____|  
    
    // low to hi transition
    // "    __"
    // "|  |  "
    // "|__|  "
    
    // fill each individual segment string
    if (curr->transition[0] == LO && curr->transition[1] == LO) {
        // "    "
        // "    "
        // "____" 
        strcat(bit_line, " "); // 1 less than the other line sizes to keep alignment
        strcat(top, "    ");  
        strcat(mid, "    "); 
        strcat(bottom, "____"); 
    } else if (curr->transition[0] == HI && curr->transition[1] == HI) {
        strcat(bit_line, " "); // 1 less than the other line sizes to keep alignment
        strcat(top, "____"); 
        strcat(mid, "    "); 
        strcat(bottom, "    "); 
    }
    // TODO: other 2 combinations


    // fill accumulating string with segment string
    strcat(top_line, top);
    strcat(mid_line, mid);
    strcat(bottom_line, bottom);
}

void print_signal(char* bit_line, char* top_line, char* mid_line, char* bottom_line, char* encoding_type, char* data_raw) {
    // TODO: print out the packet data and encoding protocol

    // newlines for pretty print
    fprintf(stdout, "\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Encoding Type: %s\n", encoding_type);
    fprintf(stdout, "Data Received: %s", data_raw);
    fprintf(stdout, "----------------------------\n");
    fprintf(stdout, "\n");
    
    fprintf(stdout, "\t\t\t"); 
    fprintf(stdout, "%s\n", bit_line); 
    fprintf(stdout, "\t\t\t"); 
    fprintf(stdout, "%s\n", top_line); 
    fprintf(stdout, "\t\t\t"); 
    fprintf(stdout, "%s\n", mid_line); 
    fprintf(stdout, "\t\t\t"); 
    fprintf(stdout, "%s\n", bottom_line); 
}

void construct_bit_arr_nrz(short* bit_array, int len, bit_signal_pair_t* signal_pair_array) {
    bit_signal_pair_t bit_signal_pair; 

    for (int i = 0; i < len; i++) {
        if (!bit_array[i]) {
            bit_signal_pair.transition[0] = LO;
            bit_signal_pair.transition[1] = LO; 
        } else {
            bit_signal_pair.transition[0] = HI;
            bit_signal_pair.transition[1] = HI; 
        }
        bit_signal_pair.bit = bit_array[i];
        signal_pair_array[i] = bit_signal_pair; 
    }
}


// 0 --> LO 
// 1 --> HI 
void visualize_nrz(short *bit_array, int len, char* data_raw, float pace) {
    // create the array of signal structs
    bit_signal_pair_t signal_pair_array[BIT_ARR_SIZE]; 
    memset(signal_pair_array, 0, BIT_ARR_SIZE * sizeof(bit_signal_pair_t));

    // fill the array of signal structs
    construct_bit_arr_nrz(bit_array, len, signal_pair_array);
    // visualize to the screen
    print_visualization(signal_pair_array, len, data_raw, pace);
}

void print_visualization(bit_signal_pair_t* signal_pair_array, int len, char* data_raw, float pace) {
    // sliding window over bit buffer
    int bit_buffer_offset = 0; // where the screen's window starts at
    char* bit_line_string = malloc(WINDOW_SIZE * MAX_SEG_CHARS);
    char* top_line_string = malloc(WINDOW_SIZE * MAX_SEG_CHARS);
    char* middle_line_string = malloc(WINDOW_SIZE * MAX_SEG_CHARS);
    char* bottom_line_string = malloc(WINDOW_SIZE * MAX_SEG_CHARS);

    // simulate a moving wire using a sliding window
    while (bit_buffer_offset < len) {
        bit_signal_pair_t* prev;

        // clear our console strings
        memset(bit_line_string, 0, WINDOW_SIZE * MAX_SEG_CHARS);
        memset(top_line_string, 0, WINDOW_SIZE * MAX_SEG_CHARS);
        memset(middle_line_string, 0, WINDOW_SIZE * MAX_SEG_CHARS);
        memset(bottom_line_string, 0, WINDOW_SIZE * MAX_SEG_CHARS);

        // print out a window of the bit array to the console
        for (int i = 0; i < MIN(WINDOW_SIZE, len - bit_buffer_offset); i++) {
            // where the printing magic happens
            // populate the arrays with each character
            fill_signal(&signal_pair_array[i+bit_buffer_offset], prev, bit_line_string, top_line_string, middle_line_string, bottom_line_string);
            // set prev to be the current
            prev = &signal_pair_array[i+bit_buffer_offset];
        }

        // clear the terminal to reset the console window
        system("clear");
        // print each individual line to the console
        if (len - bit_buffer_offset > 0) {
            print_signal(bit_line_string, top_line_string, middle_line_string, bottom_line_string, "NRZ", data_raw);
        }

        // sleep so that the clear doesn't happen right away
        usleep(80000 + ((1.0 - pace) * 800000));

        bit_buffer_offset++; // increment starting window index
        prev = NULL; // reset prev pointer to null for each window
    }

    system("clear");
    free(bit_line_string);
    free(top_line_string);
    free(middle_line_string);
    free(bottom_line_string);

}


void visualize_nrzi(short *bit_array, int len, char* data_raw, float pace) {
    // // convert char buffer into a bit buffer

}

void visualize_manchester(short *bit_array, int len, char* data_raw, float pace) {

}

void visualize_block(short *bit_array, int len, char* data_raw, float pace) {

}

