#ifndef SHARED_MEMORY_HPP
#define SHARED_MEMORY_HPP

#include <memory>

namespace shmem {

class ShmemUniquePtrDeleter {
    const size_t size_ = 1;

 public:
    ShmemUniquePtrDeleter(const size_t size) noexcept;
    size_t getSize() const noexcept;
    void operator()(void* ptr) noexcept;
};

using ShmemUniquePtr = std::unique_ptr<void, ShmemUniquePtrDeleter>;
ShmemUniquePtr makeShmemUniquePtr(const size_t size);

}  // namespace shmem

#endif  // SHARED_MEMORY_HPP
