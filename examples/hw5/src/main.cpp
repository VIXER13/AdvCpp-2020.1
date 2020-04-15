#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <memory>
#include <sys/mman.h>
#include <thread>
#include <vector>
#include <iterator>
#include "allocator.hpp"

int main() {
    auto shared_memory = shmem::makeShmemUniquePtr(2048);
    auto shmem = shmem::makeShmemUniquePtr(sizeof(std::vector<int, shmem::Allocator<int>>));
    std::vector<int, shmem::Allocator<int>>* vector =
        reinterpret_cast<std::vector<int, shmem::Allocator<int>>*>(shmem.get());
    shmem::Allocator<int> alloc(shared_memory);
    vector = new (vector) std::vector<int, shmem::Allocator<int>>{alloc};

    pid_t child = fork();
    if (child) {
        vector->push_back(1);
        vector->push_back(1);
        vector->push_back(1);
    } else {
        using namespace std::literals;
        std::this_thread::sleep_for(1s);
        vector->push_back(2);
        vector->push_back(2);
        vector->push_back(2);
        for(size_t i = 0; i < vector->size(); ++i) {
            std::cout << (*vector)[i] << " ";
        }
        std::cout << std::endl;
    }

    waitpid(child, nullptr, 0);

    return EXIT_SUCCESS;
}