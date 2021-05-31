cmake_minimum_required(VERSION 3.16)
project(FastestPath)

set(CMAKE_CXX_STANDARD 14)

set(PROJECT_HEADERS
        SF.h
        Edge.h
        UE.h
        )

set(PROJECT_SOURCES
        main.cpp
        SF.cpp
        UE.cpp)

add_executable(FastestPath ${PROJECT_SOURCES} ${PROJECT_HEADERS})