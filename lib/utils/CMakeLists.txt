cmake_minimum_required(VERSION 2.8.12)
project(utils)

set(HEADERS
    exception.h
)

set(SOURCES
    exception.cpp
)

add_library(utils ${HEADERS} ${SOURCES})