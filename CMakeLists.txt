cmake_minimum_required(VERSION 2.8.12)
project(RadioW)

set(LIBWSOCKET_DIR ${CMAKE_BINARY_DIR}/libWSocket)
set(CORAX_DIR ${CMAKE_BINARY_DIR}/corax)
set(MAGNUS_DIR ${CMAKE_BINARY_DIR}/magnus)
set(LORGAR_DIR ${MAGNUS_DIR}/public)

add_subdirectory(libWSocket ${LIBWSOCKET_DIR})
add_subdirectory(corax ${CORAX_DIR})
add_subdirectory(magnus ${MAGNUS_DIR})
add_subdirectory(lorgar ${LORGAR_DIR})
