cmake_minimum_required(VERSION 2.8.12)
project(wDispatcher)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    dispatcher.h
    handler.h
    defaulthandler.h
    logger.h
    parentreporter.h
)

set(SOURCES
    dispatcher.cpp
    handler.cpp
    defaulthandler.cpp
    logger.cpp
    parentreporter.cpp
)

add_library(wDispatcher ${HEADERS} ${SOURCES})

target_link_libraries(wDispatcher Qt5::Core)
