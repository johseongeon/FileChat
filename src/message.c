#include "message.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void broadcast_message(const char *message, const char *exclude_user) {
    pthread_mutex_lock(&clients_mutex);
    
    for (int i = 0; i < client_count; i++) {
        if (exclude_user == NULL || strcmp(clients[i].username, exclude_user) != 0) {
            send_message(clients[i].fd, message);
        }
    }
    
    pthread_mutex_unlock(&clients_mutex);
}

int send_message(int sock_fd, const char *message) {
    int len = strlen(message);
    
    // send message length
    if (write(sock_fd, &len, sizeof(int)) <= 0) {
        return -1;
    }
    
    // send message content
    if (write(sock_fd, message, len) <= 0) {
        return -1;
    }
    
    return 0;
}

char* receive_message(int sock_fd) {
    int len;
    
    // receive message length
    int n = read(sock_fd, &len, sizeof(int));
    if (n <= 0) {
        return NULL;
    }
    
    // inspect len
    if (len <= 0 || len > MAX_MESSAGE_LEN * 10) {
        return NULL;
    }
    
    char *msg = malloc(len + 1);
    if (!msg) {
        return NULL;
    }
    
    // receive message content
    n = read(sock_fd, msg, len);
    if (n <= 0) {
        free(msg);
        return NULL;
    }
    
    msg[len] = '\0';
    return msg;
}

void format_user_message(char *buffer, size_t size, 
                         const char *username, const char *content) {
    snprintf(buffer, size, "%s: %s", username, content);
}

void format_connect_message(char *buffer, size_t size, const char *username) {
    snprintf(buffer, size, "%s is connected.", username);
}

void format_disconnect_message(char *buffer, size_t size, const char *username) {
    snprintf(buffer, size, "%s is disconnected.", username);
}
