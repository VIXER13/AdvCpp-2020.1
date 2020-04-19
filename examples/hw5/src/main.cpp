#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <thread>
#include <semaphore.h>
#include "shmap.hpp"
#include <vector>

int main() {
    shmem::ShMap<int, int> map(2048);
    pid_t child = fork();
    if (child) {
        //for(size_t i = 0; i < 1000; ++i)
            map.insert({1, 1});
    } else {
        //using namespace std::literals;
        //std::this_thread::sleep_for(1s);
        map.insert({0, 0});
        std::cout << map.at(1) << " " << map.at(2) << std::endl;
    }

    waitpid(child, nullptr, 0);

    return EXIT_SUCCESS;
}