#ifndef NETWORK_H
#define NETWORK_H

#include<sys/socket.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<memory>
#include<iostream>
#include<unistd.h>
#include<errno.h>

#define BACKLOG 1024

int open_clientfd(char *hostname, char *port);
int open_listenfd(char *port);

#endif