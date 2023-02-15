#include "echo.h"

#include "unix_wrap.h"
#include "network.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <iostream>

#include "squeue.hpp"

void echo_client(char *hostname, char *port){
    rio_t rio;
    int clientfd, n;
    char buf[ECHO_MAXLINE];

    clientfd = Open_clientfd(hostname, port);
    rio_readinitb(&rio, STDIN_FILENO);

    while((n = rio_readlineb(&rio, buf, ECHO_MAXLINE)) != 0){
        rio_writen(clientfd, buf, strlen(buf));
    }

    Close(clientfd);
}

/* Serve for one line */
int serve(int connfd, rio_t &rio, char *buf)
{
    if (rio_readlineb(&rio, buf, ECHO_MAXLINE) != 0){
        std::cout << "From client: " << buf;
        return SERVE_SUCESS;
    }
    else{   /* EOF */
        return SERVE_FAILED;
    }
}

/* Echo Server powered by multi-process */
void echo_mulproc(char *port)
{
    
}

/* Echo Server powered by prethreding */
void *thread(void *);
void create_threads(pthread_t *tid, void *args);

void echo_prethreading(char *port)
{
    int listenfd, connfd;
    struct sockaddr_storage clientaddr;
    socklen_t clientlen;

    SyncQueue<int> fd_pool("fd_pool", MAXTHREAD);

    pthread_t tid[MAXTHREAD];
    create_threads(tid, &fd_pool);

    listenfd = Open_listenfd(port);
    while(1){
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        fd_pool.enqueue(connfd);
    }

    Close(listenfd);
    exit(0);
}

void create_threads(pthread_t *tid, void *args)
{
    for (int i = 0; i < MAXTHREAD; ++i)
        Pthread_create(tid + i, nullptr, thread, args);
}

void *thread(void *args)
{
    Pthread_detach(pthread_self());

    int connfd;
    rio_t rio;
    SyncQueue<int> *fd_pool = (SyncQueue<int> *) args;
    char buf[ECHO_MAXLINE];
    
    while(1){
        connfd = fd_pool->dequeue();
        rio_readinitb(&rio, connfd);
        while(serve(connfd, rio, buf) == SERVE_SUCESS);
        Close(connfd);
    }

    return nullptr;
}

/* Echo Server powered by I/O multiplexing */
void echo_select(char *port)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    static fd_pool pool;

    listenfd = Open_listenfd(port);
    init_pool(listenfd, pool);

    while (1){
        /* Wait for listening/connected descriptor(s) to become ready*/
        pool.ready_set = pool.read_set;
        pool.nready = Select(pool.maxfd + 1, &pool.ready_set);

        /* If listening descriptor ready, add new client to pool*/
        if(FD_ISSET(listenfd, &pool.ready_set)){
            clientlen = sizeof(struct sockaddr_storage);
            connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
            add_client(connfd, pool);
        }

        /* Echo a text line from each ready connected descriptor */
        check_clients(pool);
    }
}

void init_pool(const int listenfd, fd_pool &pool)
{
    pool.maxfd = listenfd;
    FD_ZERO(&pool.read_set);
    FD_SET(listenfd, &pool.read_set);
    FD_ZERO(&pool.ready_set);
    pool.maxi = 0;
    for (int i = 0; i < FD_SETSIZE; ++i)
        pool.clientfd[i] = -1;
}

void check_clients(fd_pool &pool)
{
    int connfd;
    ssize_t n;
    char buf[ECHO_MAXLINE];
    rio_t *rp;

    for (int i = 0; (i <= pool.maxi) && (pool.nready > 0); ++i){
        connfd = pool.clientfd[i];
        if((connfd == -1) || !(FD_ISSET(connfd, &pool.ready_set)))
            continue;

        pool.nready--;
        if (serve(connfd, pool.clientrio[i], buf) == SERVE_FAILED){ /* disconnect */
            Close(connfd);
            FD_CLR(connfd, &pool.read_set);
            pool.clientfd[i] = -1;
        }
    }
}

void add_client(const int connfd, fd_pool &pool)
{
    /* Find a slot for new connection */
    int i = 0;
    pool.nready--;
    for (; i < FD_SETSIZE; ++i){
        if (pool.clientfd[i] == -1) break;
    }

    if(i == FD_SETSIZE){
        Close(connfd);
        return;
    }

    FD_SET(connfd, &pool.read_set);
    pool.clientfd[i] = connfd;
    rio_readinitb(&pool.clientrio[i], connfd);

    if (connfd > pool.maxfd)
        pool.maxfd = connfd;
    
    if (i > pool.maxi)
        pool.maxi = i;
}