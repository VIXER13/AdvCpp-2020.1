#ifndef SHMAP_HPP
#define SHMAP_HPP

#include <map>
#include <mutex>
#include "linear_allocator.hpp"
#include "semaphore.hpp"

namespace shmem {

template<class Key, class T, class Compare = std::less<Key>>
class ShMap {
 public:
    using size_type              = std::size_t;
    using difference_type        = std::ptrdiff_t;
    using key_type               = Key;
    using mapped_type            = T;
    using key_compare            = Compare;
    using value_type             = std::pair<const key_type, mapped_type>;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using allocator_type         = shmem::LinearAllocator<value_type>;
    using map_type               = std::map<key_type, mapped_type, key_compare, allocator_type>;
    using pointer                = typename allocator_type::pointer;
    using const_pointer          = typename allocator_type::const_pointer;
    using iterator               = typename map_type::iterator;
    using const_iterator         = typename map_type::const_iterator;
    using reverse_iterator       = typename map_type::reverse_iterator;
    using const_reverse_iterator = typename map_type::const_reverse_iterator;

    ShMap(const size_t reserve_bytes) :
        memory_(shmem::makeShmemUniquePtr(reserve_bytes)) {
        shmem::LinearAllocator<Semaphore> semaphore_alloc{memory_};
        semaphore_ = semaphore_alloc.allocate(1);
        semaphore_alloc.construct(semaphore_);
        shmem::LinearAllocator<map_type> map_alloc{semaphore_alloc};
        map_ = map_alloc.allocate(1);
        map_alloc.construct(map_, map_alloc);
    }

    ShMap(ShMap&&) = default;
    ShMap& operator=(ShMap&&) = default;
    ~ShMap() = default;

    // Доступ к элементам

    T& at(const Key& key) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->at(key);
    }

