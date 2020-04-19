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

    ShmemUniquePtr& pool_;
    size_t offset_bytes_ = 0;

 public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    template<typename U>
	struct rebind {
        using other = LinearAllocator<U>;
    };

    LinearAllocator(ShmemUniquePtr& pool, const size_t offset_bytes = 0) :
        pool_(pool), offset_bytes_(offset_bytes) {
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
        ::memset(p, 0, n * sizeof(T)); // Считаем, что работаем с POD-типами, поэтому просто обнуляем их
    }

    size_type max_size() const noexcept {
        return pool_.get_deleter().getBufferSize() / sizeof(T);
    }

    //template<class U, class... Args>
    //void construct(U* p, Args&&... args) {}

    template <class U>
    friend bool operator==(const LinearAllocator<T>& left, const LinearAllocator<U>& right) noexcept;
    template <class U>
    friend bool operator!=(const LinearAllocator<T>& left, const LinearAllocator<U>& right) noexcept;
};

template <class T, class U>
bool operator==(const LinearAllocator<T>& left, const LinearAllocator<U>& right) noexcept {
    return left.pool_ == right.pool_;
}

template <class T, class U>
bool operator!=(const LinearAllocator<T>& left, const LinearAllocator<U>& right) noexcept {
    return !(left == right);
}

}  // namespace shmem

#endif  // ALLOCATOR_HPP
