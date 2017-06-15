cmake_minimum_required(VERSION 2.6)
project(tables)

set(SOURCES
    table.cpp
    cmap.cpp
    hhea.cpp
    hmtx.cpp
    head.cpp
)

add_library(tables STATIC ${SOURCES})
