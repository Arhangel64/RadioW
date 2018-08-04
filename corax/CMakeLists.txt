cmake_minimum_required(VERSION 2.8.12)
project(corax)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

set(HEADERS
    corax.h
    tools/parser.h
    tools/audioid.h
    tools/audiotag.h
)

set(SOURCES
    main.cpp
    corax.cpp
    tools/parser.cpp
    tools/audioid.cpp
    tools/audiotag.cpp
)

add_executable(corax ${HEADERS} ${SOURCES})
add_subdirectory(models)

target_link_libraries(corax Qt5::Core)
target_link_libraries(corax Qt5::Network)

target_link_libraries(corax wSocket)
target_link_libraries(corax wDispatcher)
target_link_libraries(corax utils)
target_link_libraries(corax wModel)
target_link_libraries(corax wDatabase)
target_link_libraries(corax tag)
target_link_libraries(corax tools)
target_link_libraries(corax coraxModels)
target_link_libraries(corax wServerUtils)

install(TARGETS corax RUNTIME DESTINATION bin)
