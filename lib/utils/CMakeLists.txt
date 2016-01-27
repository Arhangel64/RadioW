cmake_minimum_required(VERSION 2.8.12)
project(utils)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    exception.h
    signalcatcher.h
)

set(SOURCES
    exception.cpp
    signalcatcher.cpp
)

add_library(utils ${HEADERS} ${SOURCES})

target_link_libraries(utils Qt5::Core)