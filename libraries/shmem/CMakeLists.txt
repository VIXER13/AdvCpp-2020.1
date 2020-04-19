cmake_minimum_required(VERSION 3.10)

project(shmem)

find_package(Threads)

set(SHMEM_SOURCES "src/shared_memory.cpp"
                  "src/semaphore.cpp"
                  "src/shmem_exception.cpp")
set(SHMEM_HEADERS "include"
                  "containers")
add_library(shmem STATIC ${SHMEM_SOURCES})
target_include_directories(shmem PUBLIC ${SHMEM_HEADERS})
target_link_libraries(shmem ${CMAKE_THREAD_LIBS_INIT})