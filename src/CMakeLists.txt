cmake_minimum_required(VERSION 3.5)
project(fastdriver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(fastdriver ${SOURCE_FILES})

target_link_libraries(fastdriver cryptopp pthread)
