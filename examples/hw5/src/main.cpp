#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <thread>
#include <vector>
#include <map>
#include "shmap.hpp"

int main() {
    shmem::ShMap<int, int> map(2048);
    pid_t child = fork();
    if (child) {
        map.insert({1, 1});
    } else {
        //using namespace std::literals;
        //std::this_thread::sleep_for(1s);
        map.insert({2, 2});
        std::cout << map.at(1) << " " << map.at(2) << std::endl;
    }

    waitpid(child, nullptr, 0);
    
    /*
    auto shared_memory = shmem::makeShmemUniquePtr(2048);
    //auto mapmem = shmem::makeShmemUniquePtr(sizeof(std::map<int, int, std::less<int>, shmem::LinearAllocator<std::pair<const int, int>>>));
    auto map = static_cast<std::map<int, int, std::less<int>, shmem::LinearAllocator<std::pair<const int, int>>>*>(shared_memory.get());
    size_t offset = sizeof(std::map<int, int, std::less<int>, shmem::LinearAllocator<std::pair<const int, int>>>);
    {
    shmem::LinearAllocator<std::pair<const int, int>> alloc(shared_memory, offset);
    map = new (map) std::map<int, int, std::less<int>, shmem::LinearAllocator<std::pair<const int, int>>>{alloc};
    }

    pid_t child = fork();
    if (child) {
        map->emplace(1, 1);
        map->emplace(2, 2);
        map->emplace(3, 3);
        //map->get_allocator();
    } else {
        using namespace std::literals;
        std::this_thread::sleep_for(1s);
        map->emplace(5, 5);
        for(const auto& it : *map) {
            std::cout << it.first << " " << it.second << std::endl;
        }
        std::cout << std::endl;
    }

    waitpid(child, nullptr, 0);
    */
    

    return EXIT_SUCCESS;
}