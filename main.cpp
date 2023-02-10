//*/
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string>
#include<iostream>
#include<cstring>

#include "common/catch_amalgamated.hpp"
#include "common/common.h"

long Stoul(const std::string &__str, size_t *__idx = (size_t *)0, int __base = 10)
{
    try{
        return std::stoul(__str, __idx, __base);
    }
    catch (std::invalid_argument const &ex){
        std::cerr << "invalid argument" <<  std::endl;
        exit(-1);
    }
    catch (std::out_of_range const &ex){
        std::cerr << "out of interger range" <<  std::endl;
        exit(-1);
    }
}

const char* Inet_ntop(int __af, const void *__src, char *__dst, socklen_t __size){
    const char *rc = NULL;
    if(rc = inet_ntop(__af, __src, __dst, __size)){
        return rc;
    }
    else{
        std::cerr << "invalid argument" << std::endl;
        exit(-1);
    }
}

const char* hex2dd(const char *__str){
    unsigned int uint = Stoul(__str, nullptr, 16);
    uint = htonl(uint);
    char ip[16];
    return Inet_ntop(AF_INET, &uint, ip, 16);
}

TEST_CASE("Unit Test", "[Test]"){
    CHECK ( std::strcmp(hex2dd("0x0"), "0.0.0.0")  == 0 );
    CHECK ( std::strcmp(hex2dd("0xffffffff"), "255.255.255.255")  == 0 );
    CHECK ( std::strcmp(hex2dd("0x7f000001"), "127.0.0.1")  == 0 );
    CHECK ( std::strcmp(hex2dd("0xcdbca079"), "205.188.160.121")  == 0 );
    CHECK ( std::strcmp(hex2dd("0x400c950d"), "64.12.149.13")  == 0 );
    CHECK ( std::strcmp(hex2dd("0xcdbc9217"), "205.188.146.23")  == 0 );
}
//*/