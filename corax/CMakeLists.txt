cmake_minimum_required(VERSION 2.8.12)
project(corax)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    corax.h
    commands.h
    connector.h
)

set(SOURCES
    main.cpp
    corax.cpp
    commands.cpp
    connector.cpp
)

add_executable(corax ${HEADERS} ${SOURCES})

target_link_libraries(corax Qt5::Core)
target_link_libraries(corax Qt5::Network)

target_link_libraries(corax wSocket)
target_link_libraries(corax wDispatcher)
target_link_libraries(corax utils)
target_link_libraries(corax wModel)

install(TARGETS corax RUNTIME DESTINATION bin)
