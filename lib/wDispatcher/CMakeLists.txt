cmake_minimum_required(VERSION 2.8.12)
project(wDispatcher)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    dispatcher.h
    handler.h
)

set(SOURCES
    dispatcher.cpp
    handler.cpp
)

add_library(wDispatcher ${HEADERS} ${SOURCES})