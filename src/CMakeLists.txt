cmake_minimum_required(VERSION 3.5)
project(fastdriver)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(fastdriver ${SOURCE_FILES})

execute_process(COMMAND git describe --tags OUTPUT_VARIABLE VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
if ("${VERSION}" STREQUAL "")
    set(VERSION "v0.0")
endif ()
MESSAGE("Version: ${VERSION}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_library(fastodp lib/fastodp.cpp)

target_link_libraries(fastdriver cryptopp fastodp)
