cmake_minimum_required(VERSION 2.8.12)
project(wEvent)

find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(WE wevent.cpp wsetid.cpp wsetremotename.cpp wteststring.cpp)
add_library(wEvent STATIC ${WE})

target_link_libraries(wEvent Qt5::Core)