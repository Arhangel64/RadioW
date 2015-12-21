cmake_minimum_required(VERSION 2.8.12)
project(wSocket)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5WebSockets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(LWS wserver.cpp wsocket.cpp)
add_library(wSocket SHARED ${LWS})

target_include_directories(wSocket PUBLIC wEvent)

add_subdirectory(wEvent)

target_link_libraries(wSocket Qt5::Core)
target_link_libraries(wSocket Qt5::Network)
target_link_libraries(wSocket Qt5::WebSockets)
target_link_libraries(wSocket wEvent)

