typedef enum signal_value {
    LO,
    HI
} signal_value_t;

typedef struct bit_signal_pair {
    signal_value_t transition[2];
    int bit;
} bit_signal_pair_t;