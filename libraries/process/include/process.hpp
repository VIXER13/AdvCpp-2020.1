#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <sys/types.h>
#include <string>
#include "descriptor.hpp"

namespace process {

class Process {
    file_descriptor::Descriptor write_in_, read_out_;
    pid_t pid_ = -1;
    bool readable_ = false;

 public:
    Process(const Process&) = delete;
    Process& operator=(const Process& other) = delete;

    Process(Process&&) = default;
    Process& operator=(Process&& other) = default;

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

}  // namespace process

#endif  // PROCESS_HPP
