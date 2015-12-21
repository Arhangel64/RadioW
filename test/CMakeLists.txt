cmake_minimum_required(VERSION 2.8.12)
project(test)

set(HEADERS
    testTypes.h
)

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
    CXXTEST_ADD_TEST(unittest unittest.cpp ${CMAKE_CURRENT_SOURCE_DIR}/${HEADERS})
    target_link_libraries(unittest wType)
    
endif()

