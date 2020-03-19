#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <array>
#include <exception>

namespace process_lib {

class Process {
    enum : size_t {READ = 0, WRITE = 1};
    std::array<int, 2> fd_in  = {-1, -1},
                       fd_out = {-1, -1};
    pid_t pid = -1;

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

}

#endif // PROCESS_HPP
