cmake_minimum_required(VERSION 3.10)
project(wireword)

add_executable(wireword
    src/main.cpp
    src/logger.cpp
    src/ringbuf.cpp
    src/eventloop.cpp
    src/socket.cpp
    src/handlereg.cpp
    src/server.cpp)

target_include_directories(wireword PRIVATE "include/")
target_include_directories(wireword PRIVATE "libs/")