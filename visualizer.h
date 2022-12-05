#define _GNU_SOURCE

typedef enum encoding_type {
    NRZ,
    MANCHESTER,
    BLOCK
} encoding_type_t ;

typedef struct visualizer_settings {
    encoding_type_t encodingType;
} visualizer_settings_t ;