cmake_minimum_required(VERSION 3.10)

project(Process)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra)

#LIBRARIES

add_subdirectory(libraries/file_descriptor)
add_subdirectory(libraries/process)
add_subdirectory(libraries/logger)
add_subdirectory(libraries/tcp)
add_subdirectory(libraries/shmem)
add_subdirectory(libraries/epoll_server)

#EXAMPLES

add_subdirectory(examples/hw1)
add_subdirectory(examples/hw2)
add_subdirectory(examples/hw3)
add_subdirectory(examples/hw5)
add_subdirectory(examples/hw4)
