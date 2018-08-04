cmake_minimum_required(VERSION 2.8.12)
project(model)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    model.h
    modelstring.h
    list.h
    vocabulary.h
    attributes.h
    icatalogue.h
    catalogue.h
    button.h
    file/file.h
)

set(SOURCES
    model.cpp
    modelstring.cpp
    list.cpp
    vocabulary.cpp
    attributes.cpp
    icatalogue.cpp
    catalogue.cpp
    button.cpp
    file/file.cpp
)

add_library(wModel STATIC ${HEADERS} ${SOURCES})

target_link_libraries(wModel Qt5::Core)
target_link_libraries(wModel wServerUtils)
target_link_libraries(wModel wType)
target_link_libraries(wModel wController)
