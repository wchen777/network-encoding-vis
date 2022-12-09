#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_PACKET 1024
#define MIN(a, b) ((a < b) ? a : b)

// settings for sending to our own loopback listener
typedef struct ping_settings {
    int udp_sock; // fd of our current udp sender
    uint16_t udp_port; // port of the udp listener
    struct sockaddr_in *udp_addr; // addr of the udp listener
} ping_settings_t;

void setup_sender(ping_settings_t *ping_config);
int ping_self(ping_settings_t *ping_config, char* data);

