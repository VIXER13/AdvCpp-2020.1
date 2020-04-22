#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "shmap.hpp"

enum class Size : size_t {KBYTE = 1024,
                          MBYTE = 1024*KBYTE,
                          GBYTE = 1024*MBYTE};
static constexpr size_t count = 1024*1024;

bool testInsert(shmem::ShMap<size_t, size_t>& sequence_map, shmem::ShMap<size_t, size_t>& parallel_map) {
    for(size_t i = 0; i < count; ++i) {
        sequence_map.insert({i, i});
    }

    pid_t child = fork();
    if (child) {
        for(size_t i = 0; i < count; i += 2) {
            parallel_map.insert({i, i});
        }
    } else {
        for(size_t i = 1; i < count; i += 2) {
            parallel_map.insert({i, i});
        }
        exit(0);
    }

    waitpid(child, nullptr, 0);

    return sequence_map == parallel_map;
}

bool testAt(shmem::ShMap<size_t, size_t>& sequence_map, shmem::ShMap<size_t, size_t>& parallel_map) {
    for (auto& it : sequence_map) {
        it.second = it.first + 1;
    }

    pid_t child = fork();
    if (child) {
        for(size_t i = 0; i < count; i += 2) {
            parallel_map.at(i) = i + 1;
        }
    } else {
        for(size_t i = 1; i < count; i += 2) {
            parallel_map.at(i) = i + 1;
        }
        exit(0);
    }

    waitpid(child, nullptr, 0);

    return sequence_map == parallel_map;
}

bool testErase(shmem::ShMap<size_t, size_t>& sequence_map, shmem::ShMap<size_t, size_t>& parallel_map) {
    for(size_t i = 0; i < count; i += 2) {
        sequence_map.erase(i);
    }

    pid_t child = fork();
    if (child) {
        for(size_t i = 0; i < count; i += 4) {
            parallel_map.erase(i);
        }
    } else {
        for(size_t i = 2; i < count; i += 4) {
            parallel_map.erase(i);
        }
        exit(0);
    }

    waitpid(child, nullptr, 0);

    return sequence_map == parallel_map;
}

int main() {
    shmem::ShMap<size_t, size_t> sequence_map(size_t(Size::GBYTE)), parallel_map(size_t(Size::GBYTE));
    
    if (testInsert(sequence_map, parallel_map)) {
        std::cout << "Insert is ok!" << std::endl;
    } else {
        std::cerr << "Insert is not ok!" << std::endl;
    }

    if (testAt(sequence_map, parallel_map)) {
        std::cout << "At is ok!" << std::endl;
    } else {
        std::cerr << "At is not ok!" << std::endl;
    }

    if (testErase(sequence_map, parallel_map)) {
        std::cout << "Erase is ok!" << std::endl;
    } else {
        std::cerr << "Erase is not ok!" << std::endl;
    }

    return EXIT_SUCCESS;
}