CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(CASPP)

SET(CMAKE_CXX_STANDARD 17)

SET(SRC_LIST 
    ./main.cpp 
    ./common/catch_amalgamated.cpp 
    ./common/rio.cpp
    ./common/network.cpp
    ./common/unix_wrap.cpp
)
SET(OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

ADD_EXECUTABLE(main ${SRC_LIST})