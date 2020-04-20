#ifndef SHMAP_HPP
#define SHMAP_HPP

#include <map>
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

    ShMap(const size_t reserve_bytes = sizeof(map_type) + sizeof(Semaphore)) :
        memory_(shmem::makeShmemUniquePtr(reserve_bytes)) {
        allocator_type alloc(memory_, sizeof(map_type) + sizeof(Semaphore));
        map_type* map = &getMap();
        map = new (map) map_type{alloc};
        Semaphore* semaphore = &getSemaphore();
        semaphore = new (semaphore) Semaphore{};
    }

    ShMap(ShMap&&) noexcept = default;
    ShMap& operator=(ShMap&&) noexcept = default;
    ~ShMap() noexcept = default;

    // Доступ к элементам

    T& at(const Key& key) {
        SemaphoreLock lock(getSemaphore());
        return getMap().at(key);
    }

    const T& at(const Key& key) const {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().at(key);
    }

    T& operator[](const Key& key) {
        SemaphoreLock lock(getSemaphore());
        return getMap()[key];
    }

    T& operator[](Key&& key) {
        SemaphoreLock lock(getSemaphore());
        return getMap()[key];
    }

    // Итераторы

    iterator begin() noexcept {
        SemaphoreLock lock(getSemaphore());
        return getMap().begin();
    }

    const_iterator begin() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().begin();
    }

    const_iterator cbegin() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().cbegin();
    }

    iterator end() noexcept {
        SemaphoreLock lock(getSemaphore());
        return getMap().end();
    }

    const_iterator end() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().end();
    }

    const_iterator cend() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().cend();
    }

    reverse_iterator rbegin() noexcept {
        SemaphoreLock lock(getSemaphore());
        return getMap().rbegin();
    }

    const_reverse_iterator rbegin() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().rbegin();
    }

    const_reverse_iterator crbegin() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().crbegin();
    }

    reverse_iterator rend() noexcept {
        SemaphoreLock lock(getSemaphore());
        return getMap().rend();
    }

    const_reverse_iterator rend() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().rend();
    }

    const_reverse_iterator crend() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().crend();
    }

    // Ёмкость

    bool empty() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().empty();
    }

    size_type size() const noexcept {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().size();
    }

    size_type max_size() const noexcept {
        return getMap().max_size();
    }

    // Изменение

    void clear() noexcept {
        SemaphoreLock lock(getSemaphore());
        getMap().clear();
    }

    std::pair<iterator,bool> insert(const value_type& value) {
        SemaphoreLock lock(getSemaphore());
        return getMap().insert(value);
    }

    template<class P>
    std::pair<iterator,bool> insert(P&& value) {
        SemaphoreLock lock(getSemaphore());
        return getMap().insert(value);
    }

    iterator insert(const_iterator hint, const value_type& value) {
        SemaphoreLock lock(getSemaphore());
        return getMap().insert(hint, value);
    }

    template<class P>
    iterator insert(const_iterator hint, P&& value) {
        SemaphoreLock lock(getSemaphore());
        return getMap().insert(hint, value);
    }

    template<class InputIt>
    void insert(InputIt first, InputIt last) {
        SemaphoreLock lock(getSemaphore());
        getMap().insert(first, last);
    }

    void insert(std::initializer_list<value_type> ilist) {
        SemaphoreLock lock(getSemaphore());
        getMap().insert(ilist);
    }

    template<class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        SemaphoreLock lock(getSemaphore());
        return getMap().emplace(args...);
    }

    template <class... Args>
    iterator emplace_hint(const_iterator hint, Args&&... args) {
        SemaphoreLock lock(getSemaphore());
        return getMap().emplace_hint(hint, args...);
    }

    iterator erase(const_iterator position) {
        SemaphoreLock lock(getSemaphore());
        return getMap().erase(position);
    }

    iterator erase(const_iterator first, const_iterator last) {
        SemaphoreLock lock(getSemaphore());
        return getMap().erase(first, last);
    }

    size_type erase(const key_type& key) {
        SemaphoreLock lock(getSemaphore());
        return getMap().erase(key);
    }

    void swap(ShMap& other) noexcept {
        SemaphoreLock lock(getSemaphore());
        std::swap(memory_, other.memory_);
    }

    // Поиск

    size_type count(const Key& key) const {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().count(key);
    }

    iterator find(const Key& key) {
        SemaphoreLock lock(getSemaphore());
        return getMap().find(key);
    }

    const_iterator find(const Key& key) const {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().find(key);
    }

    std::pair<iterator, iterator> equal_range(const Key& key) {
        SemaphoreLock lock(getSemaphore());
        return getMap().equal_range(key);
    }

    std::pair<const_iterator, const_iterator> equal_range(const Key& key) const {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().equal_range(key);
    }

    iterator lower_bound(const Key& key) {
        SemaphoreLock lock(getSemaphore());
        return getMap().lower_bound(key);
    }

    const_iterator lower_bound(const Key& key) const {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().lower_bound(key);
    }

    iterator upper_bound(const Key& key) {
        SemaphoreLock lock(getSemaphore());
        return getMap().upper_bound(key);
    }

    const_iterator upper_bound(const Key& key) const {
        SemaphoreLock lock(const_cast<Semaphore&>(getSemaphore()));
        return getMap().upper_bound(key);
    }

    // Наблюдатели

    key_compare key_comp() const {
        return getMap().key_comp();
    }

    typename map_type::value_compare value_comp() const {
        return getMap().value_comp();
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

    map_type& getMap() noexcept {
        return *static_cast<map_type*>(memory_.get());
    }

    const map_type& getMap() const noexcept {
        return *static_cast<const map_type*>(memory_.get());
    }

    Semaphore& getSemaphore() noexcept {
        return *reinterpret_cast<Semaphore*>(size_type(memory_.get()) + sizeof(map_type));
    }

    const Semaphore& getSemaphore() const noexcept {
        return *reinterpret_cast<const Semaphore*>(size_type(memory_.get()) + sizeof(map_type));
    }
};

