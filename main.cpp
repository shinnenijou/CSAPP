#include "tiny.h"

#include <unistd.h>
#include <iostream>

int main(int argc, char **argv){
    if (argc < 2){
        std::cerr << "usage: " << argv[0] << " <port>" << std::endl;
        exit(1); 
    }

    run(argv[1]);
    exit(0);
}