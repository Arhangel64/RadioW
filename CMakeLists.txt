cmake_minimum_required(VERSION 2.8.12)
project(RadioW)

add_subdirectory(libWSocket)
add_subdirectory(corax)

target_include_directories(corax PUBLIC libWSocket)