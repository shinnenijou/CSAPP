#ifndef UNIX_WRAP_H
#define UNIX_WRAP_H

#include <sys/socket.h>

void socket_error(const char *msg, int error_code);
void unix_error(const char *msg);

// UNIX Syetem call
int Open(const char *path, const int flags);
int Close(int fd);
int Dup2(int oldfd, int newfd);

void *Mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int Munmap(void *addr, size_t length);

pid_t Fork();
pid_t Wait(int *statusp);
int Execve(const char *filename, char * const *argv, char * const *envp);


// UNIX Socket
int Accept(int listenfd, struct sockaddr *addr, socklen_t *addrlen);
int Getaddrinfo(const char *host, const char *port, 
                const struct addrinfo *hints, struct addrinfo **dst);
int Getnameinfo(const struct sockaddr *addr, socklen_t addrlen,
                char *host, size_t hostlen,
                char *port, size_t portlen, int flag);
int Setsockopt(int sockfd, int level, int opt, const void * optval, socklen_t plen);

#endif