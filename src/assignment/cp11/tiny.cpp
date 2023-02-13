#include "tiny.h"

#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

#include "unix_wrap.h"
#include "network.h"
void run(char *port)
{
    int listenfd, connfd;
    char hostname[TINY_MAXLINE], client_port[TINY_MAXLINE];
    socklen_t clientlen;
    struct sockaddr clientaddr;

    listenfd = Open_listenfd(port);
    while (1)
    {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        Getnameinfo((struct sockaddr *)&clientaddr, clientlen, hostname, 
                    TINY_MAXLINE, client_port, TINY_MAXLINE, 0);
        std::cout << "Accepted connectionn from (" 
                  << hostname << ", " << client_port << ")" << std::endl;
        doit(connfd);
        Close(connfd);
    }
}

void doit(int fd)
{
    int is_static;
    struct stat sbuf;
    char buf[TINY_MAXLINE], method[TINY_MAXLINE], uri[TINY_MAXLINE];
    char version[TINY_MAXLINE], filename[TINY_MAXLINE], cgiargs[TINY_MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    rio_readinitb(&rio, fd);
    rio_readlineb(&rio, buf, TINY_MAXLINE);
    printf("Request headers:\n");
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);
    if(strcasecmp(method, "GET")){
        clienterror(fd, method, "501", "Not implemented",
                    "Tiny does not implement this method");
        return;
    }
    read_requesthdrs(&rio);

    /* Parse URI from GET request */
    is_static = parse_uri(uri, filename, cgiargs);
    if(stat(filename, &sbuf) < 0){
        clienterror(fd, filename, "404", "Not found",
                    "Tiny couldn't find this file");
        return;
    }

    if(is_static){  /* Serve static content */
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)){
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't read the file");
            return;
        }
        serve_static(fd, filename, sbuf.st_size);
    }
    else{  /* Serve dynamic content */
        if(!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)){
            clienterror(fd, filename, "403", "Forbidden",
                        "Tiny couldn't run the CGI program");
            return;
        }
        serve_dynamic(fd, filename, cgiargs);
    }
}

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
    char buf[TINY_MAXLINE], body[TINY_MAXBUF];

    /* Build the HTTP response body */
    sprintf(body, "<html><title>Tiny Error</title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web server</em>\r\n", body);

    /* Print the HTTP response */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", (int)strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}

void read_requesthdrs(rio_t *rp)
{
    char buf[TINY_MAXLINE];

    rio_readlineb(rp, buf, TINY_MAXLINE);
    while(strcmp(buf, "\r\n")){
        rio_readlineb(rp, buf, TINY_MAXLINE);
        printf("%s", buf);
    }
    return;
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    if(!strstr(uri, "cgi-bin")) {   /* Static content */
        strcpy(cgiargs, "");
        strcpy(filename, TINY_ROOT);
        strcat(filename, uri);
        if (uri[strlen(uri) - 1] == '/'){
            strcat(filename, "index.html");
        }
        return 1;
    }
    else {  /* Dynamic content */
        ptr = index(uri, '?');
        if(ptr){
            strcpy(cgiargs, ptr+1);
            *ptr = '\0';
        }
        else
            strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}

void serve_static(int fd, char *filename, int filesize)
{
    int srcfd;
    char *srcp, filetype[TINY_MAXLINE], buf[TINY_MAXBUF];

    /* Send response headers to client */
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
    sprintf(buf, "%sConnection: close\r\n", buf);
    sprintf(buf, "%sContent-length: %d\r\n", buf, filesize);
    sprintf(buf, "%sContent-type: %s\r\n\r\n", buf, filetype);
    rio_writen(fd, buf, strlen(buf));
    printf("Response headers: \n");
    printf("%s", buf);

    /* Send response body to client */
    srcfd = Open(filename, O_RDONLY);
    srcp = (char *)Mmap(nullptr, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    Close(srcfd);
    rio_writen(fd, srcp, filesize);
    Munmap(srcp, filesize);
}

void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html"))
        strcpy(filetype, "text/html");
    else if (strstr(filename, ".gif"))
        strcpy(filetype, "image/gif");
    else if (strstr(filename, ".png"))
        strcpy(filetype, "image/png");
    else if (strstr(filename, ".jpg"))
        strcpy(filetype, "image/jpeg");
    else
        strcpy(filetype, "text/plain");
}

void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[TINY_MAXLINE], *emptylist[] = {nullptr};
    extern char ** environ;

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    rio_writen(fd, buf, strlen(buf));

    if(Fork() == 0){  /* Child */
        setenv("QUERY_STRING", cgiargs, 1);
        Dup2(fd, STDOUT_FILENO);
        Execve(filename, emptylist, environ);
    }
    Wait(nullptr);
}