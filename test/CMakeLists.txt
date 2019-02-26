cmake_minimum_required (VERSION 2.6)
project (servertest)

find_package(GTest REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(../lib/include ${GTEST_INCLUDE_DIRS})

set(SOURCES_TEST     
    ./testJSON.cpp
    ./testMain.cpp
)

link_directories(../lib)

add_executable(servertest ${SOURCES_TEST} ${SOURCES})

target_link_libraries(servertest ${GTEST_LIBRARIES} server)