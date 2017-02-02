cmake_minimum_required(VERSION 2.8.12)
project(wServerUtils)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    commands.h
    connector.h
)

set(SOURCES
    commands.cpp
    connector.cpp
)

add_library(wServerUtils ${HEADERS} ${SOURCES})

target_link_libraries(wServerUtils Qt5::Core)
target_link_libraries(wServerUtils wType)
target_link_libraries(wServerUtils wModel)
target_link_libraries(wServerUtils wSocket)
target_link_libraries(wServerUtils wDispatcher)

