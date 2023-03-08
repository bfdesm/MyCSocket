cmake_minimum_required(VERSION 3.24)
project(socketclient C)

set(CMAKE_C_STANDARD 11)
link_libraries(ws2_32)
add_executable(socketclient client.c)
