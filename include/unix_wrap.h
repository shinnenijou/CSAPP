#ifndef UNIX_WRAP_H
#define UNIX_WRAP_H

#include <sys/socket.h>
#include <signal.h>
#include <sys/select.h>
#include <pthread.h>
#include <semaphore.h>

void socket_error(const char *msg, int error_code);
void unix_error(const char *msg);

// UNIX File
int Open(const char *path, const int flags);
int Close(int fd);
int Dup2(int oldfd, int newfd);

// Unix Memory
void *Mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
int Munmap(void *addr, size_t length);
void *Malloc(size_t __size);
void *Calloc(size_t __count, size_t __size);

// Unix Process
pid_t Fork();
pid_t Wait(int *statusp);
int Execve(const char *filename, char * const *argv, char * const *envp);

// Unix SIGNAL
typedef void (*sighandler_t)(int);
sighandler_t Signal(int signum, sighandler_t handler);

// Unix I/O multiplexing
int Select(int n, fd_set *fdset);

// Unix Pthreads
typedef void* (thread_routine)(void *);
int Pthread_create(pthread_t *tid, pthread_attr_t *attr, thread_routine *f, void *arg);
int Pthread_cancel(pthread_t tid);
int Pthread_join(pthread_t tid, void **thread_return);
int Pthread_detach(pthread_t tid);
sem_t *Sem_open(const char *name, int flag, mode_t mode, unsigned int value);
int Sem_close(sem_t *s);
int Sem_unlink(const char *name);
void P(sem_t *s);
void V(sem_t *s);

// UNIX Socket
int Accept(int listenfd, struct sockaddr *addr, socklen_t *addrlen);
int Getaddrinfo(const char *host, const char *port, 
                const struct addrinfo *hints, struct addrinfo **dst);
int Getnameinfo(const struct sockaddr *addr, socklen_t addrlen,
                char *host, size_t hostlen,
                char *port, size_t portlen, int flag);
int Setsockopt(int sockfd, int level, int opt, const void * optval, socklen_t plen);

#endif