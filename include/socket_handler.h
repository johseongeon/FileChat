#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

// ========== server function  ==========

/**
 * initialize server socket
 * socket_path: Unix 소켓 경로
 * return server file descriptor (-1 on failure)
 */
int init_server_socket(const char *socket_path);

/**
 * server_fd: 서버 file descriptor
 * return client file descriptor (-1 on failure)
 */
int accept_client(int server_fd);

/**
 * add client to client management list
 * fd: client fd
 * username: client username
 */
void add_client(int fd, const char *username);

/**
 * remove client from client management list
 * fd: client fd
 */
void remove_client(int fd);

/**
 * find username by client fd
 * return username (NULL on failure)
 */
const char* find_client_username(int fd);

/**
 * recieve username from client
 * return username (NULL on failure)
 */
char* receive_username(int client_fd);

// ========== client function  ==========

/**
 * socket_path: Unix 소켓 경로
 * return socket file descriptor (-1 on failure)
 */
int connect_to_server(const char *socket_path);

/**
 * return 0 on successs
 * return -1 on failure
 */
int send_username(int sock_fd, const char *username);

#endif
