cmake_minimum_required(VERSION 2.8.12)
project(test)

find_package(CxxTest)
if(CXXTEST_FOUND)
    include_directories(${CXXTEST_INCLUDE_DIR})
    enable_testing()
    CXXTEST_ADD_TEST(testtypes testtypes.cpp ${CMAKE_CURRENT_SOURCE_DIR}/testTypes.h)
    target_link_libraries(testtypes wType)
    
    CXXTEST_ADD_TEST(testtools testtools.cpp ${CMAKE_CURRENT_SOURCE_DIR}/testTools.h)
    target_link_libraries(testtools tools)
    target_link_libraries(testtools tag)
    
    find_package(Qt5Core REQUIRED)

    set(CMAKE_INCLUDE_CURRENT_DIR ON)
    set(CMAKE_AUTOMOC ON)
    CXXTEST_ADD_TEST(testsocket testsocket.cpp ${CMAKE_CURRENT_SOURCE_DIR}/testSocket.h)
    target_link_libraries(testsocket Qt5::Core)
    target_link_libraries(testsocket wSocket)
    
    CXXTEST_ADD_TEST(testdispatcher testdispatcher.cpp ${CMAKE_CURRENT_SOURCE_DIR}/testDispatcher.h)
    target_link_libraries(testdispatcher Qt5::Core)
    target_link_libraries(testdispatcher wDispatcher)
    target_link_libraries(testdispatcher wType)
    
endif()

