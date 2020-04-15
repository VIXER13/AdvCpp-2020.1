#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP

#include "shared_memory.hpp"

namespace shmem {

template<class T>
class Allocator {
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
        using other = Allocator<U>;
    };

    Allocator(ShmemUniquePtr& pool) noexcept :
        pool_(pool) {}
    Allocator(const Allocator& other) noexcept = default;
    ~Allocator() noexcept = default;

    pointer allocate(size_type n) {
        if (static_cast<void*>(std::next(static_cast<pointer>(pool_.get()), offset_ + n)) >
            static_cast<void*>(std::next(static_cast<int8_t*>(pool_.get()), pool_.get_deleter().getSize()))) {
            throw std::bad_alloc{};
        }
        offset_ += n;
        return std::next(static_cast<pointer>(pool_.get()), offset_ - n);
    }

    void deallocate(pointer p, size_type n) {}

    size_type max_size() const noexcept {
        pool_.get_deleter().getSize();
    }

    //template<class U, class... Args>
    //void construct(U* p, Args&&... args) {}

    template <class U>
    friend bool operator==(const Allocator<T>& left, const Allocator<U>& right) noexcept;
    template <class U>
    friend bool operator!=(const Allocator<T>& left, const Allocator<U>& right) noexcept;

 private:
    ShmemUniquePtr& pool_;
    size_t offset_ = 0;
};

template <class T, class U>
bool operator==(const Allocator<T>& left, const Allocator<U>& right) noexcept {
    return left.pool_ == right.pool_;
}

template <class T, class U>
bool operator!=(const Allocator<T>& left, const Allocator<U>& right) noexcept {
    return !(left == right);
}

}  // namespace shmem

#endif  // ALLOCATOR_HPP
