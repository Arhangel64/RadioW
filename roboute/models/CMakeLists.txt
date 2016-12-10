cmake_minimum_required(VERSION 2.8.12)
project(robouteModels)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS 
    applistmodel.h
    appmodel.h
    service.h
    apppropertiesmodel.h
)

set(SOURCES
    applistmodel.cpp
    appmodel.cpp
    service.cpp
    apppropertiesmodel.cpp
)

add_library(robouteModels STATIC ${HEADERS} ${SOURCES})

target_link_libraries(robouteModels Qt5::Core)
target_link_libraries(robouteModels wType)
target_link_libraries(robouteModels wSocket)
target_link_libraries(robouteModels wSsh)
target_link_libraries(robouteModels wController)
