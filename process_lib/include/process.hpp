#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <exception>
#include "descriptor.hpp"

namespace process_lib {

class Process {
    Descriptor write_in, read_out;
    pid_t pid = -1;
    bool readable = false;

 public:
    explicit Process(const std::string& path);
    ~Process();

    size_t write(const void* data, size_t len);
    void writeExact(const void* data, size_t len);
    size_t read(void* data, size_t len);
    void readExact(void* data, size_t len);

    bool isReadable() const;
    void closeStdin();

    void close();
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

#endif  // PROCESS_HPP
