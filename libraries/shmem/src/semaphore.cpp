#include "semaphore.hpp"
#include "shmem_exception.hpp"

namespace shmem {

Semaphore::Semaphore(const unsigned int init_value) {
    if (::sem_init(&semaphore_, 1, init_value) == -1) {
        throw ShmemException{"sem_init failed."};
    }
}

Semaphore::~Semaphore() noexcept {
    ::sem_destroy(&semaphore_);
}

void Semaphore::lock() {
    while (::sem_wait(&semaphore_) == -1) {
        if (errno == EINTR) {
            continue;
        }
        throw ShmemException{"Wait error"};
    }
}

void Semaphore::unlock() {
    if (::sem_post(&semaphore_) == -1) {
        throw ShmemException{"post failed"};
    }
}

}  // namespace shmem
