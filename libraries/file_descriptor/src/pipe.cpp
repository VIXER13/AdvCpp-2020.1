#include "pipe.hpp"
#include <unistd.h>
#include <utility>
#include "file_descriptor_exception.hpp"

namespace file_descriptor {

Pipe::Pipe() {
    std::array<int, 2> fds = {-1, -1};
    if(pipe(fds.data()) < 0) {
        throw FileDescriptorException{"Pipes failed"};
    }
    fds_[0] = Descriptor(fds[0]);
    fds_[1] = Descriptor(fds[1]);
}

Descriptor& Pipe::getDescriptor(const Side side) noexcept {
    return fds_[size_t(side)];
}

}