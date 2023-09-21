cmake_minimum_required(VERSION 3.26)
project(win95-keygen)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror")

add_executable(win95-keygen main.cpp
        shared_utils.hpp
        shared_utils.cpp
        key95.hpp
        key95.cpp
)
