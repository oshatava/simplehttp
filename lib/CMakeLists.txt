cmake_minimum_required (VERSION 2.6)
project (server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
)

add_library(server ${SOURCES})