cmake_minimum_required(VERSION 2.8.12)
project(lorgar)

add_subdirectory(css)
add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(core)

configure_file(index.html index.html)
configure_file(favicon.ico favicon.ico COPYONLY)
configure_file(main.js main.js)