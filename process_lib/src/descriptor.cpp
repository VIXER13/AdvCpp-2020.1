#include "descriptor.hpp"
#include <unistd.h>

namespace process_lib {

Descriptor::Descriptor(const int fd) noexcept :
    fd_(fd) {}

Descriptor::Descriptor(Descriptor&& other) noexcept {
    move(std::move(other));
}

Descriptor& Descriptor::operator=(Descriptor&& other) noexcept {
    move(std::move(other));
    return *this;
}

Descriptor::~Descriptor() noexcept {
    close(fd_);
}

Descriptor::operator int() const noexcept {
    return fd_;
}

void Descriptor::move(Descriptor&& other) noexcept {
    fd_ = other.fd_;
    other.fd_ = -1;
}

}  // namespace process_lib
