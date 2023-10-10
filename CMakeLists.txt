CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

PROJECT(CASPP)

SET(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
SET(OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# Show ASM
SET(SHOW_ASM_SRC_LIST
    ./utils/show_asm.cpp
)

ADD_EXECUTABLE(show_asm ${SHOW_ASM_SRC_LIST})

# Tiny Server 
SET(TINY_SERVER_SRC_LIST 
    ./tiny_server.cpp 
    ./src/common/rio.cpp
    ./src/common/network.cpp
    ./src/common/unix_wrap.cpp
    ./src/assignment/cp11/tiny.cpp
    ./src/assignment/cp12/echo.cpp
    ./utils/show_bytes.cpp
)

ADD_EXECUTABLE(tiny_server ${TINY_SERVER_SRC_LIST})

# Homework Test
SET(HOMEWORK_SRC_LIST 
    ./src/assignment/ch2/ch2_hw.cpp
    ./unittest/unittest.cpp
)

ADD_EXECUTABLE(hw_test ${HOMEWORK_SRC_LIST})