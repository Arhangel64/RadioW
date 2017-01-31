cmake_minimum_required(VERSION 2.8.12)
project(robouteViews)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS 
    mainview.h
    detailedview.h
    newappdialogue.h
    commandform.h
)

set(SOURCES
    mainview.cpp
    detailedview.cpp
    newappdialogue.cpp
    commandform.cpp
)

add_library(robouteViews STATIC ${HEADERS} ${SOURCES})

target_link_libraries(robouteViews Qt5::Core)
target_link_libraries(robouteViews Qt5::Widgets)
