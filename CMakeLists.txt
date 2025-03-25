cmake_minimum_required(VERSION 3.20)
project(network_programming)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_VERSION 20)
set(CMAKE_CXX_VERSION_REQUIRED TRUE)

add_executable(echo_client1 day1/echo_client.cpp)
add_executable(echo_server1 day1/echo_server.cpp)

add_executable(ex_client day1_exercises/echo_client.cpp)
add_executable(ex_server day1_exercises/echo_server.cpp)

add_executable(getaddrinfo day1/getaddrinfo.cpp)
