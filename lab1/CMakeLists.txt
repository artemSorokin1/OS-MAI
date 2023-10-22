cmake_minimum_required(VERSION 3.20)

project(lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main src/main.cpp)
add_executable(child lib/child.cpp)
add_executable(parent lib/parent.cpp)

