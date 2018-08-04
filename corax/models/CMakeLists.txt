cmake_minimum_required(VERSION 2.8.12)
project(coraxModels)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    player.h
    proxysong.h
)

set(SOURCES
    player.cpp
    proxysong.cpp
)

add_library(coraxModels STATIC ${HEADERS} ${SOURCES})

target_link_libraries(coraxModels wModel)
