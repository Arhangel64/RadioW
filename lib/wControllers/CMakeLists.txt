cmake_minimum_required(VERSION 2.8.12)
project(controllers)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    controller.h
)

set(SOURCES
    controller.cpp
)

add_library(wControllers STATIC ${HEADERS} ${SOURCES})

target_link_libraries(wControllers Qt5::Core)
target_link_libraries(wControllers wSocket)
target_link_libraries(wControllers wDispatcher)
target_link_libraries(wControllers wType)

