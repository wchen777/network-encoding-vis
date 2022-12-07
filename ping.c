#include "ping.h"

/*
 * setup a sender to send to our own udp listener
 */
void setup_sender(ping_settings_t *ping_config) {
    struct sockaddr_in *addr = malloc(sizeof(struct sockaddr_in)); // addr of client's listener

    if (addr == NULL) {
        perror("malloc, setup udp conn");
        return -1;
    }

    memset(addr, 0, sizeof(*addr));

    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        return -1;
    }

    // set fd
    ping_config->udp_sock = sockfd;

    // filling addr information
    addr->sin_family = AF_INET;
    addr->sin_port = htons(ping_config->udp_port);
    addr->sin_addr.s_addr = inet_addr("127.0.0.1");

    // set our udp_addr
    ping_config->udp_addr = addr;
}

/*
 * send to our loopback listener
 */
int ping_self(ping_settings_t *ping_config, char *data) {
    sendto(ping_config->udp_sock, data, strlen(data), 0,
           (const struct sockaddr *) ping_config->udp_addr,  sizeof(*ping_config->udp_addr));

}