cmake_minimum_required(VERSION 2.8.12)

add_subdirectory(wSocket)
add_subdirectory(utils)
add_subdirectory(wDispatcher)
add_subdirectory(wType)
add_subdirectory(wContainer)
add_subdirectory(wTest)
add_subdirectory(wModel)

configure_file(log.js log.js)
configure_file(httpError.js httpError.js)
