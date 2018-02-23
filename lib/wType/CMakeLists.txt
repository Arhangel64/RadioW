cmake_minimum_required(VERSION 2.8.12)
project(type)

set(HEADERS
    bytearray.h
    object.h
    string.h
    vocabulary.h
    uint64.h
    address.h
    boolean.h
    event.h
    vector.h
    blob.h
)

set(SOURCES
    bytearray.cpp
    object.cpp
    string.cpp
    vocabulary.cpp
    uint64.cpp
    address.cpp
    boolean.cpp
    event.cpp
    vector.cpp
    blob.cpp
)

add_library(wType ${HEADERS} ${SOURCES})
target_link_libraries(wType utils)
