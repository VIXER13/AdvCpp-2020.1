#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <thread>
#include <vector>
#include <map>
#include "allocator.hpp"

template<class T>
using Map = std::map<T, T, std::less<T>, shmem::Allocator<std::pair<const T, T>>>;

int main() {
    auto shared_memory = shmem::makeShmemUniquePtr(2048);
    auto mapmem = shmem::makeShmemUniquePtr(sizeof(std::map<int, int, std::less<int>, shmem::Allocator<std::pair<const int, int>>>));
    auto map = static_cast<std::map<int, int, std::less<int>, shmem::Allocator<std::pair<const int, int>>>*>(mapmem.get());
    shmem::Allocator<std::pair<const int, int>> alloc(shared_memory);
    map = new (map) std::map<int, int, std::less<int>, shmem::Allocator<std::pair<const int, int>>>{alloc};

    pid_t child = fork();
    if (child) {
        map->emplace(1, 1);
    } else {
        using namespace std::literals;
        std::this_thread::sleep_for(1s);
        map->emplace(2, 2);
        for(const auto& it : *map) {
            std::cout << it.first << " " << it.second << std::endl;
        }
        std::cout << std::endl;
    }

    waitpid(child, nullptr, 0);

    return EXIT_SUCCESS;
}