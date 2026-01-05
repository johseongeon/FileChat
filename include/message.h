#ifndef MESSAGE_H
#define MESSAGE_H

#include <stddef.h>

/**
 * broadcast message to every clients
 * exclude_user: commonly, sender
 */
void broadcast_message(const char *message, const char *exclude_user);

/**
 * return 0 on success
 * -1 on failure
 */
int send_message(int sock_fd, const char *message);

/**
 * recieve message from socket
 * return recieved message (NULL on failure)
 */
char* receive_message(int sock_fd);

/**
 * formatting message : "username: content"
 * size: buffer size
 */
void format_user_message(char *buffer, size_t size, 
                         const char *username, const char *content);

/**
 * formatting connect message: "username is connected."
 * size: buffer size
 */
void format_connect_message(char *buffer, size_t size, const char *username);

/**
 * formatting disconnect message : "username is disconnected."
 * size: buffer size
 */
void format_disconnect_message(char *buffer, size_t size, const char *username);

#endif
