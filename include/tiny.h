#ifndef TINY_H
#define TINY_H

#include <sys/types.h>

#include "rio.h"

#define TINY_MAXLINE 1024
#define TINY_MAXBUF 4096
#define TINY_ROOT "../res/www"

void doit(int fd);
void read_requesthdrs(rio_t *p);
int parse_uri(char *uri, char *filename, char *cgiargs);
void serve_static(int fd, char *filename, int filesize);
void get_filetype(char *filename, char *filetype);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clienterror(int fd, char *cause, char *errnum,
                 char *shortmsg, char *longmsg);
void run(char *port);

#endif