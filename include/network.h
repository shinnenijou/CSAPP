#ifndef NETWORK_H
#define NETWORK_H

#define BACKLOG 1024

int open_clientfd(char *hostname, char *port);
int Open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);
int Open_listenfd(char *port);

#endif