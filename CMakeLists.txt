cmake_minimum_required(VERSION 3.26)
project(win95-keygen)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Weffc++ -Wextra -Wconversion -Wsign-conversion -Werror")

add_executable(main.cpp
        shared_utils.h
        shared_utils.cpp
        key95.h
        key95.cpp
)
