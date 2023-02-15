#ifndef ECHO_H
#define ECHO_H

#include "unix_wrap.h"
#include "rio.h"

#define ECHO_MAXLINE 128
#define MAXTHREAD 64
#define SERVE_FAILED -1
#define SERVE_SUCESS 0

typedef struct {  /* Represents a pool of connected descriptors */
    int maxfd;          /* Largest descriptor in read_set */
    fd_set read_set;    /* Set of all active descriptor */
    fd_set ready_set;   /* Subset of descriptors ready for reading */
    int nready;         /* Number of ready descriptors from select */
    int maxi;           /* High water index into client array(1st unused) */
    int clientfd[FD_SETSIZE];       /* Set of active descriptors */
    rio_t clientrio[FD_SETSIZE];    /* Set of active read buffers */
} fd_pool;

void init_pool(const int listenfd, fd_pool &poolp);
void add_client(const int connfd, fd_pool &poolp);
void check_clients(fd_pool &poolp);

void echo_client(char *hostname, char *port);

int serve(int connfd, rio_t &rp, char *buf);
void echo_select(char *port);
void echo_mulproc(char *port);
void echo_prethreading(char *port);

#endif