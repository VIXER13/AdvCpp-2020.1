cmake_minimum_required(VERSION 3.10)

project(hw4)

SET(HW4_EXAMPLE_SOURCES src/main.cpp)
SET(HW4_EXAMPLE_HEADERS include)
add_executable(hw4 ${HW4_EXAMPLE_SOURCES})
target_include_directories(hw4 PUBLIC ${HW4_EXAMPLE_HEADERS})
target_link_libraries(hw4 epoll_server)
target_link_libraries(hw4 logger)