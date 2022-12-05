#define _GNU_SOURCE

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

#include "visualizer.h"
#define MAX_PACKET 2048


/*
 * global visualizer settings struct
 */
visualizer_settings_t settings = {NRZ_L};



/*
 * run the visualizer. shows the physical wire on stdout
 */
int run_visualizer(char* data) {
    // TODO: this.
    return 0;
}

/*
 * start a udp listener listening for incoming packets on a given port.
 * when a packet is received,
 */
void start_udp_listener(uint16_t udp_port) {
    // open up UDP port
    int udp_sockfd;
    struct sockaddr_in servaddr, cliaddr;

    // Creating socket file descriptor
    if ((udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family    = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // hard code localhost
    servaddr.sin_port = htons(udp_port);

    // Bind the socket with the server address
    if (bind(udp_sockfd, (const struct sockaddr *)&servaddr,
             sizeof(servaddr)) < 0 )
    {
        perror("bind failed");
    }

    fprintf(stderr, "Visualizer ready on port: %i\n", udp_port);

    while(1) {
        char buffer[MAX_PACKET];
        ssize_t bytes_rec;
        unsigned int len = sizeof(cliaddr);

        bytes_rec = recvfrom(udp_sockfd, buffer, MAX_PACKET, 0, (struct sockaddr *) &cliaddr, &len);

        // dump data into stdout
        fwrite(buffer, bytes_rec, 1, stdout);

        // TODO: visualize here
    }

}

int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <listener_port>\n", argv[0]);
        exit(1);
    }
    fprintf(stderr, "Welcome to the network encoding visualizer. Here are a list of supported commands:\n");

    char* listener_port = argv[1];
    uint16_t udp_port = atoi(listener_port);

    start_udp_listener(udp_port);

//    close(udp_port);

}