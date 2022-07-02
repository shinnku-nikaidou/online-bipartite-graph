cmake_minimum_required(VERSION 3.16)

project(bipartite_matching  VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(
        ${PROJECT_NAME}
        src/bipartite_graph.hpp
        src/kvv90_test.hpp
        src/kvv90.hpp
        src/utils.hpp
        src/weighted_bigraph_test.hpp
        src/weighted_bipartite_graph.hpp
        main.cpp
) 
