cmake_minimum_required(VERSION 2.8.12)
project(qssh)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    qsshsocket.h
)

set(SOURCES
    qsshsocket.cpp
)

add_library(qssh ${HEADERS} ${SOURCES})

target_link_libraries(qssh Qt5::Core)
