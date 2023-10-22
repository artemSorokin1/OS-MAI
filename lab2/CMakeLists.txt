cmake_minimum_required(VERSION 3.20)

project(lab2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(EvenOddMergeSort src/main.cpp lib/functions.cpp)

