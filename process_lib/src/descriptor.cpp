#include "descriptor.hpp"
#include <unistd.h>

namespace process_lib {

Descriptor::Descriptor(const int fd) noexcept :
    fd_(fd) {}

Descriptor::Descriptor(Descriptor&& other) noexcept {
    std::swap(fd_, other.fd_);
    other.close();
}

Descriptor& Descriptor::operator=(Descriptor&& other) noexcept {
    std::swap(fd_, other.fd_);
    other.close();
    return *this;
}

Descriptor::~Descriptor() noexcept {
    close();
}

Descriptor::operator int() const noexcept {
    return fd_;
}

void Descriptor::close() noexcept {
    if (fd_ < 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

}  // namespace process_lib
