cmake_minimum_required(VERSION 2.8.12)
project(roboute)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    roboute.h
    mainwindow.h
)

set(SOURCES
    main.cpp
    roboute.cpp
    mainwindow.cpp
)

add_executable(roboute ${HEADERS} ${SOURCES})

add_subdirectory(views)
add_subdirectory(models)

target_link_libraries(roboute Qt5::Core)
target_link_libraries(roboute Qt5::Widgets)
target_link_libraries(roboute Qt5::Network)

target_link_libraries(roboute wSocket)
target_link_libraries(roboute wDispatcher)
target_link_libraries(roboute utils)
target_link_libraries(roboute robouteViews)
target_link_libraries(roboute robouteModels)

install(TARGETS roboute RUNTIME DESTINATION bin)
