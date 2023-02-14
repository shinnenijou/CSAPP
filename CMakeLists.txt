CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(CASPP)

SET(CMAKE_CXX_STANDARD 17)

SET(SRC_LIST 
    ./main.cpp 
    #./src/common/catch_amalgamated.cpp 
    ./src/common/rio.cpp
    ./src/common/network.cpp
    ./src/common/unix_wrap.cpp
    ./src/assignment/cp11/tiny_server.cpp
    ./utils/show_bytes.cpp
)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

SET(OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

ADD_EXECUTABLE(main ${SRC_LIST})