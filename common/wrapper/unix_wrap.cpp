#include "../common.h"

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
    if (close(fd) != 0) unix_error("Close error");
    return 0;
}

int Getaddrinfo(const char *host, const char *port, 
                const struct addrinfo *hints, struct addrinfo ** dst)
{
    int rc;
    if ((rc = getaddrinfo(host, port, hints, dst)) != 0) 
        socket_error("Getaddrinfo error", rc);
    return 0;
}

int Setsockopt(int sockfd, int level, int opt, const void * optval, socklen_t plen){
    int rc;
    if((rc = setsockopt(sockfd, level, opt, optval, plen)) != 0)
        socket_error("Setsocketopt error", rc);
    return 0;
}