#include "socket_handler.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

// client management
ClientSocket clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;


int init_server_socket(const char *socket_path) {
    int server_fd;
    struct sockaddr_un addr;

    // 기존 소켓 파일 제거 (있다면)
    unlink(socket_path);

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return -1;
    }

    // set addr struct
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    // bind addr to sock
    if (bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("bind");
        close(server_fd);
        return -1;
    }

    // start listening
    if (listen(server_fd, 10) == -1) {
        perror("listen");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

int accept_client(int server_fd) {
    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept");
    }
    return client_fd;
}

void add_client(int fd, const char *username) {
    pthread_mutex_lock(&clients_mutex);

    if (client_count < MAX_CLIENTS) {
        clients[client_count].fd = fd;
        strncpy(clients[client_count].username, username, MAX_USERNAME_LEN - 1);
        clients[client_count].username[MAX_USERNAME_LEN - 1] = '\0';
        client_count++;
    }

    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(int fd) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].fd == fd) {
            close(fd);
            // remove from array, shift other clients
            for (int j = i; j < client_count - 1; j++) {
                clients[j] = clients[j + 1];
            }
            client_count--;
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

const char* find_client_username(int fd) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < client_count; i++) {
        if (clients[i].fd == fd) {
            pthread_mutex_unlock(&clients_mutex);
            return clients[i].username;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
    return NULL;
}

char* receive_username(int client_fd) {
    int len;

    // inspect length of username
    if (read(client_fd, &len, sizeof(int)) <= 0) {
        return NULL;
    }

    if (len <= 0 || len >= MAX_USERNAME_LEN) {
        return NULL;
    }

    // malloc for username
    char *username = malloc(len + 1);
    if (!username) {
        return NULL;
    }

    // username
    if (read(client_fd, username, len) <= 0) {
        free(username);
        return NULL;
    }

    username[len] = '\0';
    return username;
}

int connect_to_server(const char *socket_path) {
    int sock_fd;
    struct sockaddr_un addr;

    sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("socket");
        return -1;
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, socket_path, sizeof(addr.sun_path) - 1);

    // connect to server
    if (connect(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("connect");
        close(sock_fd);
        return -1;
    }

    return sock_fd;
}

int send_username(int sock_fd, const char *username) {
    int len = strlen(username);

    // send length
    if (write(sock_fd, &len, sizeof(int)) <= 0) {
        return -1;
    }

    // send username
    if (write(sock_fd, username, len) <= 0) {
        return -1;
    }

    return 0;
}
