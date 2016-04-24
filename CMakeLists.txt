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
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

include_directories(lib)

set(LIBJS_DIR ${CMAKE_SOURCE_DIR}/libjs)
set(CORAX_DIR ${CMAKE_BINARY_DIR}/corax)
set(MAGNUS_DIR ${CMAKE_BINARY_DIR}/magnus)
set(LORGAR_DIR ${MAGNUS_DIR}/public)

add_subdirectory(lib)
add_subdirectory(corax ${CORAX_DIR})
add_subdirectory(magnus ${MAGNUS_DIR})
add_subdirectory(lorgar ${LORGAR_DIR})

add_subdirectory(test)

