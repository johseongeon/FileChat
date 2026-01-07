#include "common.h"
#include "inotify_handler.h"
#include "socket_handler.h"
#include "message.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/inotify.h>
#include <errno.h>
#include <pthread.h>

// new client connection
void *handle_client_connection(void *arg) {
    int client_fd = *(int *)arg;
    free(arg);
    
    char *username = receive_username(client_fd);
    if (!username) {
        close(client_fd);
        return NULL;
    }

    add_client(client_fd, username);
    
    // connection message broadcast
    char msg[MAX_MESSAGE_LEN];
    format_connect_message(msg, sizeof(msg), username);
    printf("%s\n", msg);
    broadcast_message(msg, username);
    
    free(username);
    return NULL;
}

void *socket_listener(void *arg) {
    int server_fd = init_server_socket(SOCKET_PATH);
    if (server_fd == -1) {
        fprintf(stderr, "Failed to initialize server socket\n");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on %s\n", SOCKET_PATH);
    
    while (1) {
        int *client_fd = malloc(sizeof(int));
        *client_fd = accept_client(server_fd);
        
        if (*client_fd == -1) {
            free(client_fd);
            continue;
        }
        
        pthread_t thread;
        pthread_create(&thread, NULL, handle_client_connection, client_fd);
        pthread_detach(thread);
    }
    
    return NULL;
}

void handle_file_modify(const char *username) {
    char filepath[MAX_FILEPATH_LEN];
    snprintf(filepath, sizeof(filepath), "%s/%s.txt", CLIENTS_DIR, username);
    
    // 파일에서 마지막 메시지 읽기
    char *last_message = read_last_message(filepath);
    if (last_message && strlen(last_message) > 0) {
        char msg[MAX_MESSAGE_LEN];
        format_user_message(msg, sizeof(msg), username, last_message);
        printf("%s\n", msg);
        broadcast_message(msg, username);
        free(last_message);
    }
}

void handle_file_delete(const char *username) {
    char msg[MAX_MESSAGE_LEN];
    format_disconnect_message(msg, sizeof(msg), username);
    printf("%s\n", msg);
    broadcast_message(msg, NULL);
}


int main() {
    if (mkdir(CHAT_DIR, 0755) == -1 && errno != EEXIST) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }
    
    if (mkdir(CLIENTS_DIR, 0755) == -1 && errno != EEXIST) {
        perror("mkdir");
        exit(EXIT_FAILURE);
    }
    
    // initialize inotify instance
    int inotify_fd = init_inotify(CLIENTS_DIR);
    if (inotify_fd == -1) {
        exit(EXIT_FAILURE);
    }
    
    printf("Server started. Watching %s\n", CLIENTS_DIR);
    
    pthread_t socket_thread;
    pthread_create(&socket_thread, NULL, socket_listener, NULL);
    
    // handle inotify events
    char buffer[INOTIFY_BUF_LEN];
    
    while (1) {
        int length = read(inotify_fd, buffer, INOTIFY_BUF_LEN);
        if (length < 0) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        
        // reat event from buffer
        int i = 0;
        while (i < length) {
            struct inotify_event *event = (struct inotify_event *)&buffer[i];
            
            if (event->len) {
                char username[MAX_USERNAME_LEN];
                strncpy(username, event->name, sizeof(username) - 1);
                username[sizeof(username) - 1] = '\0';
                
		// username.txt to username
                strip_txt_extension(username);
                
                if (event->mask & IN_MODIFY) {
                    // IN_MODIFY -> broadcast message
                    handle_file_modify(username);
                } else if (event->mask & IN_DELETE) {
                    // IN_DELETE -> client disconnect
                    handle_file_delete(username);
                }
                // IN_CREATE는 소켓 연결에서 처리됨
            }
            
            i += EVENT_SIZE + event->len;
        }
    }
    
    close(inotify_fd);
    return 0;
}
