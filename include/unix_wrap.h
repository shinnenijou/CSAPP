#ifndef UNIX_WRAP_H
#define UNIX_WRAP_H

#include <sys/socket.h>

void socket_error(const char *msg, int error_code);
void unix_error(const char *msg);

// UNIX Syetem call
int Open(const char *path, const int flags);
int Close(int fd);

// UNIX Socket
int Getaddrinfo(const char *host, const char *port, 
                const struct addrinfo *hints, struct addrinfo **dst);
int Setsockopt(int sockfd, int level, int opt, const void * optval, socklen_t plen);

#endif