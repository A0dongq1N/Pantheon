cmake_minimum_required(VERSION 3.12)
project(one_send_one_recv_struct_msg LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(one_send_one_recv_client_struct_msg client.cc)
add_executable(one_send_one_recv_server_struct_msg server.cc)
