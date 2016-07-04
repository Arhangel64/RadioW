cmake_minimum_required(VERSION 2.8.12)
project(magnus)

add_subdirectory(config)
add_subdirectory(lib)
add_subdirectory(middleware)
add_subdirectory(views)
add_subdirectory(test)
add_subdirectory(core)
add_subdirectory(models)
add_subdirectory(pages)

configure_file(package.json package.json)
configure_file(app.js app.js)

execute_process(COMMAND npm install WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})