cmake_minimum_required(VERSION 3.13) 
project(server)              

set(SOURCE_FILES server.c)

add_executable(server ${SOURCE_FILES})