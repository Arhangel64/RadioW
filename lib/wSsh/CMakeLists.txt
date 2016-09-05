cmake_minimum_required(VERSION 2.8.12)
project(wSsh)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    sshsocket.h
    qsshsocket.h
)

set(SOURCES
    sshsocket.cpp
    qsshsocket.cpp
)

add_library(wSsh ${HEADERS} ${SOURCES})

target_link_libraries(wSsh Qt5::Core)
target_link_libraries(wSsh ssh)
