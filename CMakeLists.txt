cmake_minimum_required(VERSION 3.16)

project(bipartite_matching  VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        ${PROJECT_NAME}
        main.cpp
) 

add_subdirectory(test)

target_include_directories(
        ${PROJECT_NAME}
        PRIVATE
        src
        test
)
