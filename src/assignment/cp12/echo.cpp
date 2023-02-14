#include "echo.h"

#include "unix_wrap.h"
#include "network.h"
#include <stdio.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

void echo_client(char *hostname, char *port){
    rio_t rio, rio_in;
    int clientfd, n;
    char buf[ECHO_MAXLINE], buf_in[ECHO_MAXLINE];

    clientfd = Open_clientfd(hostname, port);
    rio_readinitb(&rio_in, STDIN_FILENO);
    rio_readinitb(&rio, clientfd);

    if((n = rio_readlineb(&rio_in, buf_in, ECHO_MAXLINE)) != 0){
        rio_writen(clientfd, buf_in, strlen(buf_in));
    }
    else{ /* EOF, close connection */
        Close(clientfd);
        exit(0);
    }
    
    if((n = rio_readlineb(&rio, buf, ECHO_MAXLINE)) != 0){
        std::cout << "From: Server: " << buf;
    }

    Close(clientfd);
}

void echo_server(char *port)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    static fd_pool pool;

    listenfd = Open_listenfd(port);
    init_pool(listenfd, &pool);

    while (1){
        /* Wait for listening/connected descriptor(s) to become ready*/
        pool.ready_set = pool.read_set;
        pool.nready = Select(pool.maxfd + 1, &pool.ready_set);

        /* If listening descriptor ready, add new client to pool*/
        if(FD_ISSET(listenfd, &pool.ready_set)){
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
            add_client(connfd, &pool);
        }

        /* Echo a text line from each ready connected descriptor */
        check_client(&pool);
    }
}

void init_pool(int listenfd, fd_pool *poolp)
{
    poolp->maxfd = listenfd;
    FD_ZERO(&poolp->read_set);
    FD_SET(listenfd, &poolp->read_set);
    FD_ZERO(&poolp->ready_set);
    poolp->maxi = 0;
    for (int i = 0; i < FD_SETSIZE; ++i)
        poolp->clientfd[i] = -1;
}

void check_client(fd_pool *poolp)
{
    int connfd;
    ssize_t n;
    char buf[ECHO_MAXLINE];
    rio_t *rp;

    for (int i = 0; (i <= poolp->maxi) && (poolp->nready > 0); ++i){
        connfd = poolp->clientfd[i];
        if((connfd == -1) || !(FD_ISSET(connfd, &poolp->ready_set)))
            continue;

        poolp->nready--;
        rp = &poolp->clientrio[i];
        if((n = rio_readlineb(rp, buf, ECHO_MAXLINE)) != 0){
            std::cout << "From client: " << buf;
            rio_writen(connfd, buf, strlen(buf));
        }
        else{  /* EOF, close and remove descriptor from pool */
            Close(connfd);
            FD_CLR(connfd, &poolp->read_set);
            poolp->clientfd[i] = -1;
        }

    }
}

void add_client(int connfd, fd_pool *poolp)
{
    /* Find a slot for new connection */
    int i = 0;
    poolp->nready--;
    for (; i < FD_SETSIZE; ++i){
        if (poolp->clientfd[i] == -1) break;
    }

    if(i == FD_SETSIZE){
        Close(connfd);
        return;
    }

    FD_SET(connfd, &poolp->read_set);
    poolp->clientfd[i] = connfd;
    rio_readinitb(&poolp->clientrio[i], connfd);

    if (connfd > poolp->maxfd)
        poolp->maxfd = connfd;
    
    if (i > poolp->maxi)
        poolp->maxi = i;
}