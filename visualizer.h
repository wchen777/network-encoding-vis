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

#include "ping.h"

typedef enum encoding_type {
    NRZ,
    NRZ_I,
    MANCHESTER_BIPHASE,
    MANCHESTER_DIFFERENTIAL,
    BLOCK_4B5B
} encoding_type_t ;

typedef struct visualizer_settings {
    encoding_type_t encoding_type; // current type of visualization
    float pace; // how fast the visualization should be
    pthread_mutex_t vis_mutex; // mutex to protect settings fields
    pthread_t vis_thread; // the thread that runs the visualizer
} visualizer_settings_t;