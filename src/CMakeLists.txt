cmake_minimum_required(VERSION 3.1 FATAL_ERROR)
set (CMAKE_CXX_STANDARD 11)
project(fastdriver)


set(SOURCE_FILES main.cpp)
add_executable(fastdriver ${SOURCE_FILES})

execute_process(COMMAND git describe --tags OUTPUT_VARIABLE VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
if ("${VERSION}" STREQUAL "")
    set(VERSION "v0.0")
endif ()
MESSAGE("Version: ${VERSION}")

find_package(Boost COMPONENTS program_options REQUIRED)

include_directories(${Boost_INCOUDE_DIRS})
target_link_libraries(fastdriver ${Boost_LIBRARIES})

add_library(fastodp SHARED lib/fastodp.cpp)
add_library(fastodpStatic STATIC lib/fastodp.cpp)
set_target_properties(fastodpStatic PROPERTIES OUTPUT_NAME fastodp)

target_link_libraries(fastodp cryptopp pthread)
target_link_libraries(fastodpStatic cryptopp pthread)
target_link_libraries(fastdriver cryptopp fastodp)

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX /usr)
endif ()

install(TARGETS fastodp fastodpStatic DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
install(TARGETS fastdriver DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(FILES lib/fastodp.h DESTINATION ${CMAKE_INSTALL_PREFIX}/include)
