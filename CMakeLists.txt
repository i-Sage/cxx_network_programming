cmake_minimum_required(VERSION 3.20)
project(network_programming)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_VERSION 20)
set(CMAKE_CXX_VERSION_REQUIRED TRUE)

add_executable(echo_client echo_client.cpp)
add_executable(echo_server echo_server.cpp)