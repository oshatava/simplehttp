cmake_minimum_required (VERSION 2.6)
project (box)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./lib/include)
include_directories(./src)

link_directories(${CMAKE_SOURCE_DIR}/lib)

set(SOURCES 
    ./src/main.cpp    
    ./src/version.cpp
    ./src/devices.cpp
    ./src/error404.cpp
    ./src/http_logger.cpp
    ./src/coder.cpp
)

add_executable(box ${SOURCES})

target_link_libraries(box server)