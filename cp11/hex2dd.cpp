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
    char *hex = argv[1] + 2;
    size_t len = strlen(hex);
    unsigned int unit = 0, power = 1;
    for(int i = len - 1; i != -1; i--){
        unit += (hex[i]>='a'?hex[i]-'a'+10:hex[i]-'0') * power;
        power *= 16;
    }
    unit = htonl(unit);
    char ip[16];
    if(inet_ntop(AF_INET, &unit, ip, 16)){
        fprintf(stdout, "%s\n", ip);
        exit(0);
    }
    else{
        fprintf(stderr, "arguments invalid.\n");
        exit(-1);
    }
}