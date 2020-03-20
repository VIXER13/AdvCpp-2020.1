#ifndef PROCESS_HPP
#define PROCESS_HPP

#include <string>
#include <array>
#include <exception>

namespace process_lib {

class Process {
    int write_in = -1, read_out = -1;
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

 private:
    void closeAllPipes(const std::array<int, 2>& fd_in, const std::array<int, 2>& fd_out) const;
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
