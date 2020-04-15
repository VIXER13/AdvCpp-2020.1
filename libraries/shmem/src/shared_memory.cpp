#include "shared_memory.hpp"
#include <sys/mman.h>

namespace shmem {

ShmemUniquePtrDeleter::ShmemUniquePtrDeleter(const size_t size) noexcept :
    size_(size) {}

size_t ShmemUniquePtrDeleter::getSize() const noexcept {
    return size_;
}

void ShmemUniquePtrDeleter::operator()(void* ptr) noexcept {
    ::munmap(ptr, size_);
}

ShmemUniquePtr makeShmemUniquePtr(const size_t size) {
    void* mmap = ::mmap(nullptr, size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mmap == MAP_FAILED) {
        throw std::bad_alloc{};
    }
    return {mmap, ShmemUniquePtrDeleter{size}};
}

}  // namespace shmem
