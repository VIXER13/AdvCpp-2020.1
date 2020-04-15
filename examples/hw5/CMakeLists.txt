cmake_minimum_required(VERSION 3.10)

project(hw5)

SET(HW5_EXAMPLE_SOURCES src/main.cpp)
SET(HW5_EXAMPLE_HEADERS include)
add_executable(hw5 ${HW5_EXAMPLE_SOURCES})
target_include_directories(hw5 PUBLIC ${HW5_EXAMPLE_HEADERS})
target_link_libraries(hw5 shmem)