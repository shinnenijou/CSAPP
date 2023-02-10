#include "network.h"

int Close(int fd){
    if (close(fd) != 0){
        std::cerr << "[Close]: " << strerror(errno) << std::endl;
        exit(0);
    }
    return 0;
}

int Setsockopt(int sockfd, int level, int opt, const void * optval, socklen_t plen){
    int rc;
    if((rc = setsockopt(sockfd, level, opt, optval, plen)) != 0){
        std::cerr << "[Setsockopt]: " << gai_strerror(rc) << std::endl;
        exit(-1);
    }
    return 0;
}

int Getaddrinfo(const char *host, const char *port, const struct addrinfo *hints, struct addrinfo ** dst)
{
    int rc;
    if ((rc = getaddrinfo(host, port, hints, dst)) != 0)
    {
        std::cerr << "[Getaddrinfo]: " << gai_strerror(rc) << std::endl;
        exit(-1);
    }

    return 0;
}

int open_clientfd(char *hostname, char *port)
{
    int clientfd;
    struct addrinfo *p, *listp, hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_NUMERICSERV;
    hints.ai_flags |= AI_ADDRCONFIG;
    Getaddrinfo(hostname, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next)
    {
        if ((clientfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        {
            continue;
        }

        if (connect(clientfd, p->ai_addr, p->ai_addrlen) == 0)
        {
            break;
        }

        Close(clientfd);
    }

    freeaddrinfo(listp);

    if (!p)
    {
        std::cerr << "[open_clientfd]: failed connection to " 
                  << hostname << ":" << port << std::endl;
        return -1;
    }

    return clientfd;
}

int open_listenfd(char *port)
{
    int listenfd, optval = 1;
    struct addrinfo *p, *listp, hints;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;
    hints.ai_flags |= AI_NUMERICSERV;
    Getaddrinfo(nullptr, port, &hints, &listp);

    for (p = listp; p; p = p->ai_next){
        if ((listenfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0)
        {
            continue;
        }

        setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));

        if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0){
            break;
        }

        Close(listenfd);
    }

    freeaddrinfo(listp);

    if(!p){
        std::cerr << "[open_listenfd]: failed binding to " << port << std::endl;
        return -1;
    }

    return listenfd;
}