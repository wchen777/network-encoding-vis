#define _GNU_SOURCE

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef enum encoding_type {
    NRZ,
    NRZ_I,
    MANCHESTER_BIPHASE,
    MANCHESTER_DIFFERENTIAL,
    BLOCK_4B5B
} encoding_type_t ;

typedef struct visualizer_settings {
    encoding_type_t encoding_type;
    float pace;
    pthread_mutex_t vis_mutex;
    pthread_t vis_thread;
} visualizer_settings_t ;