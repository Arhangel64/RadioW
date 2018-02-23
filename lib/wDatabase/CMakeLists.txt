cmake_minimum_required(VERSION 2.8.12)
project(database)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    database.h
    resourcecache.h
)

set(SOURCES
    database.cpp
    resourcecache.cpp
)


add_library(wDatabase STATIC ${HEADERS} ${SOURCES})

target_link_libraries(wDatabase Qt5::Core)
target_link_libraries(wDatabase lmdb)
target_link_libraries(wDatabase wType)
target_link_libraries(wDatabase wModel)
