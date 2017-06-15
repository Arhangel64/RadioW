cmake_minimum_required(VERSION 2.6)
project(fontparser)

set(SOURCES 
    font.cpp
)

add_subdirectory(tables)

add_library(font STATIC ${SOURCES})
target_link_libraries(font tables)

add_executable(fontparser main.cpp)
target_link_libraries(fontparser font)


install(TARGETS fontparser RUNTIME DESTINATION bin)
