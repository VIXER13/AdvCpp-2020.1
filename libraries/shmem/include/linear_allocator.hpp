#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include <string.h>
#include "shared_memory.hpp"
#include "shmem_exception.hpp"

namespace shmem {

template<class T>
class LinearAllocator {
    template<class U>
    friend class LinearAllocator;

 public:
    ShmemUniquePtr& pool_;
    size_t offset_bytes_ = 0;

 public:
    using size_type       = std::size_t;
    using value_type      = T;
    using pointer         = T*;

    explicit LinearAllocator(ShmemUniquePtr& pool) :
        pool_(pool) {
        if (offset_bytes_ > pool_.get_deleter().getBufferSize()) {
            throw ShmemException{"offset is greater than buffer"};
        }
    }

    LinearAllocator(const LinearAllocator<T>& other) noexcept = default;

    template<class U>
    LinearAllocator(const LinearAllocator<U>& other) noexcept :
        pool_(other.pool_), offset_bytes_(other.offset_bytes_) {}

    pointer allocate(const size_type n) {
        if (size_type(pool_.get()) + offset_bytes_ + n * sizeof(T) >
            size_type(pool_.get()) + pool_.get_deleter().getBufferSize()) {
            throw ShmemException{"Shmem bad alloc"};
        }
        offset_bytes_ += n * sizeof(T);
        return reinterpret_cast<pointer>(size_type(pool_.get()) + offset_bytes_ - n * sizeof(T));
    }

    void deallocate(pointer p, size_type n) {
        if (size_type(p) - n * sizeof(T) == size_type(pool_.get())) {
            offset_bytes_ = 0;
        }
    }

    template <class U>
    friend bool operator==(const LinearAllocator<T>& left, const LinearAllocator<U>& right);
    template <class U>
    friend bool operator!=(const LinearAllocator<T>& left, const LinearAllocator<U>& right);
};

template <class T, class U>
bool operator==(const LinearAllocator<T>& left, const LinearAllocator<U>& right) {
    return left.pool_ == right.pool_;
}

template <class T, class U>
bool operator!=(const LinearAllocator<T>& left, const LinearAllocator<U>& right) {
    return !(left == right);
}

}  // namespace shmem

#endif  // ALLOCATOR_HPP
