cmake_minimum_required(VERSION 2.8.12)
project(perturabo)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    perturabo.h
)

set(SOURCES
    main.cpp
    perturabo.cpp
)

add_executable(perturabo ${HEADERS} ${SOURCES})

target_link_libraries(perturabo Qt5::Core)
target_link_libraries(perturabo Qt5::Network)

target_link_libraries(perturabo wSocket)
target_link_libraries(perturabo wDispatcher)
target_link_libraries(perturabo utils)
target_link_libraries(perturabo wModel)
target_link_libraries(perturabo wDatabase)
target_link_libraries(perturabo wServerUtils)

install(TARGETS perturabo RUNTIME DESTINATION bin)
