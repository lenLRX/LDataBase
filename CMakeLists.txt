cmake_minimum_required(VERSION 2.8)
project(LDataBase)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/build/lib)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(include)

message(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)