#define _GNU_SOURCE

#include "visualizer.h"
#define MAX_PACKET 2048
#define BUFLEN 1024

/*
 * global visualizer settings struct
 */
visualizer_settings_t vis_settings = {NRZ, 1.0, PTHREAD_MUTEX_INITIALIZER};


/*
 * run the visualizer. shows the physical wire on stdout
 */
int run_visualizer(char* data) {
    // TODO: this.
    return 0;
}


/*
 * close the udp port once the listener thread terminates
 */
void listener_cleanup(void *args) {
    if (close(*((uint16_t *)args)) == -1) {
        perror("close");
    }
    return NULL;
}

/*
 * start a UDP listener listening for incoming packets on a given port.
 * when a packet is received, run the run_visualizer function
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

    pthread_cleanup_push(listener_cleanup, (void *)&udp_sockfd);

    while(1) {
        char buffer[MAX_PACKET];
        ssize_t bytes_rec;
        unsigned int len = sizeof(cliaddr);

        bytes_rec = recvfrom(udp_sockfd, buffer, MAX_PACKET, 0, (struct sockaddr *) &cliaddr, &len);

        // dump data into stdout
        fwrite(buffer, bytes_rec, 1, stdout);

        // TODO: visualize here
    }

    pthread_cleanup_pop(1);
}

/*
 * function passed into the thread that runs the visualizer UDP listener
 */
void *run_visualizer_listener_thread(void* args) {
    uint16_t* port = (uint16_t *)args;
    start_udp_listener(*port);
    return NULL;
}

/*
 * start listening for commands
 */
void start_repl() {

    char server_command[BUFLEN];
    while (fgets(server_command, BUFLEN, stdin)) {

        // get the command name (the first word of the line)
        char *command = strtok(server_command, " ");

        // get the rest of the line
        char *rest = strtok(NULL, " ");

        // COMMANDS TO CHANGE ENCODING TYPE
        if (!strcmp(command, "nrz")) {

        } else if (!strcmp(command, "nrz_i")) {

        } else if (!strcmp(command, "manchester_b")) {

        } else if (!strcmp(command, "manchester_d")) {

        } else if (!strcmp(command, "block")) {

        }


    }
}


int main(int argc, char **argv) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <listener_port>\n", argv[0]);
        exit(1);
    }
    fprintf(stderr, "Welcome to the network encoding visualizer. Here are a list of supported commands:\n");

    int err;

    char* listener_port = argv[1];
    uint16_t udp_port = atoi(listener_port);

    if ((err = pthread_create(&vis_settings.vis_thread, run_visualizer_listener_thread, (void *)&udp_port)) != 0) {
        fprintf(stderr, "pthread create failed, err: %d\n", err);
        exit(1);
    }

    // accept commands for the listener
    start_repl();

    // cancel the listener thread once the visualizer repl is terminated
    if ((err = pthread_cancel(vis_settings.vis_thread)) != 0) {
        fprintf(stderr, "pthread cancel failed, err: %d\n", err);
        exit(1);
    }

    // join with the listener thread
    if ((err = pthread_join(vis_settings.vis_thread, NULL)) != 0) {
        fprintf(stderr, "pthread join failed, err: %d\n", err);
        exit(1);
    }

    pthread_exit(0);
}