#include "rio.h"

#include<sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>

ssize_t rio_readn(int fd, void *usrbuf, size_t n){
    size_t nleft = n;
    size_t nread;
    char *bufp = (char *)usrbuf;

    while(nleft > 0){
        if ((nread = read(fd, bufp, nleft)) < 0){
            return -1;
        }
        else if (nread == 0){
            break;
        }

        nleft -= nread;
        bufp += nread;
    }

    return (n - nread);
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n){
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = (char *)usrbuf;

    while(nleft > 0){
        if ((nwritten = write(fd, usrbuf, nleft) <= 0)){
            return -1;
        }

        nleft -= nwritten;
        bufp += nwritten;
    }
    
    return n;
}

void rio_readinitb(rio_t *rp, int fd){
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

ssize_t rio_read(rio_t *rp, void *usrbuf, size_t n){
    size_t cnt;

    /* Read bytes from file if buffer is empty */
    if (rp->rio_cnt <= 0){
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));

        if (rp->rio_cnt < 0){
            return -1;              /* System call error */
        }
        else if (rp->rio_cnt == 0){
            return 0;               /* EOF */
        }
        
        rp->rio_bufptr = rp->rio_buf;
    }

    /* Copy min(n, rp->rio_cnt) bytes from internal buffer to user buf */
    cnt = rp->rio_cnt < n ? rp->rio_cnt : n;
    memcpy(usrbuf, rp->rio_bufptr, cnt);
    rp->rio_cnt -= cnt;
    rp->rio_bufptr += cnt;
    return cnt;
}

ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen){
    size_t n, read_cnt;
    char c, *bufp = (char *)usrbuf;

    for (n = 1; n < maxlen; n++){
        if ((read_cnt = rio_read(rp, &c, 1)) == 1){
            *bufp++ = c;
            if(c == '\n'){
                n++;
                break;
            }

        }
        else if (read_cnt == 0){
            break;
        }
        else{
            return -1;
        }
    }
    *bufp = '\0';
    return n - 1;
}

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n){
    size_t nleft = n;
    size_t nread;
    char *bufp = (char *)usrbuf;

    while(nleft > 0){
        if ((nread = rio_read(rp, bufp, nleft)) < 0){
            return -1;
        }
        else if (nread == 0){
            break;
        }
        nleft -= nread;
        bufp += nread;
    }

    return (n - nleft);
}