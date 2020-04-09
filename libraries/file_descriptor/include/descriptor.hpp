#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

namespace file_descriptor {

class Descriptor {
    int fd_ = -1;

 public:
    Descriptor(const Descriptor& other) = delete;
    Descriptor& operator=(const Descriptor& other) = delete;

    explicit Descriptor() = default;
    
    explicit Descriptor(const int fd) noexcept;
    Descriptor(Descriptor&& other) noexcept;
    Descriptor& operator=(Descriptor&& other) noexcept;
    ~Descriptor() noexcept;

    operator int() const noexcept;

    void close() noexcept;
};

}  // namespace process_lib

#endif  // DESCRIPTOR_HPP
