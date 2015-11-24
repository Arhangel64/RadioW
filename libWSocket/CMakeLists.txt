cmake_minimum_required(VERSION 2.8.12)
project(wSocket)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5WebSockets REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(LWS wevent.cpp wserver.cpp wsetid.cpp wsetremotename.cpp wsocket.cpp)
add_library(wSocket SHARED ${LWS})

target_link_libraries(wSocket Qt5::Core)
target_link_libraries(wSocket Qt5::Network)
target_link_libraries(wSocket Qt5::WebSockets)

install(TARGETS wSocket LIBRARY DESTINATION bin)