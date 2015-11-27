cmake_minimum_required(VERSION 2.8.12)
project(lorgar)

add_subdirectory(css)
configure_file(index.html index.html)
configure_file(favicon.ico favicon.ico COPYONLY)