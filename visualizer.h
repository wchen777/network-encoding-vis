#define _GNU_SOURCE

typedef enum encoding_type {
    NRZ_L,
    NRZ_I,
    MANCHESTER_BIPHASE,
    MANCHESTER_DIFFERENTIAL,
    BLOCK_
} encoding_type_t ;

typedef struct visualizer_settings {
    encoding_type_t encodingType;
} visualizer_settings_t ;