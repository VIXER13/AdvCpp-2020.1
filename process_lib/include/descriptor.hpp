#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

namespace process_lib {

class Descriptor {
    int fd_ = -1;

 public:
    Descriptor() = default;
    Descriptor(const int fd) noexcept;
    Descriptor(const Descriptor& other) = delete;
    Descriptor(Descriptor&& other) noexcept;
    Descriptor& operator=(const Descriptor& other) = delete;
    Descriptor& operator=(Descriptor&& other) noexcept;
    ~Descriptor() noexcept;
    operator int() const noexcept;
};

}  // namespace process_lib

#endif  // DESCRIPTOR_HPP
