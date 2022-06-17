cmake_minimum_required(VERSION 3.23.2)

project(bipartite_matching  VERSION 1.0)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        ${PROJECT_NAME}
        main.cpp
) 
