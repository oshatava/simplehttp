cmake_minimum_required (VERSION 2.6)
project (server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

include_directories(include)

set(SOURCES 
    ./src/server.cpp
    ./src/logger.cpp
    ./src/task.cpp
    ./src/client.cpp
    ./src/client.cpp
    ./src/entity.cpp
    ./src/json.cpp
    ./src/http.cpp
    ./src/configuration.cpp
    ./src/utils.cpp
)

add_library(server ${SOURCES})