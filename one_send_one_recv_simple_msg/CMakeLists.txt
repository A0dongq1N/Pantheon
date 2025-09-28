cmake_minimum_required(VERSION 3.12)
project(one_send_one_recv_simple_msg LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(one_send_one_recv_client_simple_msg client.cc)
add_executable(one_send_one_recv_server_simple_msg server.cc)


