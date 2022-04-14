#include<stdio.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char **argv){
    if (argc != 2){
        fprintf(stderr, "arguments number error.\n");
        exit(-1);
    }
    char *ip = argv[1];
    unsigned unit;
    if(inet_pton(AF_INET, ip, &unit)){
        unit = ntohl(unit);
        char hex[9] = {0};
        hex[8] = '\0';
        for (int i = 7; i != -1; i--, unit >>= 4){
            hex[i] = (unit & 0xf)>9?(unit & 0xf)-10+'a':(unit & 0xf) + '0';
        }
        printf("0x%s\n", hex);
        exit(0);
    }
    else{
        fprintf(stderr, "arguments invalid.\n");
        exit(-1);
    }
}