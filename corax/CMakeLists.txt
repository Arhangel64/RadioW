cmake_minimum_required(VERSION 2.8.12)
project(corax)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_executable(corax main.cpp corax.cpp)

target_link_libraries(corax Qt5::Core)
target_link_libraries(corax Qt5::Network)

target_link_libraries(corax wSocket)

install(TARGETS corax RUNTIME DESTINATION bin)
