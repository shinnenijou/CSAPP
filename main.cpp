#include "tiny_server.h"

#include <unistd.h>
#include <iostream>

int main(int argc, char **argv){
    char *port;

    if (argc < 2){
        std::cerr << "usage: " << argv[0] << " <port>" << std::endl;
        std::cout << "use default port 80" << std::endl;
        port = "80";
    }
    else{
        port = argv[1];
    }

    run(port);
    
    exit(0);
}