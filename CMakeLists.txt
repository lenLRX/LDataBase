cmake_minimum_required(VERSION 2.8)
project(LDataBase)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)

message(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)