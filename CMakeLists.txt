cmake_minimum_required(VERSION 2.8.12)
project(RadioW)

function(add_jslib file name path arch)
    execute_process(COMMAND
        node polymorph 
            ${LIBJS_DIR}/${file} 
            ${path}/lib/${file} 
            ${name} 
            ${arch}
            ${path}/lib 
        WORKING_DIRECTORY ${LIBJS_DIR})
endfunction(add_jslib)

include(CheckCXXCompilerFlag)
set(CMAKE_CXX_STANDARD 11)

include_directories(lib)

set(LIBJS_DIR ${CMAKE_SOURCE_DIR}/libjs)
set(ROBOUTE_DIR ${CMAKE_BINARY_DIR}/roboute)
set(CORAX_DIR ${CMAKE_BINARY_DIR}/corax)
set(MAGNUS_DIR ${CMAKE_BINARY_DIR}/magnus)
set(LORGAR_DIR ${MAGNUS_DIR}/public)
set(PERTURABO_DIR ${CMAKE_BINARY_DIR}/perturabo)

add_subdirectory(lib)
add_subdirectory(corax ${CORAX_DIR})
add_subdirectory(magnus ${MAGNUS_DIR})
add_subdirectory(lorgar ${LORGAR_DIR})
add_subdirectory(roboute ${ROBOUTE_DIR})
add_subdirectory(perturabo ${PERTURABO_DIR})

add_subdirectory(test)
