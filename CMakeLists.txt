cmake_minimum_required(VERSION 3.9)
project(untitled2)

set(CMAKE_CXX_STANDARD 11)
link_libraries(ws2_32 wsock32)

add_executable(untitled2 main.cpp Logins.h)