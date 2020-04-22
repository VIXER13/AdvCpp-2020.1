#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include <pthread.h>
#include <semaphore.h>

namespace shmem {

class Semaphore {
    sem_t semaphore_ = {};

    Semaphore(const Semaphore&) = delete;
    Semaphore& operator=(const Semaphore&) = delete;

 public:
    Semaphore(Semaphore&& other) = default;
    Semaphore& operator=(Semaphore&& other) = default;

    explicit Semaphore(const unsigned int init_value = 1);
    ~Semaphore() noexcept;

    void lock();
    void unlock();
};

}  // namespace shmem

#endif  // SEMAPHORE_HPP
