cmake_minimum_required(VERSION 2.8.12)
project(tools)

set(HEADERS
    file.h
)

set(SOURCES
    file.cpp
)

add_library(tools ${HEADERS} ${SOURCES})

target_link_libraries(tools wType)

