cmake_minimum_required(VERSION 2.8.12)
project(magnus)

configure_file(package.json package.json)
configure_file(app.js app.js)

execute_process(COMMAND npm install WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})