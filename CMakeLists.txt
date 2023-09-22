cmake_minimum_required(VERSION 3.20)

project(lab1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(main lab1/src/main.cpp)
add_executable(child lab1/lib/child.cpp)
add_executable(parent lab1/lib/parent.cpp)


