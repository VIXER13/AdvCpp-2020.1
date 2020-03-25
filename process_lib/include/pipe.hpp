#ifndef PIPE_HPP
#define PIPE_HPP

#include "descriptor.hpp"
#include <array>

namespace process_lib {

class Pipe {
    std::array<Descriptor, 2> fds_;
    void move(Pipe&& other) noexcept;

 public:
    explicit Pipe();
    Pipe(const Pipe&) = delete;
    Pipe(Pipe&& other) noexcept;
    Pipe& operator=(const Pipe&) = delete;
    Pipe& operator=(Pipe&& other) noexcept;
    Descriptor& getDescriptor(const size_t i) noexcept;
};

}

#endif  // PIPE_HPP
