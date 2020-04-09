#ifndef PIPE_HPP
#define PIPE_HPP

#include "descriptor.hpp"
#include <array>

namespace file_descriptor {

class Pipe {
    std::array<Descriptor, 2> fds_;

 public:
    enum class Side : uint8_t {READ, WRITE};

    Pipe(const Pipe&) = delete;
    Pipe& operator=(const Pipe&) = delete;

    Pipe(Pipe&& other) = default;
    Pipe& operator=(Pipe&& other) = default;

    explicit Pipe();
    Descriptor& getDescriptor(const Side side) noexcept;
};

}

#endif  // PIPE_HPP
