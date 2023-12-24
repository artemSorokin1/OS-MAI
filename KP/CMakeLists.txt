cmake_minimum_required(VERSION 3.20)

project(KP)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(COMPILE_FILES
    json_parce/json_parce.cpp
    graph_checker/graph_checker.cpp
)

add_executable(main main.cpp ${COMPILE_FILES})
add_executable(timer timer.cpp ${COMPILE_FILES})

find_package(nlohmann_json REQUIRED)

target_link_libraries(main nlohmann_json::nlohmann_json)
target_link_libraries(timer nlohmann_json::nlohmann_json)

