cmake_minimum_required(VERSION 2.8.12)
project(models)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    model.h
    modelstring.h
    list.h
)

set(SOURCES
    model.cpp
    modelstring.cpp
    list.cpp
)

add_library(wModels STATIC ${HEADERS} ${SOURCES})

target_link_libraries(wModels Qt5::Core)
target_link_libraries(wModels wSocket)
target_link_libraries(wModels wDispatcher)
target_link_libraries(wModels wType)
