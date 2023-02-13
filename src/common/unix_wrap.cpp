#include "unix_wrap.h"

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <iostream>
void socket_error(const char *msg, int error_code)
{
    std::cerr << "[Error]" << msg << ": " << gai_strerror(error_code) << std::endl;
    exit(0);
}

void unix_error(const char *msg)
{
    std::cerr << "[Error]" << msg << ": " << strerror(errno) << std::endl;
    exit(0);
}

int Open(const char *path, const int flags){
    int fd;
    if((fd = open(path, flags)) < 0)
        unix_error("Open error");
    return fd;
}

int Close(int fd){
    if (close(fd) != 0)
        unix_error("Close error");
    return 0;
}

int Dup2(int oldfd, int newfd)
{
    int rc;
    if((rc = dup2(oldfd, newfd)) == -1)
        unix_error("Dup2 error");
    return rc;
}

void *Mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset)
{
    void *rp;
    if ((rp = mmap(addr, length, prot, flags, fd, offset)) == MAP_FAILED)
        unix_error("Mmap error");
    return rp;
}

int Munmap(void *addr, size_t length)
{
    int rc;
    if ((rc = munmap(addr, length)) == -1)
        unix_error("Munmap error");
    return rc;
}

pid_t Fork()
{
    pid_t rc;
    if((rc = fork()) == -1)
        unix_error("Fork error");
    return rc;
}

pid_t Wait(int *statusp)
{
    pid_t rc;
    if ((rc = wait(statusp)) == -1)
        unix_error("Wait error");
    return rc;
}

int Execve(const char *filename, char * const *argv, char * const *envp)
{
    int rc;
    if((rc = execve(filename, argv, envp)) == -1)
        unix_error("Execve error");
    return rc;  /* In fact never return */
}

int Accept(int listenfd, struct sockaddr *addr, socklen_t *addrlen){
    int fd;
    if((fd = accept(listenfd, addr, addrlen)) < 0)
        unix_error("Accept error");
    return fd;
}

int Getaddrinfo(const char *host, const char *port, 
                const struct addrinfo *hints, struct addrinfo ** dst)
{
    int rc;
    if ((rc = getaddrinfo(host, port, hints, dst)) != 0) 
        socket_error("Getaddrinfo error", rc);
    return 0;
}

int Getnameinfo(const struct sockaddr *addr, socklen_t addrlen,
                char *host, size_t hostlen,
                char *port, size_t portlen, int flag)
{
    int rc;
    if ((rc = getnameinfo(addr, addrlen, host, hostlen, port, portlen, flag)) != 0) 
        socket_error("Getnameinfo error", rc);
    return 0;
}

int Setsockopt(int sockfd, int level, int opt, const void * optval, socklen_t plen){
    int rc;
    if((rc = setsockopt(sockfd, level, opt, optval, plen)) != 0)
        socket_error("Setsocketopt error", rc);
    return 0;
}