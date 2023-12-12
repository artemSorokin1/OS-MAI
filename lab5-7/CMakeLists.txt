cmake_minimum_required(VERSION 3.20)

project(labs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCE_FILES
    ZeroMQ_API/ZeroMQ_API.cpp
    Tree/tree.cpp
    Timer/TimerManager.cpp
)

add_executable(test main2.cpp ${SOURCE_FILES})
add_executable(compute compute.cpp ${SOURCE_FILES})
add_executable(lab5-7 main.cpp ${SOURCE_FILES})

find_package(cppzmq CONFIG REQUIRED)
target_link_libraries(lab5-7 PRIVATE cppzmq cppzmq-static)
target_link_libraries(compute PRIVATE cppzmq cppzmq-static)
target_link_libraries(test PRIVATE cppzmq cppzmq-static)


