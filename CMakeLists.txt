cmake_minimum_required(VERSION 3.10)
project(my_http C)

set(CMAKE_C_STANDARD 99)

add_executable(server server.c)
add_executable(clinet clinet.c)
