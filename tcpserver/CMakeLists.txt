cmake_minimum_required(VERSION 3.20)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
project(tcp_server VERSION 1.0)

add_executable(tcpserver server.cpp)
add_executable(tcpclient client.cpp)