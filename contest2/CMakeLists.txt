cmake_minimum_required(VERSION 3.23)
project(contest2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined -Wall -Wextra ")
add_executable(contest2 H.v2.cpp)