cmake_minimum_required(VERSION 2.8)
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

add_library(fastodp SHARED lib/fastodp.cpp)
add_library(fastodpStatic STATIC lib/fastodp.cpp)
set_target_properties(fastodpStatic PROPERTIES OUTPUT_NAME fastodp)

target_link_libraries(fastdriver cryptopp fastodp)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

install(TARGETS fastodp fastodpStatic DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS fastdriver DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES lib/fastodp.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
