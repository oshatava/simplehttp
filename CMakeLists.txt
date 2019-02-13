cmake_minimum_required (VERSION 2.6)
project (box)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES 
    ./src/main.cpp
    ./src/server.cpp
    ./src/logger.cpp
    ./src/task.cpp
    ./src/client.cpp
    ./src/client.cpp
    ./src/response.cpp
    ./src/responses/response_fabric.cpp
    ./src/domain/version.cpp
    ./src/domain/devices.cpp
)
add_executable(box ${SOURCES})