template<class Key, class T, class Compare>
bool operator==(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    SemaphoreLock lock_left (const_cast<ShMap<Key, T, Compare>&>(left ).getSemaphore()),
                  lock_right(const_cast<ShMap<Key, T, Compare>&>(right).getSemaphore());
    return left.getMap() == right.getMap();
}

template<class Key, class T, class Compare>
bool operator!=(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    SemaphoreLock lock_left (const_cast<ShMap<Key, T, Compare>&>(left ).getSemaphore()),
                  lock_right(const_cast<ShMap<Key, T, Compare>&>(right).getSemaphore());
    return left.getMap() != right.getMap();
}

template<class Key, class T, class Compare>
bool operator<(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    SemaphoreLock lock_left (const_cast<ShMap<Key, T, Compare>&>(left ).getSemaphore()),
                  lock_right(const_cast<ShMap<Key, T, Compare>&>(right).getSemaphore());
    return left.getMap() < right.getMap();
}

template<class Key, class T, class Compare>
bool operator<=(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    SemaphoreLock lock_left (const_cast<ShMap<Key, T, Compare>&>(left ).getSemaphore()),
                  lock_right(const_cast<ShMap<Key, T, Compare>&>(right).getSemaphore());
    return left.getMap() <= right.getMap();
}

template<class Key, class T, class Compare>
bool operator>(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    SemaphoreLock lock_left (const_cast<ShMap<Key, T, Compare>&>(left ).getSemaphore()),
                  lock_right(const_cast<ShMap<Key, T, Compare>&>(right).getSemaphore());
    return left.getMap() > right.getMap();
}

template<class Key, class T, class Compare>
bool operator>=(const ShMap<Key, T, Compare>& left, const ShMap<Key, T, Compare>& right) {
    SemaphoreLock lock_left (const_cast<ShMap<Key, T, Compare>&>(left ).getSemaphore()),
                  lock_right(const_cast<ShMap<Key, T, Compare>&>(right).getSemaphore());
    return left.getMap() >= right.getMap();
}

template<class Key, class T, class Compare>
void swap(ShMap<Key, T, Compare>& left, ShMap<Key, T, Compare>& right) {
    SemaphoreLock lock_left(left.getSemaphore()), lock_right(right.getSemaphore());
    left.swap(right);
}

}  // namespace shmem

#endif  // SHMAP_HPP
