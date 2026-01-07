#include "common.h"
#include "socket_handler.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

// global vars
char username[MAX_USERNAME_LEN];
char filepath[MAX_FILEPATH_LEN];
int sock_fd;

// cleanup when terminated
void cleanup() {
    close(sock_fd);
    unlink(filepath);
    printf("\nDisconnected.\n");
}

// interrupt(or termination) signal handler
void signal_handler(int sig) {
    cleanup();
    exit(0);
}

void *receive_messages(void *arg) {
    while (1) {
        // receive massage from server
        char *msg = receive_message(sock_fd);
        if (!msg) {
            printf("Connection lost.\n");
            cleanup();
            exit(1);
        }
        
        printf("%s\n", msg);
        fflush(stdout);
        
        free(msg);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    strncpy(username, argv[1], sizeof(username) - 1);
    username[sizeof(username) - 1] = '\0';
    
    // filepath generation
    snprintf(filepath, sizeof(filepath), "%s/%s.txt", CLIENTS_DIR, username);
    
    // create file
    FILE *fp = fopen(filepath, "w");
    if (!fp) {
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    
    sock_fd = connect_to_server(SOCKET_PATH);
    if (sock_fd == -1) {
        fprintf(stderr, "Make sure the server is running.\n");
        unlink(filepath);
        exit(EXIT_FAILURE);
    }
    
    if (send_username(sock_fd, username) == -1) {
        fprintf(stderr, "Failed to send username\n");
        close(sock_fd);
        unlink(filepath);
        exit(EXIT_FAILURE);
    }
    
    signal(SIGINT, signal_handler);
    
 
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, NULL);
    pthread_detach(recv_thread);
    
    printf("Connected as %s.\n", username);
    
    // main loop
    char message[MAX_MESSAGE_LEN];
    while (1) {
        // get user input
        if (fgets(message, sizeof(message), stdin) == NULL) {
            break;
        }
        
        // remove \n from message
        message[strcspn(message, "\n")] = '\0';
        
        if (strlen(message) == 0) {
            continue;
        }
        
        // write message to file
        fp = fopen(filepath, "a");
        if (fp) {
            fprintf(fp, "%s\n", message);
            fclose(fp);
        } else {
            perror("fopen");
        }
    }
    
    cleanup();
    return 0;
}
