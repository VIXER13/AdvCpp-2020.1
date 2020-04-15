cmake_minimum_required(VERSION 3.10)

project(shmem)

set(SHMEM_SOURCES src/shared_memory.cpp)
set(SHMEM_HEADERS include)
add_library(shmem STATIC ${SHMEM_SOURCES})
target_include_directories(shmem PUBLIC ${SHMEM_HEADERS})