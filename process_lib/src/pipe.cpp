#include "pipe.hpp"
#include <unistd.h>

namespace process_lib {

Pipe::Pipe() {
    std::array<int, 2> fds = {-1, -1};
    if(pipe(fds.data()) < 0) {
        throw ProcessException{"Pipes failed"};
    }
    fds_[0] = Descriptor(fds[0]);
    fds_[1] = Descriptor(fds[1]);
}

Pipe::Pipe(Pipe&& other) noexcept {
    move(std::move(other));
}

Pipe& Pipe::operator=(Pipe&& other) noexcept {
    move(std::move(other));
    return *this;
}

void Pipe::move(Pipe&& other) noexcept {
    fds_[0] = std::move(other.fds_[0]);
    fds_[1] = std::move(other.fds_[1]);
}

Descriptor& Pipe::getDescriptor(const Side side) noexcept {
    return fds_[size_t(side)];
}

}