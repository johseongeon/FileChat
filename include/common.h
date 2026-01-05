#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>

// filepath
#define CHAT_DIR "/tmp/chat"
#define CLIENTS_DIR "/tmp/chat/clients"
#define SOCKET_PATH "/tmp/chat/server.sock"

// buffer size
#define MAX_CLIENTS 50
#define MAX_USERNAME_LEN 256
#define MAX_MESSAGE_LEN 1024
#define MAX_FILEPATH_LEN 512
#define EVENT_SIZE (sizeof(struct inotify_event))
#define INOTIFY_BUF_LEN (1024 * (EVENT_SIZE + 16))

// client socket struct
typedef struct {
    int fd;
    char username[MAX_USERNAME_LEN];
} ClientSocket;

// client management
// defined in "src/socket_handler.c"
extern ClientSocket clients[MAX_CLIENTS];
extern int client_count;
extern pthread_mutex_t clients_mutex;

#endif // COMMON_H
