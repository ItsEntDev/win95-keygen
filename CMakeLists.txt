cmake_minimum_required(VERSION 3.20)
project(win95-keygen)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wconversion -Wsign-conversion -Werror")

add_executable(win95-keygen main.cpp
        cl_args.hpp
        cl_args.cpp
        shared_utils.hpp
        shared_utils.cpp
        key95.hpp
        key95.cpp
)
