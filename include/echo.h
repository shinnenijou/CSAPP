#ifndef ECHO_H
#define ECHO_H

#include "unix_wrap.h"
#include "rio.h"

#define ECHO_MAXLINE 128

typedef struct {  /* Represents a pool of connected descriptors */
    int maxfd;          /* Largest descriptor in read_set */
    fd_set read_set;    /* Set of all active descriptor */
    fd_set ready_set;   /* Subset of descriptors ready for reading */
    int nready;         /* Number of ready descriptors from select */
    int maxi;           /* High water index into client array(1st unused) */
    int clientfd[FD_SETSIZE];       /* Set of active descriptors */
    rio_t clientrio[FD_SETSIZE];    /* Set of active read buffers */
} fd_pool;

void init_pool(int listenfd, fd_pool *poolp);
void add_client(int connfd, fd_pool *poolp);
void check_client(fd_pool *poolp);

void echo_select(char *port);
void echo_client(char *hostname, char *port);



#endif