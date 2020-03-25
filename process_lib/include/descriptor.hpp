#ifndef DESCRIPTOR_HPP
#define DESCRIPTOR_HPP

#include <string>
#include <exception>

namespace process_lib {

class Descriptor {
    int fd_ = -1;
    void move(Descriptor&& other) noexcept;

 public:
    explicit Descriptor() = default;
    explicit Descriptor(const int fd) noexcept;
    Descriptor(const Descriptor& other) = delete;
    Descriptor(Descriptor&& other) noexcept;
    Descriptor& operator=(const Descriptor& other) = delete;
    Descriptor& operator=(Descriptor&& other) noexcept;
    ~Descriptor() noexcept;
    operator int() const noexcept;
};

class ProcessException : public std::exception {
    std::string message;

 public:
    explicit ProcessException(const std::string& str) :
        message(str) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

}  // namespace process_lib

#endif  // DESCRIPTOR_HPP