    const T& at(const Key& key) const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->at(key);
    }

    T& operator[](const Key& key) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return (*map_)[key];
    }

    T& operator[](Key&& key) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return (*map_)[std::move(key)];
    }

    // Итераторы

    iterator begin() {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->begin();
    }

    const_iterator begin() const  {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->begin();
    }

    const_iterator cbegin() const  {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->cbegin();
    }

    iterator end() {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->end();
    }

    const_iterator end() const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->end();
    }

    const_iterator cend() const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->cend();
    }

    reverse_iterator rbegin() {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->rbegin();
    }

    const_reverse_iterator rbegin() const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->rbegin();
    }

    const_reverse_iterator crbegin() const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->crbegin();
    }

    reverse_iterator rend() {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->rend();
    }

    const_reverse_iterator rend() const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->rend();
    }

    const_reverse_iterator crend() const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->crend();
    }

    // Ёмкость

    bool empty() const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->empty();
    }

    size_type size() const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->size();
    }

    size_type max_size() const {
        return map_->max_size();
    }

    // Изменение

    void clear() {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        map_->clear();
    }

    std::pair<iterator,bool> insert(const value_type& value) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->insert(value);
    }

    template<class P>
    std::pair<iterator,bool> insert(P&& value) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->insert(std::move(value));
    }

    iterator insert(const_iterator hint, const value_type& value) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->insert(hint, value);
    }

    template<class P>
    iterator insert(const_iterator hint, P&& value) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->insert(hint, std::move(value));
    }

    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        map_->insert(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        map_->insert(ilist);
    }

    template<class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->emplace(std::forward<Args>(args)...);
    }

    template <class... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->emplace_hint(hint, std::forward<Args>(args)...);
    }

    iterator erase(const_iterator position) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->erase(position);
    }

    iterator erase(const_iterator first, const_iterator last) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->erase(first, last);
    }

    size_type erase(const key_type& key) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->erase(key);
    }

    void swap(ShMap& other) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        std::swap(memory_, other.memory_);
    }

    // Поиск

    size_type count(const Key& key) const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->count(key);
    }

    iterator find(const Key& key) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->find(key);
    }

    const_iterator find(const Key& key) const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->find(key);
    }

    std::pair<iterator, iterator> equal_range(const Key& key) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->equal_range(key);
    }

    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->equal_range(key);
    }

    iterator lower_bound(const Key& key) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->lower_bound(key);
    }

    const_iterator lower_bound(const Key& key) const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->lower_bound(key);
    }

    iterator upper_bound(const Key& key) {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->upper_bound(key);
    }

    const_iterator upper_bound(const Key& key) const {
        const std::lock_guard<Semaphore> lock(*semaphore_);
        return map_->upper_bound(key);
    }

    // Наблюдатели

    key_compare key_comp() const {
        return map_->key_comp();
    }

    typename map_type::value_compare value_comp() const {
        return map_->value_comp();
    }

    template<class Key_, class T_, class Compare_>
    friend bool operator==(const ShMap<Key_, T_, Compare_>& left, const ShMap<Key_, T_, Compare_>& right);
    template<class Key_, class T_, class Compare_>
    friend bool operator!=(const ShMap<Key_, T_, Compare_>& left, const ShMap<Key_, T_, Compare_>& right);
    template<class Key_, class T_, class Compare_>
    friend bool operator< (const ShMap<Key_, T_, Compare_>& left, const ShMap<Key_, T_, Compare_>& right);
    template<class Key_, class T_, class Compare_>
    friend bool operator<=(const ShMap<Key_, T_, Compare_>& left, const ShMap<Key_, T_, Compare_>& right);
    template<class Key_, class T_, class Compare_>
    friend bool operator> (const ShMap<Key_, T_, Compare_>& left, const ShMap<Key_, T_, Compare_>& right);
    template<class Key_, class T_, class Compare_>
    friend bool operator>=(const ShMap<Key_, T_, Compare_>& left, const ShMap<Key_, T_, Compare_>& right);

    template<class Key_, class T_, class Compare_>
    friend void swap(ShMap<Key_, T_, Compare_>& left, ShMap<Key_, T_, Compare_>& right);

 private:
    ShmemUniquePtr memory_;
    mutable Semaphore* semaphore_ = nullptr;
    map_type* map_ = nullptr;
};

template<class Key, class T, class Compare>
bool operator==(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    const std::scoped_lock<Semaphore, Semaphore> lock(*left.semaphore_, *right.semaphore_);
    return *left.map_ == *right.map_;
}

template<class Key, class T, class Compare>
bool operator!=(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    const std::scoped_lock<Semaphore, Semaphore> lock(*left.semaphore_, *right.semaphore_);
    return *left.map_ != *right.map_;
}

template<class Key, class T, class Compare>
bool operator<(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    const std::scoped_lock<Semaphore, Semaphore> lock(*left.semaphore_, *right.semaphore_);
    return *left.map_ < *right.map_;
}

template<class Key, class T, class Compare>
bool operator<=(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    const std::scoped_lock<Semaphore, Semaphore> lock(*left.semaphore_, *right.semaphore_);
    return *left.map_ <= *right.map_;
}

template<class Key, class T, class Compare>
bool operator>(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    const std::scoped_lock<Semaphore, Semaphore> lock(*left.semaphore_, *right.semaphore_);
    return *left.map_ > *right.map_;
}

template<class Key, class T, class Compare>
bool operator>=(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    const std::scoped_lock<Semaphore, Semaphore> lock(*left.semaphore_, *right.semaphore_);
    return *left.map_ >= *right.map_;
}

template<class Key, class T, class Compare>
void swap(ShMap<Key, T, Compare>& left, ShMap<Key, T, Compare>& right) {
    const std::scoped_lock<Semaphore, Semaphore> lock(*left.semaphore_, *right.semaphore_);
    left.swap(right);
}

}  // namespace shmem

#endif  // SHMAP_HPP
