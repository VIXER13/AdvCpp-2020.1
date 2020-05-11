cmake_minimum_required(VERSION 3.10)

project(hw6)

SET(HW6_EXAMPLE_SOURCES src/main.cpp)
SET(HW6_EXAMPLE_HEADERS include)
add_executable(hw6 ${HW6_EXAMPLE_SOURCES})
target_include_directories(hw6 PUBLIC ${HW6_EXAMPLE_HEADERS})
target_link_libraries(hw6 http_framework)