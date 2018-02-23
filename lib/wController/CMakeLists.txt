cmake_minimum_required(VERSION 2.8.12)
project(controller)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    controller.h
    controllerstring.h
    list.h
    vocabulary.h
    attributes.h
    catalogue.h
    collection.h
)

set(SOURCES
    controller.cpp
    controllerstring.cpp
    list.cpp
    vocabulary.cpp
    attributes.cpp
    catalogue.cpp
    collection.cpp
)

add_library(wController STATIC ${HEADERS} ${SOURCES})

target_link_libraries(wController Qt5::Core)
target_link_libraries(wController wSocket)
target_link_libraries(wController wDispatcher)
target_link_libraries(wController wType)

