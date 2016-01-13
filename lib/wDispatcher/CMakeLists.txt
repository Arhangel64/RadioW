cmake_minimum_required(VERSION 2.8.12)
project(wDispatcher)

set(HEADERS
    dispatcher.h
)

set(SOURCES
    dispatcher.cpp
)

add_library(wDispatcher ${HEADERS} ${SOURCES})