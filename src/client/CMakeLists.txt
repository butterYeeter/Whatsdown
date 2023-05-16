cmake_minimum_required(VERSION 3.13) 
project(client)              

set(SOURCE_FILES client.c)

add_executable(client ${SOURCE_FILES})
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS} util)