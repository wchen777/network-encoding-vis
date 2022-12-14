#include "visualizer.h"
#include "signal_encoding.h"

#define BUFLEN 1024

/*
 * global visualizer settings struct
 */
visualizer_settings_t vis_settings = {NRZ, 0.8, PTHREAD_MUTEX_INITIALIZER};
ping_settings_t ping_config;

/*
 * run the visualizer. shows the physical wire on stdout
 */
void run_visualizer(char* data, int datalen) {
    // vis_settings is set in the REPL when sending a packet of data to the listener 
    short bit_array[BIT_ARR_SIZE]; 
    short bit_array_modified[BIT_ARR_SIZE_BLOCK]; 
    decode_characters(data, bit_array); 

    switch (vis_settings.encoding_type) {
        case NRZ:
            visualize_nrz(bit_array, datalen * CHAR_BIT, data, vis_settings.pace);
            break;
        case NRZ_I:
            visualize_nrzi(bit_array, datalen * CHAR_BIT, data, vis_settings.pace);
            break;
        case MANCHESTER:
            visualize_manchester(bit_array, datalen * CHAR_BIT, data, vis_settings.pace);
            break;
        case BLOCK_4B5B: 
            decode_block(bit_array, bit_array_modified, datalen * CHAR_BIT); 
            visualize_block(bit_array_modified, (datalen * CHAR_BIT * 5)/4, data, vis_settings.pace);
            break; 
    }
}


/*
 * close the udp port once the listener thread terminates
 */
void listener_cleanup(void *args) {
    if (close(*((uint16_t *)args)) == -1) {
        perror("close");
    }
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
             sizeof(servaddr)) < 0)
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

        // recvfrom errors
        if (bytes_rec == -1) {
            break;
        }

        // fprintf(stderr, "received packet!\n");
       
        // null terminate the string 
        buffer[bytes_rec] = '\0'; 
        run_visualizer(buffer, bytes_rec); 
        // fwrite(buffer, bytes_rec, 1, stdout);
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

///*
// * print a prompt for the repl
// */
//void print_prompt() {
//    fprintf(stdout, "> ");
//
//    if (fflush(stdout) != 0) {
//        perror("fflush");
//        exit(0);
//    }
//}

/*
 * start listening for commands
 */
void start_repl() {

    char server_command[BUFLEN];
    while (fgets(server_command, BUFLEN, stdin)) {

        // continue if just newline
        if (server_command[0] == '\n') {
            continue;
        }

        // get the command name (the first word of the line)
        char saved[BUFLEN];
        memset(saved, '\0', BUFLEN); 
        strcpy(saved, server_command); 
        char *command = strtok(server_command, " \t\n");

//        // get the rest of the line
        char *rest = strtok(NULL, " ");

        // COMMANDS TO CHANGE ENCODING TYPE
        if (!strcmp(command, "nrz")) {
            vis_settings.encoding_type = NRZ;
            fprintf(stdout, "encoding protocol set to NRZ.\n");
        } else if (!strcmp(command, "nrz_i")) {
            vis_settings.encoding_type = NRZ_I;
            fprintf(stdout, "encoding protocol set to NRZ-inverted.\n");
        } else if (!strcmp(command, "manchester")) {
            vis_settings.encoding_type = MANCHESTER;
            fprintf(stdout, "encoding protocol set to Manchester.\n");
        } else if (!strcmp(command, "block")) {
            vis_settings.encoding_type = BLOCK_4B5B;
            fprintf(stdout, "encoding protocol set to 4B/5B block encoding.\n");
        } 
        
        // PING COMMANDS
        else if (!strcmp(command, "ping")) {
            // fprintf(stderr, "sending test packet!\n");
            if (!rest) {
                fprintf(stderr, "Invalid number of arguments for ping: <ping> <string>\n"); 
            }
            char* fix_this = saved; 
            fix_this += strlen("ping"); 
            while (*fix_this == ' ') {
                fix_this++;
            }
            if (ping_self(&ping_config, fix_this) == -1) {
                fprintf(stderr, "ping failed.\n");
            }
        } else if (!strcmp(command, "ping_random")) {
            if (!rest) {
                fprintf(stderr, "Invalid number of arguments for ping_random: <ping_random> <num_bytes>\n"); 
            }
            int num_bytes = atoi(rest) + 1; 

            char buf[num_bytes+1];
            memset(buf, '\0', num_bytes+1); 

            for (int i = 0; i < num_bytes; i++) {
                buf[i] = (rand() % 95) + 32; 
            }
            buf[num_bytes-1] = '\n'; 

            if (ping_self(&ping_config, buf) == -1) {
                fprintf(stderr, "ping failed.\n"); 
            }
        } else if (!strcmp(command, "pace")) {
            float prev_pace = vis_settings.pace;
            float next_pace = atof(rest);

            if (next_pace > 0.0 && next_pace <= 1.0) {
                fprintf(stdout, "pace set to %f, was %f.\n", next_pace, prev_pace);
                vis_settings.pace = next_pace;
            } else {
                fprintf(stderr, "invalid pace value.\n");
            }
        } else {
            fprintf(stderr, "command not supported.\n");
        }
    }
}


int main(int argc, char **argv) {
    srand(time(NULL)); 

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <listener_port>\n", argv[0]);
        exit(1);
    }

    fprintf(stdout, "Welcome to the network encoding visualizer. Here are a list of supported commands:\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "CHANGE ENCODING:\n");
    fprintf(stdout, "---------------------\n");
    fprintf(stdout, "nrz -> change the visualization encoding to NRZ (non-return to zero).\n");
    fprintf(stdout, "nrz_i -> change the visualization encoding to NRZ Inverted.\n");
    fprintf(stdout, "manchester -> change the visualization encoding to Manchester.\n");
    fprintf(stdout, "block -> change the visualization encoding to 4B/5B block.\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "VISUALIZE DATA:\n");
    fprintf(stdout, "---------------------\n");
    fprintf(stdout, "ping <data string> -> output `<data string>` as a signal visualization to stdout.\n");
    fprintf(stdout, "ping_random <num_bytes> -> output <num_bytes> random characters as a signal visualization.\n");
    fprintf(stdout, "pace (0.0, 1.0] -> set the signal visualizer speed.\n");
    fprintf(stdout, "\n");
    int err;

    char* listener_port = argv[1];
    uint16_t udp_port = atoi(listener_port);

    if ((err = pthread_create(&vis_settings.vis_thread, NULL, run_visualizer_listener_thread, (void *)&udp_port)) != 0) {
        fprintf(stderr, "pthread create failed, err: %d\n", err);
        exit(1);
    }

    // setup the sender udp port
    ping_config.udp_port = udp_port;
    setup_sender(&ping_config);

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