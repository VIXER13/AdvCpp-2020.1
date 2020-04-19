#include "shared_memory.hpp"
#include <sys/mman.h>
#include "shmem_exception.hpp"

namespace shmem {

ShmemUniquePtrDeleter::ShmemUniquePtrDeleter(const size_t size) noexcept :
    size_(size) {}

size_t ShmemUniquePtrDeleter::getBufferSize() const noexcept {
    return size_;
}

void ShmemUniquePtrDeleter::operator()(void* ptr) noexcept {
    ::munmap(ptr, size_);
}

ShmemUniquePtr makeShmemUniquePtr(const size_t size) {
    void* mmap = ::mmap(nullptr, size, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mmap == MAP_FAILED) {
        throw ShmemException{"mmap failed"};
    }
    return {mmap, ShmemUniquePtrDeleter{size}};
}

}  // namespace shmem
